#include <glm/gtc/matrix_transform.hpp>

#include "CapitalEngine.h"
#include "World.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

World::World() {
  _log.console("{ (X) }", "constructing World");
}

World::~World() {
  _log.console("{ (X) }", "destructing World");
}

std::vector<VkVertexInputBindingDescription> World::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
  bindingDescriptions.push_back(
      {0, sizeof(Cell), VK_VERTEX_INPUT_RATE_INSTANCE});
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
World::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
  attributeDescriptions.push_back(
      {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Cell, position)});
  attributeDescriptions.push_back(
      {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Cell, color)});
  attributeDescriptions.push_back(
      {2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Cell, size)});
  return attributeDescriptions;
}

std::vector<World::Cell> World::initializeCells() {
  const uint_fast16_t width = _control.grid.dimensions[0];
  const uint_fast16_t height = _control.grid.dimensions[1];
  const uint_fast32_t numGridPoints = width * height;
  const uint_fast32_t numAliveCells = _control.grid.totalAliveCells;
  const float gap = _control.grid.gap;
  std::array<float, 4> size = {_control.cells.size};

  if (numAliveCells > numGridPoints) {
    throw std::runtime_error(
        "!ERROR! Number of alive cells exceeds number of grid points");
  }

  std::vector<World::Cell> cells(numGridPoints);
  std::vector<bool> isAliveIndices(numGridPoints, false);

  for (uint_fast32_t i = 0; i < numGridPoints; i++) {
    isAliveIndices.push_back(false);
  }

  std::vector<uint_fast32_t> aliveCellIndices =
      _control.setCellsAliveRandomly(_control.grid.totalAliveCells);
  for (int aliveIndex : aliveCellIndices) {
    isAliveIndices[aliveIndex] = true;
  }

  const float startX = -((width - 1) * gap) / 2.0f;
  const float startY = -((height - 1) * gap) / 2.0f;

  for (uint_fast32_t i = 0; i < numGridPoints; ++i) {
    const uint_fast16_t x = static_cast<uint_fast16_t>(i % width);
    const uint_fast16_t y = static_cast<uint_fast16_t>(i / width);
    const float posX = startX + x * gap;
    const float posY = startY + y * gap;

    const std::array<float, 4> pos = {posX, posY, 0.0f, 0.0f};
    const bool isAlive = isAliveIndices[i];

    const std::array<float, 4>& color = isAlive ? blue : red;
    const std::array<int, 4>& state = isAlive ? alive : dead;

    cells[i] = {pos, color, size, state};
  }
  //_log.console(aliveCellIndices);
  return cells;
}

bool World::isIndexAlive(const std::vector<int>& aliveCells, int index) {
  return std::find(aliveCells.begin(), aliveCells.end(), index) !=
         aliveCells.end();
}

World::UniformBufferObject World::updateUniforms() {
  UniformBufferObject uniformObject{};
  uniformObject.gridDimensions = _control.grid.dimensions;
  uniformObject.passedHours = _control.timer.passedHours;
  uniformObject.model = _world.setModel();
  uniformObject.view = _world.setView();
  uniformObject.proj = _world.setProjection(_mechanics.swapChain.extent);
  uniformObject.lightDirection = {1.0f, 0.0f, 0.5f, 0.2f};
  uniformObject.cellSize = _control.cells.size;
  return uniformObject;
}

void World::updateCamera() {
  constexpr uint8_t left = 0;
  constexpr uint8_t right = 1;
  constexpr uint8_t middle = 2;

  glm::vec2 deltas[3]{};
  for (uint8_t i = 0; i < 3; ++i) {
    deltas[i] = _window.mouse.buttonDown[i].position -
                _window.mouse.previousButtonDown[i].position;
    _window.mouse.previousButtonDown[i].position =
        _window.mouse.buttonDown[i].position;
  }
  glm::vec2 leftButtonDelta = deltas[left];
  glm::vec2 rightButtonDelta = deltas[right];
  glm::vec2 middleButtonDelta = deltas[middle];

  constexpr float rotationSpeed = 0.3f * glm::pi<float>() / 180.0f;
  float turn = rotationSpeed * leftButtonDelta.x;
  float tilt = rotationSpeed * -leftButtonDelta.y;

  glm::vec3 cameraRight = glm::cross(camera.front, camera.up);

  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), turn, camera.up) *
                             glm::rotate(glm::mat4(1.0f), tilt, cameraRight);
  camera.front =
      glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera.front, 0.0f)));
  camera.up =
      glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera.up, 0.0f)));

  // Reset the roll component to maintain a level camera orientation
  camera.up = glm::cross(cameraRight, camera.front);

  constexpr float panningSpeed = 0.01f;
  glm::vec3 cameraUp = glm::cross(cameraRight, camera.front);
  camera.position += panningSpeed * rightButtonDelta.x * -cameraRight;
  camera.position += panningSpeed * rightButtonDelta.y * -cameraUp;

  constexpr float zoomSpeed = 0.01f;
  camera.position += zoomSpeed * middleButtonDelta.x * camera.front;
}

glm::mat4 World::setModel() {
  glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f));
  return model;
}

glm::mat4 World::setView() {
  updateCamera();
  glm::mat4 view;
  view =
      glm::lookAt(camera.position, camera.position + camera.front, camera.up);
  return view;
}

glm::mat4 World::setProjection(VkExtent2D& swapChainExtent) {
  glm::mat4 projection = glm::perspective(
      glm::radians(60.0f),
      swapChainExtent.width / static_cast<float>(swapChainExtent.height), 0.1f,
      10.0f);

  projection[1][1] *= -1;  // flip y axis
  projection[0][0] *= -1;  // flip x axis

  return projection;
};
