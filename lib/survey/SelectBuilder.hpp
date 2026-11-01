#pragma once

#include "survey/detail/ChoiceRequest.hpp"
#include "survey/detail/OptionList.hpp"
#include "survey/detail/PromptContext.hpp"
#include <concepts>
#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace survey
{
template <typename T = std::string> class SelectBuilder
{
public:
	SelectBuilder(detail::PromptContext context, std::string title)
		: m_context(context)
		, m_title(std::move(title))
	{
	}

	SelectBuilder& Option(std::string label, T value)
	{
		m_options.Add(std::move(label), std::move(value));
		return *this;
	}

	SelectBuilder& Option(std::string label)
		requires std::same_as<T, std::string>
	{
		T value = label;
		return Option(std::move(label), std::move(value));
	}

	SelectBuilder& Default()
	{
		m_defaultIndex = m_options.LastIndex();
		return *this;
	}

	[[nodiscard]] T Ask() const
	{
		const detail::ChoiceRequest request{
			.title = m_title,
			.labels = m_options.Labels(),
			.defaults = BuildDefaults(),
			.mode = detail::SelectionMode::Single,
		};

		const std::vector<std::size_t> picked = detail::AskChoice(m_context, request);
		return m_options.ValueAt(picked.front());
	}

private:
	[[nodiscard]] std::vector<std::size_t> BuildDefaults() const
	{
		if (!m_defaultIndex.has_value())
		{
			return {};
		}

		return {*m_defaultIndex};
	}

	detail::PromptContext m_context;
	std::string m_title;
	detail::OptionList<T> m_options;
	std::optional<std::size_t> m_defaultIndex;
};
} // namespace survey
