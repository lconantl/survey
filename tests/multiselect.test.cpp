#include <gtest/gtest.h>
#include <survey/Scripted.hpp>

using namespace testing;

// Проверка выбора нескольких пунктов цифрами
TEST(MultiSelectBuilderTest, DigitsToggle)
{
	const auto session = survey::Scripted("1 3 enter");

	const auto picked
		= session.MultiSelect<int>("?").Option("A", 1).Option("B", 2).Option("C", 3).Ask();

	EXPECT_EQ(picked, (std::vector{1, 3}));
}

// Проверка снятия отметки повторным нажатием
TEST(MultiSelectBuilderTest, UntoggleSpace)
{
	const auto session = survey::Scripted("space down space up space enter");

	EXPECT_EQ(
		session.MultiSelect("?").Option("A").Option("B").Ask(), (std::vector<std::string>{"B"}));
}

// Проверка вариантов по умолчанию
TEST(MultiSelectBuilderTest, Defaults)
{
	const auto session = survey::Scripted("enter");

	const auto picked
		= session.MultiSelect("?").Option("A").Default().Option("B").Option("C").Default().Ask();

	EXPECT_EQ(picked, (std::vector<std::string>{"A", "C"}));
}

// Проверка повтора вопроса при нарушении минимума
TEST(MultiSelectBuilderTest, MinRetries)
{
	const auto session = survey::Scripted("enter space enter");

	EXPECT_EQ(session.MultiSelect("?").Option("A").Option("B").Min(1).Ask(),
		(std::vector<std::string>{"A"}));
}

// Проверка ограничения максимума
TEST(MultiSelectBuilderTest, MaxRetries)
{
	const auto session = survey::Scripted("1 2 enter 2 enter");

	EXPECT_EQ(session.MultiSelect("?").Option("A").Option("B").Max(1).Ask(),
		(std::vector<std::string>{"A"}));
}

// Проверка пустого ответа без вариантов по умолчанию
TEST(MultiSelectBuilderTest, EmptyAnswer)
{
	const auto session = survey::Scripted("enter");

	EXPECT_TRUE(session.MultiSelect("?").Option("A").Ask().empty());
}

// Проверка запрета минимума больше количества
TEST(MultiSelectBuilderTest, UnreachableMin)
{
	const auto session = survey::Scripted("enter");

	EXPECT_THROW((void)session.MultiSelect("?").Option("A").Min(2).Ask(), std::logic_error);
}
