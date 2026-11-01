#include "WipeString.hpp"

namespace survey::detail
{
void WipeString(std::string& value) noexcept
{
	value.resize(value.capacity());

	volatile char* data = value.data();
	for (std::size_t index = 0; index < value.size(); ++index)
	{
		data[index] = '\0';
	}

	value.clear();
}
} // namespace survey::detail
