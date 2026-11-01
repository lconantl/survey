#include "survey/SecretString.hpp"
#include "secret/WipeString.hpp"
#include <utility>

namespace survey
{
SecretString::SecretString() noexcept = default;

SecretString::SecretString(std::string&& value) noexcept
	: m_value(std::move(value))
{
	detail::WipeString(value);
}

SecretString::~SecretString() noexcept
{
	detail::WipeString(m_value);
}

SecretString::SecretString(SecretString&& other) noexcept
	: m_value(std::move(other.m_value))
{
	detail::WipeString(other.m_value);
}

SecretString& SecretString::operator=(SecretString&& other) noexcept
{
	if (this != &other)
	{
		detail::WipeString(m_value);
		m_value = std::move(other.m_value);
		detail::WipeString(other.m_value);
	}

	return *this;
}

std::string_view SecretString::View() const noexcept
{
	return m_value;
}

std::size_t SecretString::Size() const noexcept
{
	return m_value.size();
}

bool SecretString::IsEmpty() const noexcept
{
	return m_value.empty();
}
} // namespace survey
