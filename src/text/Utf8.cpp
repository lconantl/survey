#include "Utf8.hpp"

namespace survey::detail
{
namespace
{
constexpr char32_t ONE_BYTE_LIMIT = 0x80;
constexpr char32_t TWO_BYTES_LIMIT = 0x800;
constexpr char32_t THREE_BYTES_LIMIT = 0x10000;
constexpr unsigned char CONTINUATION_MASK = 0xC0;
constexpr unsigned char CONTINUATION_MARKER = 0x80;

char ToByte(const char32_t value)
{
	return static_cast<char>(static_cast<unsigned char>(value));
}

char Continuation(const char32_t codePoint, const int shift)
{
	return ToByte(0x80 | ((codePoint >> shift) & 0x3F));
}

bool IsContinuationByte(const char symbol) noexcept
{
	return (static_cast<unsigned char>(symbol) & CONTINUATION_MASK) == CONTINUATION_MARKER;
}
} // namespace

std::string EncodeUtf8(const char32_t codePoint)
{
	if (codePoint < ONE_BYTE_LIMIT)
	{
		return {ToByte(codePoint)};
	}

	if (codePoint < TWO_BYTES_LIMIT)
	{
		return {ToByte(0xC0 | (codePoint >> 6)), Continuation(codePoint, 0)};
	}

	if (codePoint < THREE_BYTES_LIMIT)
	{
		return {ToByte(0xE0 | (codePoint >> 12)),
			Continuation(codePoint, 6),
			Continuation(codePoint, 0)};
	}

	return {ToByte(0xF0 | (codePoint >> 18)),
		Continuation(codePoint, 12),
		Continuation(codePoint, 6),
		Continuation(codePoint, 0)};
}

std::size_t CountCodePoints(const std::string_view text) noexcept
{
	std::size_t count = 0;
	for (const char symbol : text)
	{
		if (!IsContinuationByte(symbol))
		{
			++count;
		}
	}

	return count;
}

void RemoveLastCodePoint(std::string& text) noexcept
{
	while (!text.empty() && IsContinuationByte(text.back()))
	{
		text.pop_back();
	}

	if (!text.empty())
	{
		text.pop_back();
	}
}

std::vector<std::string> SplitCodePoints(const std::string_view text)
{
	std::vector<std::string> codePoints;
	for (const char symbol : text)
	{
		if (IsContinuationByte(symbol) && !codePoints.empty())
		{
			codePoints.back() += symbol;
			continue;
		}
		codePoints.emplace_back(1, symbol);
	}

	return codePoints;
}
} // namespace survey::detail
