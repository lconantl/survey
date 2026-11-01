#include "survey/detail/ChoiceRequest.hpp"
#include <stdexcept>

namespace survey::detail
{
void AssertHasOptions(const std::size_t count)
{
	if (count == 0)
	{
		throw std::logic_error("Список вариантов ответа пуст: сначала добавьте Option()");
	}
}
} // namespace survey::detail
