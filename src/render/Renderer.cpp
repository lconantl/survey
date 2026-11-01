#include "Renderer.hpp"
#include "color/ColorTable.hpp"
#include <format>

namespace survey::detail
{
namespace
{
constexpr auto ANSWER_PREFIX = "- ";
constexpr auto ERASE_BELOW = "\x1b[J";

std::string BuildEraseSequence(const std::size_t frameHeight)
{
	std::string sequence = "\r";
	if (frameHeight > 1)
	{
		sequence += std::format("\x1b[{}A", frameHeight - 1);
	}

	return sequence + ERASE_BELOW;
}

std::string JoinLines(const std::vector<std::string>& lines)
{
	std::string text;
	for (std::size_t index = 0; index < lines.size(); ++index)
	{
		if (index > 0)
		{
			text += '\n';
		}
		text += lines[index];
	}

	return text;
}
} // namespace

Renderer::Renderer(ITerminal& terminal, const Theme& theme) noexcept
	: m_terminal(terminal)
	, m_theme(theme)
	, m_frameHeight(0)
{
}

void Renderer::WriteTitle(const std::string& title)
{
	m_terminal.Write(Colorize(title, m_theme.question) + "\n");
}

void Renderer::Draw(const std::vector<std::string>& lines)
{
	Clear();
	m_terminal.Write(JoinLines(lines));
	m_frameHeight = lines.size();
}

void Renderer::Collapse(const std::string& answer)
{
	Clear();
	m_terminal.Write(Colorize(ANSWER_PREFIX + answer, m_theme.answer) + "\n");
}

void Renderer::Clear()
{
	if (m_frameHeight == 0)
	{
		return;
	}

	m_terminal.Write(BuildEraseSequence(m_frameHeight));
	m_frameHeight = 0;
}
} // namespace survey::detail
