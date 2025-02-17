#pragma once
#include <vulkan/vulkan.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>

#include "Control.h"

class World {
 public:
  World();
  ~World();

  struct Camera {
    const float fieldOfView = 60.0f;
    const float nearClipping = 0.0001f;
    const float farClipping = 200.0f;
    glm::vec3 position{0.0f, 0.0f, 10.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, -1.0f, 0.0f};
  } camera;

  struct Light {
    std::array<float, 4> position{0.0f, 2.0f, 10.0f, 0.0f};
  } light;

  struct Cell {
    std::array<float, 4> position;
    std::array<float, 4> color;
    std::array<float, 4> size;
    std::array<int, 4> states;
    std::array<float, 4> tileSidesHeight;
    std::array<float, 4> tileCornersHeight;
  };

  struct UniformBufferObject {
    std::array<float, 4> light;
    std::array<uint32_t, 2> gridDimensions;
    float gridHeight;
    float cellSize;
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  };

  struct Tile {
    const uint32_t vertexCount{90};
    const float cubeSize{0.1f};
  } tile;

 public:
  UniformBufferObject updateUniforms();
  void updateCamera();
  float getForwardMovement(const glm::vec2& leftButtonDelta);

  std::vector<World::Cell> initializeCells();
  bool isIndexAlive(const std::vector<int>& aliveCells, int index);

  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions();
  static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

 private:
  glm::mat4 setModel();
  glm::mat4 setView();
  glm::mat4 setProjection(VkExtent2D& swapChainExtent);

  std::vector<float> setGridHeight(int amount, float min, float max);

  inline static const std::array<float, 4> red{1.0f, 0.0f, 0.0f, 1.0f};
  inline static const std::array<float, 4> blue{0.0f, 0.0f, 1.0f, 1.0f};

  inline static const std::array<int, 4> alive{1, 0, 0, 0};
  inline static const std::array<int, 4> dead{-1, 0, 0, 0};
};
