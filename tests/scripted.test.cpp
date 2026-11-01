#include <gtest/gtest.h>
#include <survey/Scripted.hpp>

using namespace testing;

// Проверка разбора именованных клавиш и текста
TEST(ScriptedTest, ParsesKeys)
{
	const std::vector<survey::Key> keys = survey::ParseScript("down 'ab' enter");

	ASSERT_EQ(keys.size(), 4u);
	EXPECT_EQ(keys[0].code, survey::KeyCode::Down);
	EXPECT_EQ(keys[2].text, "b");
	EXPECT_EQ(keys[3].code, survey::KeyCode::Enter);
}

// Проверка ошибки на неизвестной клавише
TEST(ScriptedTest, UnknownKey)
{
	EXPECT_THROW(survey::ParseScript("jump"), std::invalid_argument);
}

// Проверка ошибки на незакрытой кавычке
TEST(ScriptedTest, UnclosedQuote)
{
	EXPECT_THROW(survey::ParseScript("'abc"), std::invalid_argument);
}

// Проверка ошибки, когда нажатия закончились
TEST(ScriptedTest, Exhausted)
{
	const auto session = survey::Scripted("down");

	EXPECT_THROW((void)session.Select("?").Option("A").Ask(), std::runtime_error);
}
