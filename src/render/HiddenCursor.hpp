#pragma once

#include "survey/ITerminal.hpp"

namespace survey::detail
{
class HiddenCursor final
{
public:
	explicit HiddenCursor(ITerminal& terminal);
	~HiddenCursor() noexcept;

	HiddenCursor(const HiddenCursor&) = delete;
	HiddenCursor& operator=(const HiddenCursor&) = delete;
	HiddenCursor(HiddenCursor&&) = delete;
	HiddenCursor& operator=(HiddenCursor&&) = delete;

private:
	ITerminal& m_terminal;
};
} // namespace survey::detail
