#include "choice/ChoiceList.hpp"
#include <gtest/gtest.h>

using namespace testing;
using survey::detail::ChoiceList;
using survey::detail::SelectionMode;

// Проверка начальной позиции курсора на варианте по умолчанию
TEST(ChoiceListTest, CursorOnDefault)
{
	const ChoiceList list(3, SelectionMode::Single, {2});

	EXPECT_EQ(list.Cursor(), 2u);
}

// Проверка единственной отметки в одиночном выборе
TEST(ChoiceListTest, SingleExclusive)
{
	ChoiceList list(3, SelectionMode::Single, {});

	list.Pick(0);
	list.Pick(2);

	EXPECT_FALSE(list.IsMarked(0));
	EXPECT_EQ(list.Resolve(), (std::vector<std::size_t>{2}));
}

// Проверка запрета пустого списка
TEST(ChoiceListTest, RejectsEmpty)
{
	EXPECT_THROW(ChoiceList(0, SelectionMode::Single, {}), std::logic_error);
}

// Проверка запрета варианта по умолчанию вне списка
TEST(ChoiceListTest, RejectsBadDefault)
{
	EXPECT_THROW(ChoiceList(2, SelectionMode::Multiple, {5}), std::out_of_range);
}
