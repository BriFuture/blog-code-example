## HW 2020 software challenge

Solution written by @Brifuture. 

Best score: `2.8s online` / `4.1s offline`(my laptop: intel 8250U, 8G memory).

Change definitions in `definitions.h` to deploy different configuration.

### Dataset

Dataset could be downloaded from [github repo](https://github.com/liusen1006/2020HuaweiCodecraft-TestData).

Download this repo and move `test_data_*.txt` into the build directory of CMakeProject (If you build without CMake, the program will try to find data file in `/data/test_data.txt`).

Change `DataInputFile` in `definitions.h` with the actual filename which contains the data you downloaded.

### Build on Windows

Download `PThread.7z` from [Tag-hw2020swc](https://github.com/BriFuture/blog-code-example/releases/download/hw2020swc/pthread.7z), then extracted it into directory like `D:/software`.

CMake is used to generate project based on MSVC/gcc. If PThread is installed into location other than `D:/software/pthread`, you have to change `target_include_directories` and `target_link_libraries` in the file `CMakeLists.txt`.

> Note: pthread.dll wont be found when running the project. You can add it into path by either of the following:
> - Add pthread dll location into **PATH**. 
> - Open VistualStudio click project `Main` properties, Choose `Debug` tag, set `PATH=%PATH%D:\software\pthread\dll\x64;` in the environment variables.

### Build on linux

In the shell terminal, type `./run.sh` without CMake, type `./runcmake.sh` to build project with CMake.

### Strategy

See `main.cpp` to get more details.
