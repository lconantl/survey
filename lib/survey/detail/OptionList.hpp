#pragma once

#include "survey/detail/ChoiceRequest.hpp"
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace survey::detail
{
template <typename T> class OptionList
{
public:
	void Add(std::string label, T value)
	{
		m_labels.push_back(std::move(label));
		m_values.push_back(std::move(value));
	}

	[[nodiscard]] std::size_t LastIndex() const
	{
		AssertHasOptions(m_labels.size());
		return m_labels.size() - 1;
	}

	[[nodiscard]] const std::vector<std::string>& Labels() const noexcept
	{
		return m_labels;
	}

	[[nodiscard]] T ValueAt(const std::size_t index) const
	{
		return m_values.at(index);
	}

private:
	std::vector<std::string> m_labels;
	std::vector<T> m_values;
};
} // namespace survey::detail
