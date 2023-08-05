# CAPITAL Engine
CAPITAL Engine is a cross platform Vulkan engine built in C++ for simulations and algorithms that benefit from parallel computing. Currently tested on Linux and Windows, keeping external libraries to a minimum. GLFW for platform agnostic window and input handeling, GLM for our mathematical operations. 

In it's current stage CAPITAL Engine is running Conway's Game of Life in a Vulkan compute shader. We will explore increasing the resolution within the lifespan of a cell in future updates.

![alt text](https://github.com/whooki3/CAPITAL-engine/blob/development/assets/CoverCapture.PNG?raw=true)

[Development enviroment](https://vulkan-tutorial.com/Development_environment)

## Windows development
Additional Include Directories
```
$(SolutionDir)..\Libraries\glfw-3.3.8\include
$(SolutionDir)..\Libraries\glm
C:\VulkanSDK\1.3.224.1\Include
```
Linker Additional Libraries Directories
```text
C:\VulkanSDK\1.3.224.1\Lib
$(SolutionDir)..\Libraries\glfw-3.3.8\lib-vc2022
```
Linker Additional Dependencies
```text
vulkan-1.lib
glfw3.lib
```

## Linux development
Build management using CMake
Go to **build** sub-directory:
```bash
  cmake ..
  make -j
```
The excutable **CapitalEngine** is compiled in the **bin** sub-directory. De .spv files in **shaders** sub-directory.
Executing: Go to the project root directory **CAPITAL-Engine**:
```bash
./bin/CapitalEngine
```



Started from the tutorial series by *Sascha Willems*: [Vulkan tutorial](https://vulkan-tutorial.com/Introduction).

