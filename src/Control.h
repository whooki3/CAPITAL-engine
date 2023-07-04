#pragma once

#include <array>
#include <string>
#include <vector>

constexpr int off = -1;

class Control {
 public:
  Control();
  ~Control();

  struct Timer {
    float speed = 5.0f;
    uint64_t passedHours{0};
  } timer;

  struct Grid {
    uint_fast32_t totalAliveCells = 75000;
    std::array<uint_fast16_t, 2> dimensions = {500, 500};
    float height = 0.75f;
    int heightSteps = 15;
  } grid;

  struct DisplayConfiguration {
    const char* title{"CAPITAL Engine"};
    uint16_t width = 1280;
    uint16_t height = 720;
  } display;

  struct Compute {
    const uint8_t localSizeX{8};
    const uint8_t localSizeY{8};
  } compute;

 public:
  std::vector<uint_fast32_t> setCellsAliveRandomly(uint_fast32_t numberOfCells);
  void setPassedHours();

  void setPushConstants();
};
