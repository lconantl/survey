#pragma once

#include "survey/ITerminal.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace survey
{
class ScriptedTerminal final : public ITerminal
{
public:
	explicit ScriptedTerminal(std::vector<Key> keys);

	Key ReadKey() override;
	void Write(std::string_view text) override;

	[[nodiscard]] const std::string& Output() const noexcept;
	[[nodiscard]] bool IsExhausted() const noexcept;

private:
	std::vector<Key> m_keys;
	std::size_t m_position;
	std::string m_output;
};
} // namespace survey
