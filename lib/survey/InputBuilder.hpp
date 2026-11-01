#pragma once

#include "survey/Validator.hpp"
#include "survey/detail/PromptContext.hpp"
#include "survey/detail/TextRequest.hpp"
#include <string>

namespace survey
{
class InputBuilder
{
public:
	InputBuilder(detail::PromptContext context, std::string title);

	InputBuilder& Default(std::string value);
	InputBuilder& Validate(Validator predicate, std::string message);

	[[nodiscard]] std::string Ask() const;

private:
	detail::PromptContext m_context;
	detail::TextRequest m_request;
};
} // namespace survey
