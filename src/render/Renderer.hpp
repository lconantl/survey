#pragma once

#include "survey/ITerminal.hpp"
#include "survey/Theme.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace survey::detail
{
class Renderer
{
public:
	Renderer(ITerminal& terminal, const Theme& theme) noexcept;

	void WriteTitle(const std::string& title);
	void Draw(const std::vector<std::string>& lines);
	void Collapse(const std::string& answer);
	void Clear();

private:
	ITerminal& m_terminal;
	const Theme& m_theme;
	std::size_t m_frameHeight;
};
} // namespace survey::detail
