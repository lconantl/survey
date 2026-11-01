#include "KeyMatch.hpp"
#include "survey/CancelledError.hpp"

namespace survey::detail
{
namespace
{
bool IsCancelKey(const Key& key) noexcept
{
	return key.code == KeyCode::Escape || key.code == KeyCode::Interrupt;
}

bool IsDigitText(const std::string& text) noexcept
{
	return text.size() == 1 && text.front() >= '1' && text.front() <= '9';
}
} // namespace

bool IsSpaceKey(const Key& key) noexcept
{
	return key.code == KeyCode::Character && key.text == " ";
}

std::optional<std::size_t> ToDigitIndex(const Key& key) noexcept
{
	if (key.code != KeyCode::Character || !IsDigitText(key.text))
	{
		return std::nullopt;
	}

	return static_cast<std::size_t>(key.text.front() - '1');
}

void AssertIsNotCancelled(const Key& key, Renderer& renderer)
{
	if (IsCancelKey(key))
	{
		renderer.Clear();
		throw CancelledError();
	}
}
} // namespace survey::detail
