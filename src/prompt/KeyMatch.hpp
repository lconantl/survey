#pragma once

#include "render/Renderer.hpp"
#include "survey/Key.hpp"
#include <cstddef>
#include <optional>

namespace survey::detail
{
bool IsSpaceKey(const Key& key) noexcept;

std::optional<std::size_t> ToDigitIndex(const Key& key) noexcept;

void AssertIsNotCancelled(const Key& key, Renderer& renderer);
} // namespace survey::detail
