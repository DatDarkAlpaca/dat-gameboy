#pragma once
#include "common.hpp"
#include "core/event/events.hpp"
#include "core/cartridge_handle.hpp"
#include "gameboy/components/cartridge/cartridge.hpp"

namespace dat
{
	constexpr inline u64 LOAD_ROM_EVENT						= static_cast<u64>(EventType::AMOUNT) + 0;
	constexpr inline u64 PROPAGATE_LOAD_ROM_EVENT			= static_cast<u64>(EventType::AMOUNT) + 1;
	constexpr inline u64 INSERT_ROM_EVENT					= static_cast<u64>(EventType::AMOUNT) + 2;
	constexpr inline u64 CARTRIDGE_REMOVE_EVENT				= static_cast<u64>(EventType::AMOUNT) + 3;
	constexpr inline u64 PROPAGATE_CARTRIDGE_REMOVE_EVENT	= static_cast<u64>(EventType::AMOUNT) + 4;
	constexpr inline u64 CARTRIDGE_FOCUS_EVENT				= static_cast<u64>(EventType::AMOUNT) + 5;
	constexpr inline u64 DISASSEMBLER_STEP_EVENT			= static_cast<u64>(EventType::AMOUNT) + 6;
}

namespace dat
{
	// Called when a load() cartridge event is issued. Not necessarily when the cartridge is insert into the gameboy.
	// This event is consumed by the cartridge holder.
	struct LoadROMFileEvent : public Event
	{
		DEFINE_EVENT(static_cast<EventType>(LOAD_ROM_EVENT));

	public:
		LoadROMFileEvent(const dat_shared<ICartridge>& cartridge)
			: cartridge(cartridge)
		{

		}

	public:
		dat_shared<ICartridge> cartridge;
	};

	// Issued by the cartridge holder to pass the cartridge information to other frames.
	struct PropagateLoadROMFileEvent : public Event
	{
		DEFINE_EVENT(static_cast<EventType>(PROPAGATE_LOAD_ROM_EVENT));

	public:
		PropagateLoadROMFileEvent(cartridge_handle handle)
			: handle(handle)
		{

		}

	public:
		cartridge_handle handle;
	};

	// Called when the cartridge is placed into the gameboy. Must pass through the cartridge_holder, therefore it only uses
	// a handle as a parameter.
	struct InsertCartridgeEvent : public Event
	{
		DEFINE_EVENT(static_cast<EventType>(INSERT_ROM_EVENT));

	public:
		InsertCartridgeEvent(cartridge_handle handle)
			: handle(handle)
		{

		}

	public:
		cartridge_handle handle;
	};

	// Called when a cartridge is removed. Passes through the cartridge holder.
	struct CartridgeRemoveEvent : public Event
	{
		DEFINE_EVENT(static_cast<EventType>(CARTRIDGE_REMOVE_EVENT));

	public:
		CartridgeRemoveEvent(cartridge_handle handle)
			: handle(handle)
		{

		}

	public:
		cartridge_handle handle;
	};
	
	// Called when a cartridge gets clicked
	struct CartridgeFocusEvent : public Event
	{
		DEFINE_EVENT(static_cast<EventType>(CARTRIDGE_FOCUS_EVENT));

	public:
		CartridgeFocusEvent(cartridge_handle handle)
			: handle(handle)
		{

		}

	public:
		cartridge_handle handle;
	};

	// Disassembler events:
	struct DisassemblerStepEvent : public Event
	{
		DEFINE_EVENT(static_cast<EventType>(DISASSEMBLER_STEP_EVENT));
	};
}