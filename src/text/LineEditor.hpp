#pragma once

#include <string>
#include <string_view>

namespace survey::detail
{
class LineEditor final
{
public:
	LineEditor();
	~LineEditor() noexcept;

	LineEditor(const LineEditor&) = delete;
	LineEditor& operator=(const LineEditor&) = delete;
	LineEditor(LineEditor&&) = delete;
	LineEditor& operator=(LineEditor&&) = delete;

	void Append(std::string_view text);
	void EraseLast() noexcept;

	[[nodiscard]] const std::string& Text() const noexcept;
	[[nodiscard]] bool IsEmpty() const noexcept;
	[[nodiscard]] std::string TakeText();

private:
	std::string m_text;
};
} // namespace survey::detail
