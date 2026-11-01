#include "RawInputGuard.hpp"
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace survey::detail
{
namespace
{
void AssertIsRawModeSet(const bool success)
{
	if (!success)
	{
		throw std::runtime_error("Не удалось перевести консоль в режим посимвольного ввода");
	}
}

#ifdef _WIN32
constexpr DWORD COOKED_INPUT_FLAGS = ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT;

DWORD ReadInputMode(HANDLE handle)
{
	DWORD mode = 0;
	AssertIsRawModeSet(GetConsoleMode(handle, &mode) != 0);

	return mode;
}
#else
termios ReadSettings()
{
	termios settings{};
	AssertIsRawModeSet(tcgetattr(STDIN_FILENO, &settings) == 0);

	return settings;
}

termios MakeRaw(termios settings) noexcept
{
	settings.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO | ISIG));
	settings.c_iflag &= static_cast<tcflag_t>(~(IXON | ICRNL));
	settings.c_cc[VMIN] = 1;
	settings.c_cc[VTIME] = 0;

	return settings;
}
#endif
} // namespace

#ifdef _WIN32

RawInputGuard::RawInputGuard()
	: m_handle(GetStdHandle(STD_INPUT_HANDLE))
	, m_previousMode(ReadInputMode(m_handle))
{
	AssertIsRawModeSet(SetConsoleMode(m_handle, m_previousMode & ~COOKED_INPUT_FLAGS) != 0);
}

RawInputGuard::~RawInputGuard() noexcept
{
	SetConsoleMode(m_handle, m_previousMode);
}

#else

RawInputGuard::RawInputGuard()
	: m_previousSettings(ReadSettings())
{
	const termios raw = MakeRaw(m_previousSettings);
	AssertIsRawModeSet(tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0);
}

RawInputGuard::~RawInputGuard() noexcept
{
	tcsetattr(STDIN_FILENO, TCSANOW, &m_previousSettings);
}

#endif
} // namespace survey::detail
