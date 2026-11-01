#include "choice/ChoiceList.hpp"
#include "color/ColorTable.hpp"
#include "prompt/KeyMatch.hpp"
#include "render/HiddenCursor.hpp"
#include "render/Renderer.hpp"
#include "survey/detail/ChoiceRequest.hpp"
#include <format>
#include <optional>
#include <stdexcept>

namespace survey::detail
{
namespace
{
constexpr auto EMPTY_ANSWER = "—";
constexpr auto ANSWER_SEPARATOR = ", ";

struct Confirmation
{
	std::optional<std::vector<std::size_t>> picked;
	std::string error;
};

void AssertIsRangeValid(const ChoiceRequest& request)
{
	if (request.maxCount.has_value() && request.minCount > *request.maxCount)
	{
		throw std::logic_error("Min() больше, чем Max()");
	}
}

void AssertIsMinReachable(const ChoiceRequest& request)
{
	if (request.minCount > request.labels.size())
	{
		throw std::logic_error("Min() больше, чем количество вариантов ответа");
	}
}

void AssertIsRequestValid(const ChoiceRequest& request)
{
	AssertHasOptions(request.labels.size());
	AssertIsRangeValid(request);
	AssertIsMinReachable(request);
}

Color ChooseLineColor(const ChoiceList& list, const std::size_t index, const Theme& theme)
{
	if (index == list.Cursor())
	{
		return theme.cursor;
	}

	return list.IsMarked(index) ? theme.selected : Color::Default;
}

std::string BuildOptionLine(const ChoiceList& list,
	const std::vector<std::string>& labels,
	const std::size_t index,
	const Theme& theme)
{
	const std::string text = std::format("{}. {}", index + 1, labels[index]);
	return Colorize(text, ChooseLineColor(list, index, theme));
}

std::vector<std::string> BuildFrame(const ChoiceList& list,
	const ChoiceRequest& request,
	const std::string& error,
	const Theme& theme)
{
	std::vector<std::string> lines;
	for (std::size_t index = 0; index < list.Count(); ++index)
	{
		lines.push_back(BuildOptionLine(list, request.labels, index, theme));
	}

	if (!error.empty())
	{
		lines.push_back(Colorize(error, theme.error));
	}

	return lines;
}

std::string JoinAnswer(
	const std::vector<std::string>& labels, const std::vector<std::size_t>& indexes)
{
	if (indexes.empty())
	{
		return EMPTY_ANSWER;
	}

	std::string answer;
	for (const std::size_t index : indexes)
	{
		if (!answer.empty())
		{
			answer += ANSWER_SEPARATOR;
		}
		answer += labels[index];
	}

	return answer;
}

std::string FindCountViolation(const ChoiceRequest& request, const std::size_t count)
{
	if (count < request.minCount)
	{
		return std::format("Выберите вариантов не меньше: {}", request.minCount);
	}

	if (request.maxCount.has_value() && count > *request.maxCount)
	{
		return std::format("Можно выбрать не больше: {}", *request.maxCount);
	}

	return {};
}

Confirmation TryConfirm(const ChoiceList& list, const ChoiceRequest& request)
{
	std::vector<std::size_t> picked = list.Resolve();
	std::string error = FindCountViolation(request, picked.size());
	if (!error.empty())
	{
		return {std::nullopt, std::move(error)};
	}

	return {std::move(picked), {}};
}

void PickByDigit(ChoiceList& list, const Key& key)
{
	const std::optional<std::size_t> index = ToDigitIndex(key);
	if (index.has_value() && *index < list.Count())
	{
		list.Pick(*index);
	}
}

void ApplyKey(ChoiceList& list, const Key& key)
{
	if (key.code == KeyCode::Up)
	{
		list.MoveUp();
		return;
	}

	if (key.code == KeyCode::Down)
	{
		list.MoveDown();
		return;
	}

	if (IsSpaceKey(key))
	{
		list.Toggle();
		return;
	}

	PickByDigit(list, key);
}
} // namespace

std::vector<std::size_t> AskChoice(const PromptContext& context, const ChoiceRequest& request)
{
	AssertIsRequestValid(request);

	ChoiceList list(request.labels.size(), request.mode, request.defaults);
	Renderer renderer(context.terminal, context.theme);
	const HiddenCursor hiddenCursor(context.terminal);
	std::string error;

	renderer.WriteTitle(request.title);
	while (true)
	{
		renderer.Draw(BuildFrame(list, request, error, context.theme));

		const Key key = context.terminal.ReadKey();
		AssertIsNotCancelled(key, renderer);
		error.clear();

		if (key.code != KeyCode::Enter)
		{
			ApplyKey(list, key);
			continue;
		}

		Confirmation confirmation = TryConfirm(list, request);
		if (confirmation.picked.has_value())
		{
			renderer.Collapse(JoinAnswer(request.labels, *confirmation.picked));
			return std::move(*confirmation.picked);
		}
		error = std::move(confirmation.error);
	}
}
} // namespace survey::detail
