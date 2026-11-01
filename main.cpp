#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <survey/Survey.hpp>
#include <vector>

namespace
{
enum class Mood
{
	Good,
	Normal,
	Bad,
};

std::string DescribeMood(const Mood mood)
{
	switch (mood)
	{
	case Mood::Good: return "хорошее";
	case Mood::Normal: return "нормальное";
	case Mood::Bad: return "плохое";
	}

	return "неизвестное";
}

bool IsNotEmpty(const std::string_view text)
{
	return !text.empty();
}

bool IsEmail(const std::string_view text)
{
	const std::size_t at = text.find('@');
	return at != std::string_view::npos && at > 0 && at + 1 < text.size();
}

std::string JoinLanguages(const std::vector<std::string>& languages)
{
	std::string result;
	for (const auto& language : languages)
	{
		result += result.empty() ? language : ", " + language;
	}

	return result;
}

void RunSurvey()
{
	const survey::Survey prompt;

	const Mood mood = prompt.Select<Mood>("Как ваше настроение?")
						  .Option("Хорошо", Mood::Good)
						  .Option("Нормально", Mood::Normal)
						  .Option("Плохо", Mood::Bad)
						  .Ask();

	if (!prompt.Confirm("Хотите разработать программу?"))
	{
		std::cout << std::endl << "Жаль! Настроение: " << DescribeMood(mood) << std::endl;
		return;
	}

	const std::vector<std::string> languages = prompt.MultiSelect("На каких языках?")
												   .Option("C++")
												   .Default()
												   .Option("Go")
												   .Option("Python")
												   .Option("Rust")
												   .Min(1)
												   .Ask();

	const std::string email = prompt.Input("Ваша почта")
								  .Default("me@example.com")
								  .Validate(IsEmail, "Нужен адрес вида name@domain")
								  .Ask();

	const survey::SecretString password
		= prompt.Password("Пароль").Validate(IsNotEmpty, "Пароль не может быть пустым").Ask();

	std::cout << std::endl
			  << "Настроение: " << DescribeMood(mood) << std::endl
			  << "Языки: " << JoinLanguages(languages) << std::endl
			  << "Почта: " << email << std::endl
			  << "Пароль получен: " << password.Size() << " байт" << std::endl;
}
} // namespace

int main()
{
	try
	{
		RunSurvey();
	}
	catch (const survey::CancelledError& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Ошибка] " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
