#include "ColorTable.hpp"
#include <array>
#include <format>
#include <stdexcept>

namespace survey::detail
{
namespace
{
constexpr auto RESET_SEQUENCE = "\x1b[0m";

struct ColorEntry
{
	std::string_view name;
	Color color;
	int code;
};

constexpr std::array<ColorEntry, 17> COLOR_TABLE = {{
	{"default", Color::Default, 39},
	{"black", Color::Black, 30},
	{"red", Color::Red, 31},
	{"green", Color::Green, 32},
	{"yellow", Color::Yellow, 33},
	{"blue", Color::Blue, 34},
	{"magenta", Color::Magenta, 35},
	{"cyan", Color::Cyan, 36},
	{"white", Color::White, 37},
	{"gray", Color::Gray, 90},
	{"bright_red", Color::BrightRed, 91},
	{"bright_green", Color::BrightGreen, 92},
	{"bright_yellow", Color::BrightYellow, 93},
	{"bright_blue", Color::BrightBlue, 94},
	{"bright_magenta", Color::BrightMagenta, 95},
	{"bright_cyan", Color::BrightCyan, 96},
	{"bright_white", Color::BrightWhite, 97},
}};

const ColorEntry* FindByName(const std::string_view name) noexcept
{
	for (const auto& entry : COLOR_TABLE)
	{
		if (entry.name == name)
		{
			return &entry;
		}
	}

	return nullptr;
}

int FindCode(const Color color) noexcept
{
	for (const auto& entry : COLOR_TABLE)
	{
		if (entry.color == color)
		{
			return entry.code;
		}
	}

	return COLOR_TABLE.front().code;
}

const ColorEntry& AssertIsKnownColor(const ColorEntry* entry, const std::string_view name)
{
	if (entry == nullptr)
	{
		throw std::invalid_argument("Неизвестный цвет: " + std::string(name));
	}

	return *entry;
}
} // namespace

Color ParseColor(const std::string_view name)
{
	return AssertIsKnownColor(FindByName(name), name).color;
}

std::string Colorize(const std::string_view text, const Color color)
{
	if (color == Color::Default)
	{
		return std::string(text);
	}

	return std::format("\x1b[{}m{}{}", FindCode(color), text, RESET_SEQUENCE);
}
} // namespace survey::detail
