#include "ChoiceList.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace survey::detail
{
namespace
{
void AssertIsIndexValid(const std::size_t index, const std::size_t count)
{
	if (index >= count)
	{
		throw std::out_of_range("Номер варианта ответа вне списка");
	}
}

std::vector<std::size_t> NormalizeDefaults(
	std::vector<std::size_t> defaults, const std::size_t count, const SelectionMode mode)
{
	for (const std::size_t index : defaults)
	{
		AssertIsIndexValid(index, count);
	}

	std::sort(defaults.begin(), defaults.end());
	defaults.erase(std::unique(defaults.begin(), defaults.end()), defaults.end());

	if (mode == SelectionMode::Single && defaults.empty())
	{
		defaults.push_back(0);
	}

	return defaults;
}

std::size_t AssertIsNotEmpty(const std::size_t count)
{
	AssertHasOptions(count);
	return count;
}

std::size_t ChooseInitialCursor(const std::vector<std::size_t>& defaults) noexcept
{
	return defaults.empty() ? 0 : defaults.front();
}
} // namespace

ChoiceList::ChoiceList(
	const std::size_t count, const SelectionMode mode, std::vector<std::size_t> defaults)
	: m_mode(mode)
	, m_marked(AssertIsNotEmpty(count), false)
	, m_defaults(NormalizeDefaults(std::move(defaults), count, mode))
	, m_cursor(ChooseInitialCursor(m_defaults))
{
}

void ChoiceList::MoveUp() noexcept
{
	m_cursor = (m_cursor + Count() - 1) % Count();
}

void ChoiceList::MoveDown() noexcept
{
	m_cursor = (m_cursor + 1) % Count();
}

void ChoiceList::Toggle()
{
	if (m_mode == SelectionMode::Single && !m_marked[m_cursor])
	{
		MarkExclusively(m_cursor);
		return;
	}

	Flip(m_cursor);
}

void ChoiceList::Pick(const std::size_t index)
{
	AssertIsIndexValid(index, Count());
	m_cursor = index;

	if (m_mode == SelectionMode::Single)
	{
		MarkExclusively(index);
		return;
	}

	Flip(index);
}

std::size_t ChoiceList::Cursor() const noexcept
{
	return m_cursor;
}

std::size_t ChoiceList::Count() const noexcept
{
	return m_marked.size();
}

bool ChoiceList::IsMarked(const std::size_t index) const
{
	AssertIsIndexValid(index, Count());
	return m_marked[index];
}

std::vector<std::size_t> ChoiceList::Resolve() const
{
	std::vector<std::size_t> marked;
	for (std::size_t index = 0; index < Count(); ++index)
	{
		if (m_marked[index])
		{
			marked.push_back(index);
		}
	}

	return marked.empty() ? m_defaults : marked;
}

void ChoiceList::MarkExclusively(const std::size_t index)
{
	std::fill(m_marked.begin(), m_marked.end(), false);
	m_marked[index] = true;
}

void ChoiceList::Flip(const std::size_t index)
{
	m_marked[index] = !m_marked[index];
}
} // namespace survey::detail
