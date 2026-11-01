#pragma once

#include "survey/ITerminal.hpp"
#include "survey/Theme.hpp"

namespace survey::detail
{
struct PromptContext
{
	ITerminal& terminal;
	const Theme& theme;
};
} // namespace survey::detail
