#pragma once

#include "survey/ITerminal.hpp"
#include <optional>

namespace survey
{
class ConsoleTerminal final : public ITerminal
{
public:
	ConsoleTerminal();
	~ConsoleTerminal() noexcept override;

	ConsoleTerminal(const ConsoleTerminal&) = delete;
	ConsoleTerminal& operator=(const ConsoleTerminal&) = delete;
	ConsoleTerminal(ConsoleTerminal&&) = delete;
	ConsoleTerminal& operator=(ConsoleTerminal&&) = delete;

	Key ReadKey() override;
	void Write(std::string_view text) override;

private:
#ifdef _WIN32
	std::optional<Key> ConsumeCharacter(wchar_t symbol);

	unsigned int m_previousOutputCodePage;
	unsigned long m_previousOutputMode;
	wchar_t m_pendingHighSurrogate;
#endif
};
} // namespace survey
