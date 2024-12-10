#pragma once
#include "common.hpp"

namespace dat
{
	enum e_MBCType
	{
		INVALID,
		NO_MBC,
		MBC1,
		MBC2,
		MBC3,
		MBC5,
		MBC6,
		MBC7,
		VARIOUS,
	};

	struct s_CartridgeHeader
	{
		u8 entryPoint[4];				// 0100-0103
		u8 nintendoLogo[48];			// 0104-0133
		u8 title[15];					// 0134-013E
		// u8 manufacturerCode[4];		// 013F-0142 [Part of title]
		u8 cgbFlag;						// 0143
		u8 newLicenseeCode[2];			// 0144–0145
		u8 sgbFlag;						// 0146
		u8 cartridgeType;				// 0147
		u8 romSize;						// 0148
		u8 ramSize;						// 0149
		u8 destinationCode;				// 014A
		u8 oldLicenseeCode;				// 014B
		u8 maskROMNumber;				// 014C
		u8 headerChecksum;				// 014D
		u8 globalChecksum[2];			// 014E-014F
	};

	constexpr inline const char* get_cartridge_new_licensee_code(const s_CartridgeHeader& header)
	{
		// TODO: test

		switch (header.newLicenseeCode[1])
		{
			case 0x0:  return "None";
			case 0x1:  return "Nintendo Research & Development 1";
			case 0x8:  return "Capcom";
			case 0x13: return "EA (Electronic Arts)";
			case 0x18: return "Hudson Soft";
			case 0x19: return "B-AI";
			case 0x20: return "KSS";
			case 0x22: return "Planning Office WADA";
			case 0x24: return "PCM Complete";
			case 0x25: return "San-X";
			case 0x28: return "Kemco";
			case 0x29: return "SETA Corporation";
			case 0x30: return "Viacom";
			case 0x31: return "Nintendo";
			case 0x32: return "Bandai";
			case 0x33: return "Ocean Software/Acclaim Entertainment";
			case 0x34: return "Konami";
			case 0x35: return "HectorSoft";
			case 0x37: return "Taito";
			case 0x38: return "Hudson Soft";
			case 0x39: return "Banpresto";
			case 0x41: return "Ubi Soft1";
			case 0x42: return "Atlus";
			case 0x44: return "Malibu Interactive";
			case 0x46: return "Angel";
			case 0x47: return "Bullet-Proof Software2";
			case 0x49: return "Irem";
			case 0x50: return "Absolute";
			case 0x51: return "Acclaim Entertainment";
			case 0x52: return "Activision";
			case 0x53: return "Sammy USA Corporation";
			case 0x54: return "Konami";
			case 0x55: return "Hi Tech Expressions";
			case 0x56: return "LJN";
			case 0x57: return "Matchbox";
			case 0x58: return "Mattel";
			case 0x59: return "Milton Bradley Company";
			case 0x60: return "Titus Interactive";
			case 0x61: return "Virgin Games Ltd.3";
			case 0x64: return "Lucasfilm Games4";
			case 0x67: return "Ocean Software";
			case 0x69: return "EA (Electronic Arts)";
			case 0x70: return "Infogrames5";
			case 0x71: return "Interplay Entertainment";
			case 0x72: return "Broderbund";
			case 0x73: return "Sculptured Software6";
			case 0x75: return "The Sales Curve Limited7";
			case 0x78: return "THQ";
			case 0x79: return "Accolade";
			case 0x80: return "Misawa Entertainment";
			case 0x83: return "lozc";
			case 0x86: return "Tokuma Shoten";
			case 0x87: return "Tsukuda Original";
			case 0x91: return "Chunsoft Co.8";
			case 0x92: return "Video System";
			case 0x93: return "Ocean Software/Acclaim Entertainment";
			case 0x95: return "Varie";
			case 0x96: return "Yonezawa/s'pal";
			case 0x97: return "Kaneko";
			case 0x99: return "Pack-In-Video";
				// case 0x9H: return "Bottom Up"
			case 0xA4: return "Konami (Yu - Gi - Oh!)";
			case 0xBL: return "MTO";
				// case 0xDK: return "Kodansha";
		}

		return "Unknown";
	}

	constexpr inline const char* get_cartridge_old_licensee_code(const s_CartridgeHeader& header)
	{
		switch (header.oldLicenseeCode)
		{
			case 0x00: return "None";
			case 0x01: return "Nintendo";
			case 0x08: return "Capcom";
			case 0x09: return "HOT-B";
			case 0x0A: return "Jaleco";
			case 0x0B: return "Coconuts Japan";
			case 0x0C: return "Elite Systems";
			case 0x13: return "EA (Electronic Arts)";
			case 0x18: return "Hudson Soft";
			case 0x19: return "ITC Entertainment";
			case 0x1A: return "Yanoman";
			case 0x1D: return "Japan Clary";
			case 0x1F: return "Virgin Games Ltd.3";
			case 0x24: return "PCM Complete";
			case 0x25: return "San-X";
			case 0x28: return "Kemco";
			case 0x29: return "SETA Corporation";
			case 0x30: return "Infogrames5";
			case 0x31: return "Nintendo";
			case 0x32: return "Bandai";
				// case 0x33: "Indicates that the New licensee code should be used instead.";
			case 0x34: return "Konami";
			case 0x35: return "HectorSoft";
			case 0x38: return "Capcom";
			case 0x39: return "Banpresto";
			case 0x3C: return "Entertainment Interactive(stub)";
			case 0x3E: return "Gremlin";
			case 0x41: return "Ubi Soft1";
			case 0x42: return "Atlus";
			case 0x44: return "Malibu Interactive";
			case 0x46: return "Angel";
			case 0x47: return "Spectrum HoloByte";
			case 0x49: return "Irem";
			case 0x4A: return "Virgin Games Ltd.3";
			case 0x4D: return "Malibu Interactive";
			case 0x4F: return "U.S.Gold";
			case 0x50: return "Absolute";
			case 0x51: return "Acclaim Entertainment";
			case 0x52: return "Activision";
			case 0x53: return "Sammy USA Corporation";
			case 0x54: return "GameTek";
			case 0x55: return "Park Place13";
			case 0x56: return "LJN";
			case 0x57: return "Matchbox";
			case 0x59: return "Milton Bradley Company";
			case 0x5A: return "Mindscape";
			case 0x5B: return "Romstar";
			case 0x5C: return "Naxat Soft14";
			case 0x5D: return "Tradewest";
			case 0x60: return "Titus Interactive";
			case 0x61: return "Virgin Games Ltd.3";
			case 0x67: return "Ocean Software";
			case 0x69: return "EA (Electronic Arts)";
			case 0x6E: return "Elite Systems";
			case 0x6F: return "Electro Brain";
			case 0x70: return "Infogrames5";
			case 0x71: return "Interplay Entertainment";
			case 0x72: return "Broderbund";
			case 0x73: return "Sculptured Software6";
			case 0x75: return "The Sales Curve Limited7";
			case 0x78: return "THQ";
			case 0x79: return "Accolade15";
			case 0x7A: return "Triffix Entertainment";
			case 0x7C: return "MicroProse";
			case 0x7F: return "Kemco";
			case 0x80: return "Misawa Entertainment";
			case 0x83: return "LOZC G.";
			case 0x86: return "Tokuma Shoten";
			case 0x8B: return "Bullet-Proof Software";
			case 0x8C: return "Vic Tokai Corp.16";
			case 0x8E: return "Ape Inc.17";
			case 0x8F: return "I'Max18";
			case 0x91: return "Chunsoft Co.8";
			case 0x92: return "Video System";
			case 0x93: return "Tsubaraya Productions";
			case 0x95: return "Varie";
			case 0x96: return "Yonezawa19/S'Pal";
			case 0x97: return "Kemco";
			case 0x99: return "Arc";
			case 0x9A: return "Nihon Bussan";
			case 0x9B: return "Tecmo";
			case 0x9C: return "Imagineer";
			case 0x9D: return "Banpresto";
			case 0x9F: return "Nova";
			case 0xA1: return "Hori Electric";
			case 0xA2: return "Bandai";
			case 0xA4: return "Konami";
			case 0xA6: return "Kawada";
			case 0xA7: return "Takara";
			case 0xA9: return "Technos Japan";
			case 0xAA: return "Broderbund";
			case 0xAC: return "Toei Animation";
			case 0xAD: return "Toho";
			case 0xAF: return "Namco";
			case 0xB0: return "Acclaim Entertainment";
			case 0xB1: return "ASCII Corporation or Nexsoft";
			case 0xB2: return "Bandai";
			case 0xB4: return "Square Enix";
			case 0xB6: return "HAL Laboratory";
			case 0xB7: return "SNK";
			case 0xB9: return "Pony Canyon";
			case 0xBA: return "Culture Brain";
			case 0xBB: return "Sunsoft";
			case 0xBD: return "Sony Imagesoft";
			case 0xBF: return "Sammy Corporation";
			case 0xC0: return "Taito";
			case 0xC2: return "Kemco";
			case 0xC3: return "Square";
			case 0xC4: return "Tokuma Shoten";
			case 0xC5: return "Data East";
			case 0xC6: return "Tonkin House";
			case 0xC8: return "Koei";
			case 0xC9: return "UFL";
			case 0xCA: return "Ultra Games";
			case 0xCB: return "VAP, Inc.";
			case 0xCC: return "Use Corporation";
			case 0xCD: return "Meldac";
			case 0xCE: return "Pony Canyon";
			case 0xCF: return "Angel";
			case 0xD0: return "Taito";
			case 0xD1: return "SOFEL(Software Engineering Lab)";
			case 0xD2: return "Quest";
			case 0xD3: return "Sigma Enterprises";
			case 0xD4: return "ASK Kodansha Co.";
			case 0xD6: return "Naxat Soft14";
			case 0xD7: return "Copya System";
			case 0xD9: return "Banpresto";
			case 0xDA: return "Tomy";
			case 0xDB: return "LJN";
			case 0xDD: return "Nippon Computer Systems";
			case 0xDE: return "Human Ent.";
			case 0xDF: return "Altron";
			case 0xE0: return "Jaleco";
			case 0xE1: return "Towa Chiki";
			case 0xE2: return "Yutaka";	// Needs more info.
			case 0xE3: return "Varie";
			case 0xE5: return "Epoch";
			case 0xE7: return "Athena";
			case 0xE8: return "Asmik Ace Entertainment";
			case 0xE9: return "Natsume";
			case 0xEA: return "King Records";
			case 0xEB: return "Atlus";
			case 0xEC: return "Epic/Sony Records";
			case 0xEE: return "IGS";
			case 0xF0: return "A Wave";
			case 0xF3: return "Extreme Entertainment";
			case 0xFF: return "LJN";
		}

		return "Unknown";
	}

	constexpr inline const char* get_cartridge_licensee_code(const s_CartridgeHeader& header)
	{
		if (header.oldLicenseeCode == 0x33)
			return get_cartridge_new_licensee_code(header);

		return get_cartridge_old_licensee_code(header);
	}

	constexpr inline e_MBCType get_cartridge_type(const s_CartridgeHeader& header)
	{
		switch (header.cartridgeType)
		{
			case 0x00: return e_MBCType::NO_MBC; // "ROM ONLY";

			case 0x01: return e_MBCType::MBC1; // "MBC1";
			case 0x02: return e_MBCType::MBC1; // "MBC1+RAM";
			case 0x03: return e_MBCType::MBC1; // "MBC1+RAM+BATTERY";

			case 0x05: return e_MBCType::MBC2; // "MBC2";
			case 0x06: return e_MBCType::MBC2; // "MBC2+BATTERY";

			case 0x08: return e_MBCType::VARIOUS; // "ROM+RAM9";
			case 0x09: return e_MBCType::VARIOUS; // "ROM+RAM+BATTERY9";
			case 0x0B: return e_MBCType::VARIOUS; // "MMM01";
			case 0x0C: return e_MBCType::VARIOUS; // "MMM01+RAM";
			case 0x0D: return e_MBCType::VARIOUS; // "MMM01+ RAM+BATTERY";

			case 0x0F: return e_MBCType::MBC3; // "MBC3+TIMER+BATTERY";
			case 0x10: return e_MBCType::MBC3; // "MBC3+TIMER+RAM+BATTERY 10";
			case 0x11: return e_MBCType::MBC3; // "MBC3";
			case 0x12: return e_MBCType::MBC3; // "MBC3+RAM10";
			case 0x13: return e_MBCType::MBC3; // "MBC3+RAM+BATTERY 10";

			case 0x19: return e_MBCType::MBC5; // "MBC5";
			case 0x1A: return e_MBCType::MBC5; // "MBC5+RAM";
			case 0x1B: return e_MBCType::MBC5; // "MBC5+RAM+BATTERY";
			case 0x1C: return e_MBCType::MBC5; // "MBC5+RUMBLE";
			case 0x1D: return e_MBCType::MBC5; // "MBC5+RUMBLE+RAM";
			case 0x1E: return e_MBCType::MBC5; // "MBC5+RUMBLE+RAM+BATTERY";

			case 0x20: return e_MBCType::MBC6; // "MBC6";

			case 0x22: return e_MBCType::MBC7; // "MBC7+SENSOR+RUMBLE+RAM+BATTERY";

			case 0xFC: return e_MBCType::VARIOUS; // "POCKET_CAMERA";
			case 0xFD: return e_MBCType::VARIOUS; // "BANDAI_TAMA5";
			case 0xFE: return e_MBCType::VARIOUS; // "HuC3";
			case 0xFF: return e_MBCType::VARIOUS; // "HuC1+RAM+BATTERY";
		}

		return e_MBCType::INVALID;
	}

	constexpr inline const char* get_cartridge_type_name(const s_CartridgeHeader& header)
	{
		switch (header.cartridgeType)
		{
			case 0x00: return "ROM ONLY";
			case 0x01: return "MBC1";
			case 0x02: return "MBC1+RAM";
			case 0x03: return "MBC1+RAM+BATTERY";
			case 0x05: return "MBC2";
			case 0x06: return "MBC2+BATTERY";
			case 0x08: return "ROM+RAM9";
			case 0x09: return "ROM+RAM+BATTERY9";
			case 0x0B: return "MMM01";
			case 0x0C: return "MMM01+RAM";
			case 0x0D: return "MMM01+ RAM+BATTERY";
			case 0x0F: return "MBC3+TIMER+BATTERY";
			case 0x10: return "MBC3+TIMER+RAM+BATTERY 10";
			case 0x11: return "MBC3";
			case 0x12: return "MBC3+RAM10";
			case 0x13: return "MBC3+RAM+BATTERY 10";
			case 0x19: return "MBC5";
			case 0x1A: return "MBC5+RAM";
			case 0x1B: return "MBC5+RAM+BATTERY";
			case 0x1C: return "MBC5+RUMBLE";
			case 0x1D: return "MBC5+RUMBLE+RAM";
			case 0x1E: return "MBC5+RUMBLE+RAM+BATTERY";
			case 0x20: return "MBC6";
			case 0x22: return "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
			case 0xFC: return "POCKET_CAMERA";
			case 0xFD: return "BANDAI_TAMA5";
			case 0xFE: return "HuC3";
			case 0xFF: return "HuC1+RAM+BATTERY";
		}

		return "Unknown";
	}

	inline std::string get_cartridge_name(const s_CartridgeHeader& header)
	{
		std::string title;

		for (u8 character : header.title)
			title += static_cast<char>(character);

		return title;
	}

	constexpr inline bool is_cartridge_logo_valid(const s_CartridgeHeader& header)
	{
		for (u32 i = 0; i < NintendoLogoSize; ++i)
		{
			if (header.nintendoLogo[i] != NintendoLogo[i])
				return false;
		}

		return true;
	}

	constexpr inline u32 get_rom_size(const s_CartridgeHeader& header)
	{
		if (header.romSize >= 0x52 && header.romSize <= 0x54)
			DAT_LOG_WARN("Header ROM size likely inaccurate.");

		return KB(32) * (1 << header.romSize);
	}

	constexpr inline u32 get_ram_size(const s_CartridgeHeader& header)
	{
		switch (header.ramSize)
		{
			case 0x0: 
				return 0;

			case 0x1:
				DAT_LOG_WARN("Header RAM size likely inaccurate.");
				return 0;

			case 0x2: return KB(8);			// 1 bank
			case 0x3: return KB(32);		// 4 banks of 8 KiB each
			case 0x4: return KB(128);		// 16 banks of 8 KiB each
			case 0x5: return KB(64);		// 8 banks of 8 KiB each

			default:
				DAT_LOG_WARN("Header RAM size likely inaccurate.");
				return 0;
		}
	}
}