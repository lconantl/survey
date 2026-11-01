#pragma once

#include "survey/SecretString.hpp"
#include "survey/Validator.hpp"
#include "survey/detail/PromptContext.hpp"
#include "survey/detail/TextRequest.hpp"
#include <string>

namespace survey
{
class PasswordBuilder
{
public:
	PasswordBuilder(detail::PromptContext context, std::string title);

	PasswordBuilder& Mask(char symbol);
	PasswordBuilder& Validate(Validator predicate, std::string message);

	[[nodiscard]] SecretString Ask() const;

private:
	detail::PromptContext m_context;
	detail::TextRequest m_request;
};
} // namespace survey
