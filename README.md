# dat-gameboy

> dat-gameboy is a faulty, barely functional gameboy emulator written in C++. It's just a hobby project, so it isn't meant to be the fastest, let alone the most accurate emulator out there.

# Getting Started

This repository uses [Premake 5](https://github.com/premake/premake-core/releases) to generate project files, but it has only been tested with [Visual Studio](https://visualstudio.microsoft.com/pt-br/downloads/).

## Prerequisites

You'll need to install `python`, and `premake5` in order to build this project.

* [Python 3](https://www.python.org/downloads/)
* [Premake 5](https://github.com/premake/premake-core/releases)

You may also need to add an environment variable to use `premake5` on Windows machines.

## Installation
1. **Clone the repository**
    ```bash
    git clone --recurse-submodules https://github.com/DatDarkAlpaca/dat-gameboy
    ```

2. **Generate the project**
    If you're building on a Windows machine, you can use the `build.cmd` file to execute the Python script. 
    This automatically calls premake, so you must specify the project file of your choice.

    ```bash
    .\build <action: vs2022, vs2021, gmake, ...>
    ```

3. **Compile the project**
   Use the generated project files to build the project. 
   After that, you should have a working copy under `dat-gameboy/build/bin/{configuration}/dat-gameboy`. 

# References
* [GBDev - Pandocs](https://gbdev.io/pandocs)
* [GBDev - Optables](https://gbdev.io/gb-opcodes/optables/)
* [rgbds](https://rgbds.gbdev.io/docs/v0.8.0/gbz80.7)
* [Game Boy: Complete Technical Reference](https://gekkio.fi)
* [GBDev - Bootstrap](https://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM#The_.27Pokemon.27_CGB_bootstrap)
* [GBEDB - PPU](https://hacktix.github.io/GBEDG/ppu/)

# License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT) - see the [LICENSE](LICENSE) file for details
