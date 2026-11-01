#pragma once

#include "survey/detail/ChoiceRequest.hpp"
#include <cstddef>
#include <vector>

namespace survey::detail
{
class ChoiceList
{
public:
	ChoiceList(std::size_t count, SelectionMode mode, std::vector<std::size_t> defaults);

	void MoveUp() noexcept;
	void MoveDown() noexcept;
	void Toggle();
	void Pick(std::size_t index);

	[[nodiscard]] std::size_t Cursor() const noexcept;
	[[nodiscard]] std::size_t Count() const noexcept;
	[[nodiscard]] bool IsMarked(std::size_t index) const;
	[[nodiscard]] std::vector<std::size_t> Resolve() const;

private:
	void MarkExclusively(std::size_t index);
	void Flip(std::size_t index);

	SelectionMode m_mode;
	std::vector<bool> m_marked;
	std::vector<std::size_t> m_defaults;
	std::size_t m_cursor;
};
} // namespace survey::detail
