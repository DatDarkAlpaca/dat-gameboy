#pragma once
#include <platform/platform.hpp>

#include <gameboy/gameboy.hpp>
#include <gameboy/file.hpp>
#include <gameboy/instruction/instruction_set.hpp>

#include "utils/clock.hpp"
#include "frames/frames.hpp"
#include "events/input_handler.hpp"
#include "core/cartridge_holder.hpp"
#include "core/assets/texture_library.hpp"

namespace dat
{
	class s_DatApplication
	{
	public:
		explicit s_DatApplication();

	public:
		void run();

	private:
		void on_event(Event& event);

		void on_update();

		void on_render();

	private:
		void initialize_assets();

		void initialize_ui();

	private:
		dat_shared<s_Menubar> menuBar;
		dat_shared<s_CartridgeAnalyzer> cartridgeAnalyzer;
		dat_shared<s_Disassembler> disassembler;
		dat_shared<s_GameboyFrame> gameboyFrame;
		dat_shared<s_CartridgeFrameHolder> cartridgeFrames;
		dat_unique<s_CartridgeHolder> cartridgeHolder;
		dat_shared<s_SerialReader> serialReader;

	private:
		Window window;
		dat_unique<s_Gameboy> gameboy;
		dat_unique<s_QuadRenderer> mainRenderer;
		s_InputHandler inputHandler;

	private:
		s_TextureLibrary textureLibrary;
		asset_handle defaultTextureHandle;
		asset_handle gameboyTextureHandle;
		asset_handle cartridgeTextureHandle;
		asset_handle invalidCartridgeTextureHandle;

	private:
		s_Clock m_Clock;
		bool m_IsOpen = true;
	};
}