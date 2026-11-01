#pragma once

#include <stdexcept>

namespace survey
{
class CancelledError : public std::runtime_error
{
public:
	CancelledError();
};
} // namespace survey
