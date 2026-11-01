#pragma once

#include "survey/detail/ChoiceRequest.hpp"
#include "survey/detail/OptionList.hpp"
#include "survey/detail/PromptContext.hpp"
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace survey
{
template <typename T = std::string> class MultiSelectBuilder
{
public:
	MultiSelectBuilder(detail::PromptContext context, std::string title)
		: m_context(context)
		, m_title(std::move(title))
	{
	}

	MultiSelectBuilder& Option(std::string label, T value)
	{
		m_options.Add(std::move(label), std::move(value));
		return *this;
	}

	MultiSelectBuilder& Option(std::string label)
		requires std::same_as<T, std::string>
	{
		T value = label;
		return Option(std::move(label), std::move(value));
	}

	MultiSelectBuilder& Default()
	{
		const std::size_t index = m_options.LastIndex();
		if (std::find(m_defaults.begin(), m_defaults.end(), index) == m_defaults.end())
		{
			m_defaults.push_back(index);
		}

		return *this;
	}

	MultiSelectBuilder& Min(const std::size_t count) noexcept
	{
		m_minCount = count;
		return *this;
	}

	MultiSelectBuilder& Max(const std::size_t count) noexcept
	{
		m_maxCount = count;
		return *this;
	}

	[[nodiscard]] std::vector<T> Ask() const
	{
		const detail::ChoiceRequest request{
			.title = m_title,
			.labels = m_options.Labels(),
			.defaults = m_defaults,
			.mode = detail::SelectionMode::Multiple,
			.minCount = m_minCount,
			.maxCount = m_maxCount,
		};

		return CollectValues(detail::AskChoice(m_context, request));
	}

private:
	[[nodiscard]] std::vector<T> CollectValues(const std::vector<std::size_t>& indexes) const
	{
		std::vector<T> values;
		values.reserve(indexes.size());

		for (const std::size_t index : indexes)
		{
			values.push_back(m_options.ValueAt(index));
		}

		return values;
	}

	detail::PromptContext m_context;
	std::string m_title;
	detail::OptionList<T> m_options;
	std::vector<std::size_t> m_defaults;
	std::size_t m_minCount = 0;
	std::optional<std::size_t> m_maxCount;
};
} // namespace survey
