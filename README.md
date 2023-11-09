# Practical Modern C++

Practical Modern C++, slides and part of code.

## Preview Slides
* VSCode with Extensions:
  * `Marp for VS Code`

## Build Testing Code
### Environment
* *Linux* or *Windows + WSL + Linux*
* Our dev environment:
  * Windows 11 + WSL + Ubuntu 22.04 LTS

### Installation steps
  * Install following build toolchain by package manager of Linux
    * **cmake** >= 3.22
    * **clang** >= 14
    * **ninja-build**
  * Install following package with offcial document
    * **Vcpkg**: [Vcpkg: Overview / Quick Start: Unix](https://github.com/microsoft/vcpkg/blob/master/README.md#quick-start-unix)
  * Install IDE for developinng and debugging
    * **GDB** (up to date)
    * VSCode on your host machine
      * Change "toolchainFile" attribute in CMakePresets.json to your vcpkg.cmake file path.
      * Suggested extensions:
        * `clangd`
        * `CMake`
        * `CMake Tools`
        * `C/C++ Extension Pack`
  * Build by CMake or VSCode.

