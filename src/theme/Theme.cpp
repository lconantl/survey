#include "survey/Theme.hpp"
#include "color/ColorTable.hpp"
#include <array>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace survey
{
namespace
{
using ColorField = Color Theme::*;

constexpr auto WHITESPACE = " \t\r";
constexpr char COMMENT_MARKER = '#';
constexpr char ASSIGNMENT = '=';

constexpr std::array<std::pair<std::string_view, ColorField>, 6> THEME_FIELDS = {{
	{"question", &Theme::question},
	{"cursor", &Theme::cursor},
	{"selected", &Theme::selected},
	{"answer", &Theme::answer},
	{"hint", &Theme::hint},
	{"error", &Theme::error},
}};

std::string_view TrimView(const std::string_view text) noexcept
{
	const std::size_t first = text.find_first_not_of(WHITESPACE);
	if (first == std::string_view::npos)
	{
		return {};
	}

	const std::size_t last = text.find_last_not_of(WHITESPACE);
	return text.substr(first, last - first + 1);
}

bool IsSkippable(const std::string_view line) noexcept
{
	return line.empty() || line.front() == COMMENT_MARKER;
}

std::size_t AssertIsAssignment(const std::string_view line)
{
	const std::size_t position = line.find(ASSIGNMENT);
	if (position == std::string_view::npos)
	{
		throw std::invalid_argument(
			"Строка темы должна иметь вид «ключ = цвет»: " + std::string(line));
	}

	return position;
}

ColorField AssertIsKnownField(const std::string_view key)
{
	for (const auto& [name, field] : THEME_FIELDS)
	{
		if (name == key)
		{
			return field;
		}
	}

	throw std::invalid_argument("Неизвестный параметр темы: " + std::string(key));
}

void ApplyLine(Theme& theme, const std::string_view line)
{
	const std::size_t position = AssertIsAssignment(line);
	const std::string_view key = TrimView(line.substr(0, position));
	const std::string_view value = TrimView(line.substr(position + 1));

	theme.*AssertIsKnownField(key) = detail::ParseColor(value);
}

void AssertIsFileOpen(const std::ifstream& file, const std::filesystem::path& path)
{
	if (!file.is_open())
	{
		const std::u8string name = path.u8string();
		throw std::runtime_error(
			"Не удалось открыть файл темы: " + std::string(name.begin(), name.end()));
	}
}

std::string ReadFile(const std::filesystem::path& path)
{
	std::ifstream file(path, std::ios::binary);
	AssertIsFileOpen(file, path);

	std::ostringstream content;
	content << file.rdbuf();

	return content.str();
}
} // namespace

Theme Theme::Parse(const std::string_view text)
{
	Theme theme;
	std::size_t start = 0;

	while (start <= text.size())
	{
		const std::size_t end = std::min(text.find('\n', start), text.size());
		const std::string_view line = TrimView(text.substr(start, end - start));
		if (!IsSkippable(line))
		{
			ApplyLine(theme, line);
		}
		start = end + 1;
	}

	return theme;
}

Theme Theme::LoadFromFile(const std::filesystem::path& path)
{
	return Parse(ReadFile(path));
}
} // namespace survey
