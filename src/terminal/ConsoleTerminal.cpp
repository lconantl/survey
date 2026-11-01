#include "survey/ConsoleTerminal.hpp"
#include "terminal/RawInputGuard.hpp"
#include "text/Utf8.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <cerrno>
#include <poll.h>
#include <unistd.h>
#endif

namespace survey
{
namespace
{
constexpr auto SHOW_CURSOR = "\x1b[?25h";

void AssertIsInteractive(const bool isInteractive)
{
	if (!isInteractive)
	{
		throw std::runtime_error(
			"Опросу нужна интерактивная консоль: ввод или вывод перенаправлен");
	}
}

void WriteToConsole(const std::string_view text)
{
	std::cout.write(text.data(), static_cast<std::streamsize>(text.size()));
	std::cout.flush();
}

#ifdef _WIN32
constexpr wchar_t CTRL_C_SYMBOL = 3;
constexpr wchar_t FIRST_PRINTABLE = 32;

HANDLE InputHandle() noexcept
{
	return GetStdHandle(STD_INPUT_HANDLE);
}

HANDLE OutputHandle() noexcept
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}

bool IsConsoleHandle(HANDLE handle) noexcept
{
	DWORD mode = 0;
	return handle != INVALID_HANDLE_VALUE && GetConsoleMode(handle, &mode) != 0;
}

DWORD ReadOutputMode() noexcept
{
	DWORD mode = 0;
	GetConsoleMode(OutputHandle(), &mode);

	return mode;
}

void AssertIsRecordRead(const bool success)
{
	if (!success)
	{
		throw std::runtime_error("Не удалось прочитать нажатие клавиши из консоли");
	}
}

bool IsKeyPress(const INPUT_RECORD& record) noexcept
{
	return record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown != 0;
}

KEY_EVENT_RECORD ReadKeyEvent()
{
	INPUT_RECORD record{};
	DWORD readCount = 0;

	do
	{
		AssertIsRecordRead(ReadConsoleInputW(InputHandle(), &record, 1, &readCount) != 0);
	} while (readCount == 0 || !IsKeyPress(record));

	return record.Event.KeyEvent;
}

std::optional<Key> MapVirtualKey(const WORD virtualKey) noexcept
{
	switch (virtualKey)
	{
	case VK_UP: return Key{KeyCode::Up, {}};
	case VK_DOWN: return Key{KeyCode::Down, {}};
	case VK_RETURN: return Key{KeyCode::Enter, {}};
	case VK_ESCAPE: return Key{KeyCode::Escape, {}};
	case VK_BACK: return Key{KeyCode::Backspace, {}};
	default: return std::nullopt;
	}
}

bool IsHighSurrogate(const wchar_t symbol) noexcept
{
	return symbol >= 0xD800 && symbol <= 0xDBFF;
}

bool IsLowSurrogate(const wchar_t symbol) noexcept
{
	return symbol >= 0xDC00 && symbol <= 0xDFFF;
}

char32_t CombineSurrogates(const wchar_t high, const wchar_t low) noexcept
{
	return 0x10000 + ((static_cast<char32_t>(high) - 0xD800) << 10)
		+ (static_cast<char32_t>(low) - 0xDC00);
}
#else
constexpr unsigned char ESCAPE_BYTE = 27;
constexpr unsigned char CTRL_C_BYTE = 3;
constexpr unsigned char DELETE_BYTE = 127;
constexpr unsigned char BACKSPACE_BYTE = 8;
constexpr unsigned char FIRST_PRINTABLE = 32;
constexpr unsigned char FIRST_FINAL_BYTE = 0x40;
constexpr unsigned char LAST_FINAL_BYTE = 0x7E;
constexpr int ESCAPE_TIMEOUT_MS = 30;

void AssertIsByteRead(const ssize_t result)
{
	if (result != 1)
	{
		throw std::runtime_error("Не удалось прочитать нажатие клавиши из консоли");
	}
}

unsigned char ReadByte()
{
	unsigned char byte = 0;
	ssize_t result = 0;

	do
	{
		result = read(STDIN_FILENO, &byte, 1);
	} while (result < 0 && errno == EINTR);

	AssertIsByteRead(result);
	return byte;
}

bool HasPendingInput(const int timeoutMs) noexcept
{
	pollfd descriptor{STDIN_FILENO, POLLIN, 0};
	return poll(&descriptor, 1, timeoutMs) > 0;
}

bool IsFinalByte(const unsigned char byte) noexcept
{
	return byte >= FIRST_FINAL_BYTE && byte <= LAST_FINAL_BYTE;
}

unsigned char ReadFinalByte()
{
	unsigned char byte = ReadByte();
	while (!IsFinalByte(byte))
	{
		byte = ReadByte();
	}

	return byte;
}

std::optional<Key> MapFinalByte(const unsigned char byte) noexcept
{
	switch (byte)
	{
	case 'A': return Key{KeyCode::Up, {}};
	case 'B': return Key{KeyCode::Down, {}};
	default: return std::nullopt;
	}
}

std::optional<Key> ReadEscapeSequence()
{
	if (!HasPendingInput(ESCAPE_TIMEOUT_MS))
	{
		return Key{KeyCode::Escape, {}};
	}

	const unsigned char introducer = ReadByte();
	if (introducer != '[' && introducer != 'O')
	{
		return std::nullopt;
	}

	return MapFinalByte(ReadFinalByte());
}

std::optional<Key> MapByte(const unsigned char byte)
{
	switch (byte)
	{
	case ESCAPE_BYTE: return ReadEscapeSequence();
	case '\r':
	case '\n': return Key{KeyCode::Enter, {}};
	case DELETE_BYTE:
	case BACKSPACE_BYTE: return Key{KeyCode::Backspace, {}};
	case CTRL_C_BYTE: return Key{KeyCode::Interrupt, {}};
	default: break;
	}

	if (byte < FIRST_PRINTABLE)
	{
		return std::nullopt;
	}

	return Key{KeyCode::Character, std::string(1, static_cast<char>(byte))};
}
#endif
} // namespace

#ifdef _WIN32

ConsoleTerminal::ConsoleTerminal()
	: m_previousOutputCodePage(GetConsoleOutputCP())
	, m_previousOutputMode(ReadOutputMode())
	, m_pendingHighSurrogate(0)
{
	AssertIsInteractive(IsConsoleHandle(InputHandle()) && IsConsoleHandle(OutputHandle()));

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleMode(OutputHandle(),
		m_previousOutputMode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

ConsoleTerminal::~ConsoleTerminal() noexcept
{
	try
	{
		WriteToConsole(SHOW_CURSOR);
	}
	catch (const std::exception&)
	{
	}

	SetConsoleMode(OutputHandle(), m_previousOutputMode);
	SetConsoleOutputCP(m_previousOutputCodePage);
}

Key ConsoleTerminal::ReadKey()
{
	const detail::RawInputGuard guard;

	while (true)
	{
		const KEY_EVENT_RECORD event = ReadKeyEvent();
		if (std::optional<Key> key = MapVirtualKey(event.wVirtualKeyCode))
		{
			return *key;
		}

		if (std::optional<Key> key = ConsumeCharacter(event.uChar.UnicodeChar))
		{
			return *key;
		}
	}
}

std::optional<Key> ConsoleTerminal::ConsumeCharacter(const wchar_t symbol)
{
	if (symbol == CTRL_C_SYMBOL)
	{
		return Key{KeyCode::Interrupt, {}};
	}

	if (IsHighSurrogate(symbol))
	{
		m_pendingHighSurrogate = symbol;
		return std::nullopt;
	}

	if (IsLowSurrogate(symbol) && m_pendingHighSurrogate != 0)
	{
		const char32_t codePoint = CombineSurrogates(m_pendingHighSurrogate, symbol);
		m_pendingHighSurrogate = 0;
		return Key{KeyCode::Character, detail::EncodeUtf8(codePoint)};
	}

	if (symbol < FIRST_PRINTABLE)
	{
		return std::nullopt;
	}

	return Key{KeyCode::Character, detail::EncodeUtf8(static_cast<char32_t>(symbol))};
}

#else

ConsoleTerminal::ConsoleTerminal()
{
	AssertIsInteractive(isatty(STDIN_FILENO) != 0 && isatty(STDOUT_FILENO) != 0);
}

ConsoleTerminal::~ConsoleTerminal() noexcept
{
	try
	{
		WriteToConsole(SHOW_CURSOR);
	}
	catch (const std::exception&)
	{
	}
}

Key ConsoleTerminal::ReadKey()
{
	const detail::RawInputGuard guard;

	while (true)
	{
		if (std::optional<Key> key = MapByte(ReadByte()))
		{
			return *key;
		}
	}
}

#endif

void ConsoleTerminal::Write(const std::string_view text)
{
	WriteToConsole(text);
}
} // namespace survey
