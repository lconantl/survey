#include "survey/CancelledError.hpp"

namespace survey
{
CancelledError::CancelledError()
	: std::runtime_error("Опрос отменён пользователем")
{
}
} // namespace survey
