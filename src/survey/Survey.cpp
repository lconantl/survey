#include "survey/Survey.hpp"
#include "survey/ConsoleTerminal.hpp"

namespace survey
{
namespace
{
constexpr auto YES_LABEL = "Да";
constexpr auto NO_LABEL = "Нет";

std::unique_ptr<ITerminal> ResolveTerminal(std::unique_ptr<ITerminal> terminal)
{
	if (terminal != nullptr)
	{
		return terminal;
	}

	return std::make_unique<ConsoleTerminal>();
}

void AddConfirmOption(
	SelectBuilder<bool>& builder, const char* label, const bool value, const bool defaultValue)
{
	builder.Option(label, value);
	if (value == defaultValue)
	{
		builder.Default();
	}
}
} // namespace

Survey::Survey(Theme theme, std::unique_ptr<ITerminal> terminal)
	: m_theme(theme)
	, m_terminal(ResolveTerminal(std::move(terminal)))
{
}

Survey::~Survey() = default;

Survey::Survey(Survey&& other) noexcept = default;

Survey& Survey::operator=(Survey&& other) noexcept = default;

bool Survey::Confirm(std::string title, const bool defaultValue) const
{
	SelectBuilder<bool> builder(Context(), std::move(title));
	AddConfirmOption(builder, YES_LABEL, true, defaultValue);
	AddConfirmOption(builder, NO_LABEL, false, defaultValue);

	return builder.Ask();
}

InputBuilder Survey::Input(std::string title) const
{
	return {Context(), std::move(title)};
}

PasswordBuilder Survey::Password(std::string title) const
{
	return {Context(), std::move(title)};
}

detail::PromptContext Survey::Context() const noexcept
{
	return {*m_terminal, m_theme};
}
} // namespace survey
