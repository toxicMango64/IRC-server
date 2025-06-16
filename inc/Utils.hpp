// Utils.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

// # include "Server.hpp"

#include <atomic>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <cerrno>
#include <cstring>
#include <iomanip>
#include <ctime>

bool validatePort( const std::string& portStr, int& result );
bool validatePassword( const std::string& password );

// // Compiler-specific macros for optimizations
// #if defined(_MSC_VER)
//   #define COLD_FN __declspec(noinline)
//   #define LIKELY(x) (x)
//   #define UNLIKELY(x) (x)
// #elif defined(__GNUC__) || defined(__clang__)
//   #define COLD_FN __attribute__((cold)) __attribute__((noinline))
//   #define LIKELY(x) __builtin_expect(!!(x), 1)
//   #define UNLIKELY(x) __builtin_expect(!!(x), 0)
// #else
//   #define COLD_FN
//   #define LIKELY(x) (x)
//   #define UNLIKELY(x) (x)
// #endif

#ifndef DEBUG_MODE /* DEBUG_MODE */
# define DEBUG_MODE true /* Set to false to disable debug output */
#endif /* DEBUG_MODE */

// // Lightweight global toggle (can be changed at runtime too)
// static constexpr bool kDebugEnabled = DEBUG_MODE;

// Optional: Enable if you're not using C++20's std::format
#define USE_SIMPLE_FORMATTER

// Lock to avoid mangled logs from concurrent threads
static std::mutex log_mutex;

#if defined(_MSC_VER)
#define COLD_FN __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define COLD_FN __attribute__((cold)) __attribute__((noinline))
#else
#define COLD_FN
#endif

COLD_FN inline void logError(const char* format, ...) {
#if DEBUG_MODE

	if (!format) return;

	std::ostringstream out;

	std::time_t t = std::time(NULL);
	char time_buf[64];
	std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
	out << "[" << time_buf << "] ";

	va_list args;
	va_start(args, format);

	for (const char* p = format; *p != '\0'; ++p) {
		if (*p == '%' && *(p + 1) != '\0') {
			++p;
			switch (*p) {
				case 's': {
					const char* str = va_arg(args, const char*);
					out << (str ? str : "(null)");
					break;
				}
				case 'd':
				case 'i': {
					int val = va_arg(args, int);
					out << val;
					break;
				}
				case 'x': {
					int val = va_arg(args, int);
					out << std::hex << val << std::dec;
					break;
				}
				case '%': {
					out << '%';
					break;
				}
				default:
					out << '%' << *p;
					break;
			}
		} else {
			out.put(*p);
		}
	}

	va_end(args);

	if (errno != 0) {
		out << " [errno: " << errno << " - " << std::strerror(errno) << "]";
	}

	std::cerr << "⚠️ ERROR: " << out.str() << std::endl;
#endif
}

#endif // UTILS_HPP
