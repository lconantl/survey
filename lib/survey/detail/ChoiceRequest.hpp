#pragma once

#include "survey/detail/PromptContext.hpp"
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace survey::detail
{
enum class SelectionMode
{
	Single,
	Multiple,
};

struct ChoiceRequest
{
	std::string title = {};
	std::vector<std::string> labels = {};
	std::vector<std::size_t> defaults = {};
	SelectionMode mode = SelectionMode::Single;
	std::size_t minCount = 0;
	std::optional<std::size_t> maxCount = {};
};

std::vector<std::size_t> AskChoice(const PromptContext& context, const ChoiceRequest& request);

void AssertHasOptions(std::size_t count);
} // namespace survey::detail
