#pragma once

#include "survey/Key.hpp"
#include <string_view>

namespace survey
{
class ITerminal
{
public:
	virtual ~ITerminal() = default;

	virtual Key ReadKey() = 0;
	virtual void Write(std::string_view text) = 0;
};
} // namespace survey
