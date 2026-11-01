#include "survey/PasswordBuilder.hpp"
#include <stdexcept>
#include <utility>

namespace survey
{
namespace
{
constexpr char FIRST_PRINTABLE = '!';
constexpr char LAST_PRINTABLE = '~';

void AssertIsMaskPrintable(const char symbol)
{
	if (symbol < FIRST_PRINTABLE || symbol > LAST_PRINTABLE)
	{
		throw std::invalid_argument("Символ маски пароля должен быть видимым ASCII-символом");
	}
}
} // namespace

PasswordBuilder::PasswordBuilder(detail::PromptContext context, std::string title)
	: m_context(context)
	, m_request{.title = std::move(title), .isSecret = true}
{
}

PasswordBuilder& PasswordBuilder::Mask(const char symbol)
{
	AssertIsMaskPrintable(symbol);
	m_request.mask = symbol;

	return *this;
}

PasswordBuilder& PasswordBuilder::Validate(Validator predicate, std::string message)
{
	detail::AssertIsValidatorSet(predicate);
	m_request.rules.push_back({std::move(predicate), std::move(message)});

	return *this;
}

SecretString PasswordBuilder::Ask() const
{
	return SecretString(detail::AskText(m_context, m_request));
}
} // namespace survey
