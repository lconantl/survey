#pragma once

#ifndef _WIN32
#include <termios.h>
#endif

namespace survey::detail
{
class RawInputGuard final
{
public:
	RawInputGuard();
	~RawInputGuard() noexcept;

	RawInputGuard(const RawInputGuard&) = delete;
	RawInputGuard& operator=(const RawInputGuard&) = delete;
	RawInputGuard(RawInputGuard&&) = delete;
	RawInputGuard& operator=(RawInputGuard&&) = delete;

private:
#ifdef _WIN32
	void* m_handle;
	unsigned long m_previousMode;
#else
	termios m_previousSettings;
#endif
};
} // namespace survey::detail
