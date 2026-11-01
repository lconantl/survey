#include "HiddenCursor.hpp"
#include <exception>

namespace survey::detail
{
namespace
{
constexpr auto HIDE_CURSOR = "\x1b[?25l";
constexpr auto SHOW_CURSOR = "\x1b[?25h";
} // namespace

HiddenCursor::HiddenCursor(ITerminal& terminal)
	: m_terminal(terminal)
{
	m_terminal.Write(HIDE_CURSOR);
}

HiddenCursor::~HiddenCursor() noexcept
{
	try
	{
		m_terminal.Write(SHOW_CURSOR);
	}
	catch (const std::exception&)
	{
	}
}
} // namespace survey::detail
