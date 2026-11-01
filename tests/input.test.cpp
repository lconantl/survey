#include <gtest/gtest.h>
#include <survey/Scripted.hpp>

using namespace testing;

namespace
{
bool IsNotEmpty(const std::string_view text)
{
	return !text.empty();
}
} // namespace

// Проверка ввода текста
TEST(InputBuilderTest, TypedText)
{
	const auto session = survey::Scripted("'me@mail.ru' enter");

	EXPECT_EQ(session.Input("Почта").Ask(), "me@mail.ru");
}

// Проверка значения по умолчанию
TEST(InputBuilderTest, DefaultValue)
{
	const auto session = survey::Scripted("enter");

	EXPECT_EQ(session.Input("Порт").Default("587").Ask(), "587");
}

// Проверка удаления UTF-8 символа
TEST(InputBuilderTest, BackspaceUtf8)
{
	const auto session = survey::Scripted("'при' backspace enter");

	EXPECT_EQ(session.Input("?").Ask(), "пр");
}

// Проверка повтора ввода после ошибки проверки
TEST(InputBuilderTest, ValidationRetries)
{
	const auto session = survey::Scripted("enter 'ok' enter");

	EXPECT_EQ(session.Input("?").Validate(IsNotEmpty, "Пусто").Ask(), "ok");
}

// Проверка запрета пустого валидатора
TEST(InputBuilderTest, EmptyValidator)
{
	const auto session = survey::Scripted("");

	EXPECT_THROW(session.Input("?").Validate(nullptr, "x"), std::invalid_argument);
}

// Проверка ввода пароля
TEST(PasswordBuilderTest, SecretValue)
{
	const auto session = survey::Scripted("'s3cret' enter");

	EXPECT_EQ(session.Password("Пароль").Ask().View(), "s3cret");
}

// Проверка, что пароль не попадает в вывод
TEST(PasswordBuilderTest, HiddenInOutput)
{
	auto terminal
		= std::make_unique<survey::ScriptedTerminal>(survey::ParseScript("'s3cret' enter"));
	const survey::ScriptedTerminal& output = *terminal;
	const survey::Survey session({}, std::move(terminal));

	(void)session.Password("Пароль").Ask();

	EXPECT_EQ(output.Output().find("s3cret"), std::string::npos);
}

// Проверка отмены ввода через Ctrl+C
TEST(PasswordBuilderTest, InterruptCancels)
{
	const auto session = survey::Scripted("'ab' ctrl+c");

	EXPECT_THROW((void)session.Password("Пароль").Ask(), survey::CancelledError);
}
