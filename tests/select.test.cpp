#include <gtest/gtest.h>
#include <survey/Scripted.hpp>

using namespace testing;

// Проверка выбора по умолчанию первого пункта
TEST(SelectBuilderTest, FirstByDefault)
{
	const auto session = survey::Scripted("enter");

	EXPECT_EQ(session.Select("?").Option("A").Option("B").Ask(), "A");
}

// Проверка явного варианта по умолчанию
TEST(SelectBuilderTest, ExplicitDefault)
{
	const auto session = survey::Scripted("enter");

	EXPECT_EQ(session.Select("?").Option("A").Option("B").Default().Ask(), "B");
}

// Проверка выбора стрелкой и пробелом
TEST(SelectBuilderTest, ArrowAndSpace)
{
	const auto session = survey::Scripted("down down space enter");

	EXPECT_EQ(session.Select<int>("?").Option("A", 1).Option("B", 2).Option("C", 3).Ask(), 3);
}

// Проверка: без пробела Enter берёт вариант по умолчанию
TEST(SelectBuilderTest, EnterIgnoresCursor)
{
	const auto session = survey::Scripted("down enter");

	EXPECT_EQ(session.Select("?").Option("A").Option("B").Ask(), "A");
}

// Проверка выбора цифрой
TEST(SelectBuilderTest, DigitPicks)
{
	const auto session = survey::Scripted("2 enter");

	EXPECT_EQ(session.Select("?").Option("A").Option("B").Option("C").Ask(), "B");
}

// Проверка перехода курсора по кругу
TEST(SelectBuilderTest, CursorWraps)
{
	const auto session = survey::Scripted("up space enter");

	EXPECT_EQ(session.Select("?").Option("A").Option("B").Option("C").Ask(), "C");
}

// Проверка подтверждения с отказом по умолчанию
TEST(SelectBuilderTest, ConfirmDefaultNo)
{
	const auto session = survey::Scripted("enter");

	EXPECT_FALSE(session.Confirm("Удалить?", false));
}

// Проверка отмены через Esc
TEST(SelectBuilderTest, EscapeCancels)
{
	const auto session = survey::Scripted("esc");

	EXPECT_THROW((void)session.Select("?").Option("A").Ask(), survey::CancelledError);
}

// Проверка запрета Default() без вариантов
TEST(SelectBuilderTest, DefaultWithoutOption)
{
	const auto session = survey::Scripted("");

	EXPECT_THROW(session.Select("?").Default(), std::logic_error);
}

// Проверка свёрнутого ответа в выводе
TEST(SelectBuilderTest, PrintsAnswer)
{
	auto terminal = std::make_unique<survey::ScriptedTerminal>(survey::ParseScript("2 enter"));
	const survey::ScriptedTerminal& output = *terminal;
	const survey::Survey session({}, std::move(terminal));

	(void)session.Select("Настроение?").Option("Хорошо").Option("Плохо").Ask();

	EXPECT_NE(output.Output().find("- Плохо"), std::string::npos);
}
