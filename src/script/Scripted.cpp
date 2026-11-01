#include "survey/Scripted.hpp"
#include "survey/ScriptedTerminal.hpp"
#include "text/Utf8.hpp"
#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace survey
{
namespace
{
constexpr auto WHITESPACE = " \t\r\n";

struct NamedKey
{
	std::string_view name;
	Key key;
};

const std::array<NamedKey, 7>& NamedKeys()
{
	static const std::array<NamedKey, 7> keys = {{
		{"up", {KeyCode::Up, {}}},
		{"down", {KeyCode::Down, {}}},
		{"enter", {KeyCode::Enter, {}}},
		{"esc", {KeyCode::Escape, {}}},
		{"backspace", {KeyCode::Backspace, {}}},
		{"ctrl+c", {KeyCode::Interrupt, {}}},
		{"space", {KeyCode::Character, " "}},
	}};

	return keys;
}

bool IsQuote(const char symbol) noexcept
{
	return symbol == '\'' || symbol == '"';
}

std::size_t AssertIsQuoteClosed(const std::string_view script, const std::size_t start)
{
	const std::size_t end = script.find(script[start], start + 1);
	if (end == std::string_view::npos)
	{
		throw std::invalid_argument("В сценарии не закрыта кавычка");
	}

	return end;
}

Key MapToken(const std::string_view token)
{
	for (const auto& [name, key] : NamedKeys())
	{
		if (name == token)
		{
			return key;
		}
	}

	if (detail::CountCodePoints(token) == 1)
	{
		return {KeyCode::Character, std::string(token)};
	}

	throw std::invalid_argument("Неизвестная клавиша в сценарии: " + std::string(token));
}

void AppendCharacters(std::vector<Key>& keys, const std::string_view text)
{
	for (auto& symbol : detail::SplitCodePoints(text))
	{
		keys.push_back({KeyCode::Character, std::move(symbol)});
	}
}

std::size_t ParseQuoted(
	std::vector<Key>& keys, const std::string_view script, const std::size_t start)
{
	const std::size_t end = AssertIsQuoteClosed(script, start);
	AppendCharacters(keys, script.substr(start + 1, end - start - 1));

	return end + 1;
}

std::size_t ParseWord(
	std::vector<Key>& keys, const std::string_view script, const std::size_t start)
{
	const std::size_t end = std::min(script.find_first_of(WHITESPACE, start), script.size());
	keys.push_back(MapToken(script.substr(start, end - start)));

	return end;
}
} // namespace

std::vector<Key> ParseScript(const std::string_view script)
{
	std::vector<Key> keys;
	std::size_t position = script.find_first_not_of(WHITESPACE);

	while (position != std::string_view::npos && position < script.size())
	{
		position = IsQuote(script[position]) ? ParseQuoted(keys, script, position)
											 : ParseWord(keys, script, position);
		position = script.find_first_not_of(WHITESPACE, position);
	}

	return keys;
}

Survey Scripted(const std::string_view script, Theme theme)
{
	return Survey(theme, std::make_unique<ScriptedTerminal>(ParseScript(script)));
}
} // namespace survey
