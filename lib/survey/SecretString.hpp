#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace survey
{
class SecretString final
{
public:
	SecretString() noexcept;
	explicit SecretString(std::string&& value) noexcept;
	~SecretString() noexcept;

	SecretString(const SecretString&) = delete;
	SecretString& operator=(const SecretString&) = delete;
	SecretString(SecretString&& other) noexcept;
	SecretString& operator=(SecretString&& other) noexcept;

	[[nodiscard]] std::string_view View() const noexcept;
	[[nodiscard]] std::size_t Size() const noexcept;
	[[nodiscard]] bool IsEmpty() const noexcept;

private:
	std::string m_value;
};
} // namespace survey
