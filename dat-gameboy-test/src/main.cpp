#undef DAT_ENABLE_DEBUG_LOG
#undef DAT_ENABLE_WARNINGS

#include "pch.hpp"
#include "tests.hpp"

#define KLKL std::uppercase << std::setfill('0') << std::setw(2) << std::right << std::hex
#define YYYY std::uppercase << std::setfill('0') << std::setw(4) << std::right << std::hex

int main(int argc, char** argv)
{
	dat::initialize_logger();
	testing::InitGoogleTest(&argc, argv);

	::testing::GTEST_FLAG(filter) = "GameboyFileTests_0X*";

	return RUN_ALL_TESTS(); 

	dat::s_Gameboy gameboy;
	gameboy.load_bootloader_rom(dat::read_ROM("res/roms/DMG_ROM.gb"));

	auto cartridge = dat::read_cartrige("res/roms/blargg/cpu_instrs/individual/03-op sp,hl.gb");
	gameboy.load_cartridge(cartridge);

	gameboy.cpu.A = 0x01;
	gameboy.cpu.F.set(0xB0);
	gameboy.cpu.BC.set(0x0013);
	gameboy.cpu.DE.set(0x00D8);
	gameboy.cpu.HL.set(0x014D);
	gameboy.cpu.SP.set(0xFFFE);
	gameboy.cpu.PC.set(0x0100);

	std::ofstream file("C:\\Users\\paulo\\Desktop\\gameboy-doctor\\logout.txt");

	gameboy.cpu.tick();

	u32 cpuTicksPerFrame = 17592;
	for (u32 i = 0; i < cpuTicksPerFrame; ++i)
	{
		file <<  "A:"  << KLKL << (int)gameboy.cpu.A					<< ' ';
		file <<  "F:"  << KLKL << (int)gameboy.cpu.F.get_register()		<< ' ';
		file <<  "B:"  << KLKL << (int)gameboy.cpu.BC.get_msb()			<< ' ';
		file <<  "C:"  << KLKL << (int)gameboy.cpu.BC.get_lsb()			<< ' ';
		file <<  "D:"  << KLKL << (int)gameboy.cpu.DE.get_msb()			<< ' ';
		file <<  "E:"  << KLKL << (int)gameboy.cpu.DE.get_lsb()			<< ' ';
		file <<  "H:"  << KLKL << (int)gameboy.cpu.HL.get_msb()			<< ' ';
		file <<  "L:"  << KLKL << (int)gameboy.cpu.HL.get_lsb()			<< ' ';
		file <<  "SP:" << KLKL << (int)gameboy.cpu.SP.get_msb() << KLKL << (int)gameboy.cpu.SP.get_lsb() << ' ';
		file <<  "PC:" << KLKL << (int)gameboy.cpu.PC.get_msb() << KLKL << (int)gameboy.cpu.PC.get_lsb() << ' ';

		file << "PCMEM:" << KLKL << (int)gameboy.memory.read(gameboy.cpu.PC.get()) << ',';
		file << KLKL << (int)gameboy.memory.read(gameboy.cpu.PC.get() + 1) << ',';
		file << KLKL << (int)gameboy.memory.read(gameboy.cpu.PC.get() + 2) << ',';
		file << KLKL << (int)gameboy.memory.read(gameboy.cpu.PC.get() + 3) << '\n';

		gameboy.cpu.tick();
		gameboy.timer.tick();
		gameboy.ppu.tick();
	}
	
	return 0;
	//return RUN_ALL_TESTS();
}