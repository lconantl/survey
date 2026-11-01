### API

```cpp
#include <survey/Survey.hpp>

const survey::Survey prompt;

const Mood mood = prompt.Select<Mood>("Как ваше настроение?")
	.Option("Хорошо", Mood::Good)
	.Option("Нормально", Mood::Normal).Default()
	.Option("Плохо", Mood::Bad)
	.Ask();

const std::string city = prompt.Select("Город?").Option("Москва").Option("Казань").Ask();

const std::vector<std::string> languages = prompt.MultiSelect("Языки?")
	.Option("C++").Default()
	.Option("Go")
	.Option("Rust")
	.Min(1)
	.Max(2)
	.Ask();

const bool isConfirmed = prompt.Confirm("Отправить письмо?", true);

const std::string email = prompt.Input("Почта")
	.Default("me@example.com")
	.Validate(IsEmail, "Нужен адрес вида name@domain")
	.Ask();

const survey::SecretString password = prompt.Password("Пароль")
	.Mask('*')
	.Validate(IsNotEmpty, "Пароль не может быть пустым")
	.Ask();
```

- `Select<T>` / `MultiSelect<T>` возвращают значения типа `T`. Без шаблонного параметра
  `T = std::string`, и `Option("текст")` можно вызывать с одной подписью.
- `Validate` можно вызывать несколько раз. Срабатывает первое нарушенное правило, его
  текст выводится цветом ошибки, вопрос повторяется.
- `SecretString` затирает память при уничтожении. Значение доступно через `View()`.
  В консоли пароль выводится звёздочками, а после ответа — как `- ********` фиксированной длины.
- Ошибки программиста (`Default()` до `Option()`, `Min()` больше `Max()`, пустой
  валидатор) — `std::logic_error`.

### Тема

```cpp
survey::Theme theme;
theme.cursor = survey::Color::BrightCyan;
theme.selected = survey::Color::Green;

const survey::Survey prompt(theme);
const survey::Survey fromFile(survey::Theme::LoadFromFile("survey.ini"));
```

```ini
# survey.ini
question = bright_white
cursor = cyan
selected = green
answer = gray
hint = gray
error = red
```

Цвета: `default`, `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`,
`gray`, `bright_red`, `bright_green`, `bright_yellow`, `bright_blue`, `bright_magenta`,
`bright_cyan`, `bright_white`.

### Тесты

`survey::Scripted` создаёт опросник, который вместо клавиатуры читает сценарий нажатий:

```cpp
#include <survey/Scripted.hpp>

// Проверка выбора второго пункта
TEST(MenuTest, PicksSecond)
{
	const auto session = survey::Scripted("down space enter");

	EXPECT_EQ(session.Select("?").Option("A").Option("B").Ask(), "B");
}
```

Синтаксис сценария: `up`, `down`, `space`, `enter`, `esc`, `backspace`, `ctrl+c`,
любой одиночный символ (`2`, `x`) и текст в кавычках (`'me@mail.ru'`).
Если нажатий не хватило, бросается `std::runtime_error`.

Чтобы проверить вывод, создайте `ScriptedTerminal` сами:

```cpp
auto terminal = std::make_unique<survey::ScriptedTerminal>(survey::ParseScript("2 enter"));
const survey::ScriptedTerminal& output = *terminal;
const survey::Survey session({}, std::move(terminal));
(void)session.Select("?").Option("A").Option("B").Ask();

EXPECT_NE(output.Output().find("- B"), std::string::npos);
```

Свой терминал — реализуйте `survey::ITerminal` (`ReadKey`, `Write`).
