#include "text/Utf8.hpp"
#include <gtest/gtest.h>

using namespace testing;
using namespace survey::detail;

// Проверка кодирования кириллицы
TEST(Utf8Test, EncodesCyrillic)
{
	EXPECT_EQ(EncodeUtf8(U'Ж'), "Ж");
}

// Проверка кодирования эмодзи
TEST(Utf8Test, EncodesEmoji)
{
	EXPECT_EQ(EncodeUtf8(U'😀'), "😀");
}

// Проверка подсчёта символов
TEST(Utf8Test, CountsCodePoints)
{
	EXPECT_EQ(CountCodePoints("aЖ😀"), 3u);
}

// Проверка разбиения на символы
TEST(Utf8Test, SplitsCodePoints)
{
	EXPECT_EQ(SplitCodePoints("aЖ"), (std::vector<std::string>{"a", "Ж"}));
}
