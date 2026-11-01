#include "survey/ScriptedTerminal.hpp"
#include <stdexcept>
#include <utility>

namespace survey
{
namespace
{
void AssertHasKeys(const bool isExhausted)
{
	if (isExhausted)
	{
		throw std::runtime_error("Сценарий нажатий закончился раньше, чем опрос");
	}
}
} // namespace

ScriptedTerminal::ScriptedTerminal(std::vector<Key> keys)
	: m_keys(std::move(keys))
	, m_position(0)
{
}

Key ScriptedTerminal::ReadKey()
{
	AssertHasKeys(IsExhausted());
	return m_keys[m_position++];
}

void ScriptedTerminal::Write(const std::string_view text)
{
	m_output += text;
}

const std::string& ScriptedTerminal::Output() const noexcept
{
	return m_output;
}

bool ScriptedTerminal::IsExhausted() const noexcept
{
	return m_position >= m_keys.size();
}
} // namespace survey
