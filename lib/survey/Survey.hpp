#pragma once

#include "survey/CancelledError.hpp"
#include "survey/ITerminal.hpp"
#include "survey/InputBuilder.hpp"
#include "survey/MultiSelectBuilder.hpp"
#include "survey/PasswordBuilder.hpp"
#include "survey/SecretString.hpp"
#include "survey/SelectBuilder.hpp"
#include "survey/Theme.hpp"
#include "survey/detail/PromptContext.hpp"
#include <memory>
#include <string>
#include <utility>

namespace survey
{
class Survey
{
public:
	explicit Survey(Theme theme = {}, std::unique_ptr<ITerminal> terminal = nullptr);
	~Survey();

	Survey(const Survey&) = delete;
	Survey& operator=(const Survey&) = delete;
	Survey(Survey&& other) noexcept;
	Survey& operator=(Survey&& other) noexcept;

	template <typename T = std::string>
	[[nodiscard]] SelectBuilder<T> Select(std::string title) const
	{
		return SelectBuilder<T>(Context(), std::move(title));
	}

	template <typename T = std::string>
	[[nodiscard]] MultiSelectBuilder<T> MultiSelect(std::string title) const
	{
		return MultiSelectBuilder<T>(Context(), std::move(title));
	}

	[[nodiscard]] bool Confirm(std::string title, bool defaultValue = true) const;
	[[nodiscard]] InputBuilder Input(std::string title) const;
	[[nodiscard]] PasswordBuilder Password(std::string title) const;

private:
	[[nodiscard]] detail::PromptContext Context() const noexcept;

	Theme m_theme;
	std::unique_ptr<ITerminal> m_terminal;
};
} // namespace survey
