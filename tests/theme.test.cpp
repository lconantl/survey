#include <gtest/gtest.h>
#include <survey/Theme.hpp>

using namespace testing;

// Проверка разбора цветов темы
TEST(ThemeTest, ParsesColors)
{
	const survey::Theme theme
		= survey::Theme::Parse("cursor = red\n# заметка\n\nselected=bright_green");

	EXPECT_EQ(theme.cursor, survey::Color::Red);
	EXPECT_EQ(theme.selected, survey::Color::BrightGreen);
	EXPECT_EQ(theme.error, survey::Color::Red);
}

// Проверка ошибки на неизвестном параметре
TEST(ThemeTest, UnknownKey)
{
	EXPECT_THROW(survey::Theme::Parse("border = red"), std::invalid_argument);
}

// Проверка ошибки на неизвестном цвете
TEST(ThemeTest, UnknownColor)
{
	EXPECT_THROW(survey::Theme::Parse("cursor = pink"), std::invalid_argument);
}

// Проверка ошибки на отсутствующем файле
TEST(ThemeTest, MissingFile)
{
	EXPECT_THROW(survey::Theme::LoadFromFile("missing-theme.ini"), std::runtime_error);
}
