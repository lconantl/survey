#include "survey/detail/TextRequest.hpp"
#include <stdexcept>

namespace survey::detail
{
void AssertIsValidatorSet(const Validator& predicate)
{
	if (!predicate)
	{
		throw std::invalid_argument("Validate() получил пустую функцию проверки");
	}
}
} // namespace survey::detail
