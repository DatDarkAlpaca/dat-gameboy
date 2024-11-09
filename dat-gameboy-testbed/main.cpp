#include "pch.hpp"
#include "gameboy/file.hpp"
#include "gameboy/gameboy.hpp"
#include "gameboy/instruction/instruction_set.hpp"

#include "platform/platform.hpp"

void handle_windowing()
{
    using namespace dat;

    Window window;
    WindowProperties properties;
    GraphicsSpecification specification{ GraphicsAPI::OPENGL, 4, 5 };

    setup_opengl_windowing(window, properties, specification);

    while (window.is_open())
    {
        window.poll_events();
    }
}

// https://gbdev.io/pandocs/Specifications.html
// https://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM
// https://gbdev.io/gb-opcodes/optables/
// https://rgbds.gbdev.io/docs/v0.8.0/gbz80.7

int main()
{
    using namespace dat;

    dat::initialize_logger();
    dat::initialize_platform();

    auto json = dat::load_opcodes_json("res/opcodes.json");
    dat::initialize_instruction_set(json);

    dat::s_Gameboy gameboy;
    dat::load_ROM_file("res/test.bin", gameboy.memory.rom());
    gameboy.run();

    std::cout << "A: 0x" << std::hex << (int)gameboy.cpu.registers.A   << "\n";
    std::cout << "B: 0x" << std::hex << (int)gameboy.cpu.registers.B() << "\n";
    std::cout << "C: 0x" << std::hex << (int)gameboy.cpu.registers.C() << "\n";
    std::cout << "D: 0x" << std::hex << (int)gameboy.cpu.registers.D() << "\n";
    std::cout << "E: 0x" << std::hex << (int)gameboy.cpu.registers.E() << "\n";
    std::cout << "H: 0x" << std::hex << (int)gameboy.cpu.registers.H() << "\n";
    std::cout << "L: 0x" << std::hex << (int)gameboy.cpu.registers.L() << "\n";
    std::cout << "PC: " << std::hex << (int)gameboy.cpu.registers.PC << "\n";

    return 0;
}