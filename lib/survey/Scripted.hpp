#pragma once

#include "survey/Key.hpp"
#include "survey/ScriptedTerminal.hpp"
#include "survey/Survey.hpp"
#include "survey/Theme.hpp"
#include <string_view>
#include <vector>

namespace survey
{
std::vector<Key> ParseScript(std::string_view script);

Survey Scripted(std::string_view script, Theme theme = {});
} // namespace survey
