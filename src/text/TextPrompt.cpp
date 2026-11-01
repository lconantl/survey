#include "color/ColorTable.hpp"
#include "prompt/KeyMatch.hpp"
#include "render/Renderer.hpp"
#include "survey/detail/TextRequest.hpp"
#include "text/LineEditor.hpp"
#include "text/Utf8.hpp"

namespace survey::detail
{
namespace
{
constexpr std::size_t SECRET_ANSWER_LENGTH = 8;
constexpr auto EMPTY_ANSWER = "—";

bool IsDefaultShown(const LineEditor& editor, const TextRequest& request) noexcept
{
	return editor.IsEmpty() && request.defaultValue.has_value();
}

std::string BuildDisplay(const LineEditor& editor, const TextRequest& request, const Theme& theme)
{
	if (request.isSecret)
	{
		return std::string(CountCodePoints(editor.Text()), request.mask);
	}

	if (IsDefaultShown(editor, request))
	{
		return Colorize(*request.defaultValue, theme.hint);
	}

	return editor.Text();
}

std::vector<std::string> BuildFrame(
	const std::string& error, const std::string& display, const Theme& theme)
{
	if (error.empty())
	{
		return {display};
	}

	return {Colorize(error, theme.error), display};
}

std::string_view SelectCandidate(const LineEditor& editor, const TextRequest& request) noexcept
{
	if (IsDefaultShown(editor, request))
	{
		return *request.defaultValue;
	}

	return editor.Text();
}

std::string FindViolation(const std::string_view value, const std::vector<ValidationRule>& rules)
{
	for (const auto& rule : rules)
	{
		if (!rule.predicate(value))
		{
			return rule.message;
		}
	}

	return {};
}

std::string DescribeAnswer(const std::string& value, const TextRequest& request)
{
	if (request.isSecret)
	{
		return std::string(SECRET_ANSWER_LENGTH, request.mask);
	}

	return value.empty() ? std::string(EMPTY_ANSWER) : value;
}

std::string TakeValue(LineEditor& editor, const TextRequest& request)
{
	if (IsDefaultShown(editor, request))
	{
		return *request.defaultValue;
	}

	return editor.TakeText();
}

void ApplyKey(LineEditor& editor, const Key& key)
{
	if (key.code == KeyCode::Backspace)
	{
		editor.EraseLast();
		return;
	}

	if (key.code == KeyCode::Character)
	{
		editor.Append(key.text);
	}
}
} // namespace

std::string AskText(const PromptContext& context, const TextRequest& request)
{
	Renderer renderer(context.terminal, context.theme);
	LineEditor editor;
	std::string error;

	renderer.WriteTitle(request.title);
	while (true)
	{
		const std::string display = BuildDisplay(editor, request, context.theme);
		renderer.Draw(BuildFrame(error, display, context.theme));

		const Key key = context.terminal.ReadKey();
		AssertIsNotCancelled(key, renderer);
		error.clear();

		if (key.code != KeyCode::Enter)
		{
			ApplyKey(editor, key);
			continue;
		}

		error = FindViolation(SelectCandidate(editor, request), request.rules);
		if (error.empty())
		{
			std::string value = TakeValue(editor, request);
			renderer.Collapse(DescribeAnswer(value, request));
			return value;
		}
	}
}
} // namespace survey::detail
