#pragma once

#include <string>

namespace survey
{
enum class KeyCode
{
	Up,
	Down,
	Enter,
	Escape,
	Backspace,
	Interrupt,
	Character,
};

struct Key
{
	KeyCode code = KeyCode::Character;
	std::string text;
};
} // namespace survey
