#pragma once

#include "survey/Color.hpp"
#include <string>
#include <string_view>

namespace survey::detail
{
Color ParseColor(std::string_view name);

std::string Colorize(std::string_view text, Color color);
} // namespace survey::detail
