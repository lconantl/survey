#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace survey::detail
{
std::string EncodeUtf8(char32_t codePoint);

std::size_t CountCodePoints(std::string_view text) noexcept;

void RemoveLastCodePoint(std::string& text) noexcept;

std::vector<std::string> SplitCodePoints(std::string_view text);
} // namespace survey::detail
