# CppND-System-Monitor

This repository contains the code for system monitor for linux OS which shows system information and information about processes running on the OS. 


![System Monitor](images/monitor.png)

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Install ncurses: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the repository: `git clone https://github.com/prathmesh-dali/CppND-System-Monitor-Project-Updated.git`

2. Build the repository: `make build`

3. Run the resulting executable: `./build/monitor`
