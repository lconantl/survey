#include "survey/InputBuilder.hpp"
#include <utility>

namespace survey
{
InputBuilder::InputBuilder(detail::PromptContext context, std::string title)
	: m_context(context)
	, m_request{.title = std::move(title)}
{
}

InputBuilder& InputBuilder::Default(std::string value)
{
	m_request.defaultValue = std::move(value);
	return *this;
}

InputBuilder& InputBuilder::Validate(Validator predicate, std::string message)
{
	detail::AssertIsValidatorSet(predicate);
	m_request.rules.push_back({std::move(predicate), std::move(message)});

	return *this;
}

std::string InputBuilder::Ask() const
{
	return detail::AskText(m_context, m_request);
}
} // namespace survey
