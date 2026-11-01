#pragma once

#include <functional>
#include <string_view>

namespace survey
{
using Validator = std::function<bool(std::string_view)>;
} // namespace survey
