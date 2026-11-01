#pragma once

#include "survey/Color.hpp"
#include <filesystem>
#include <string_view>

namespace survey
{
struct Theme
{
	Color question = Color::BrightWhite;
	Color cursor = Color::Cyan;
	Color selected = Color::Green;
	Color answer = Color::Gray;
	Color hint = Color::Gray;
	Color error = Color::Red;

	static Theme Parse(std::string_view text);
	static Theme LoadFromFile(const std::filesystem::path& path);
};
} // namespace survey
