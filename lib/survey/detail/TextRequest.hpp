#pragma once

#include "survey/Validator.hpp"
#include "survey/detail/PromptContext.hpp"
#include <optional>
#include <string>
#include <vector>

namespace survey::detail
{
struct ValidationRule
{
	Validator predicate = {};
	std::string message = {};
};

struct TextRequest
{
	std::string title = {};
	std::optional<std::string> defaultValue = {};
	std::vector<ValidationRule> rules = {};
	bool isSecret = false;
	char mask = '*';
};

std::string AskText(const PromptContext& context, const TextRequest& request);

void AssertIsValidatorSet(const Validator& predicate);
} // namespace survey::detail
