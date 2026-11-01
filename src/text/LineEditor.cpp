#include "LineEditor.hpp"
#include "secret/WipeString.hpp"
#include "text/Utf8.hpp"
#include <utility>

namespace survey::detail
{
namespace
{
constexpr std::size_t INITIAL_CAPACITY = 256;
} // namespace

LineEditor::LineEditor()
{
	m_text.reserve(INITIAL_CAPACITY);
}

LineEditor::~LineEditor() noexcept
{
	WipeString(m_text);
}

void LineEditor::Append(const std::string_view text)
{
	m_text += text;
}

void LineEditor::EraseLast() noexcept
{
	RemoveLastCodePoint(m_text);
}

const std::string& LineEditor::Text() const noexcept
{
	return m_text;
}

bool LineEditor::IsEmpty() const noexcept
{
	return m_text.empty();
}

std::string LineEditor::TakeText()
{
	std::string text = m_text;
	WipeString(m_text);

	return text;
}
} // namespace survey::detail
