#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <cstdarg>
#include <cerrno>
#include <cstring>
#include <iomanip>
#include <ctime>

bool validatePort( const std::string& portStr, int& result );
bool validatePassword( const std::string& password );

#ifndef DEBUG_MODE /* DEBUG_MODE */
# define DEBUG_MODE false /* Set to false to disable debug output */
#endif /* DEBUG_MODE */

#define USE_SIMPLE_FORMATTER

#if defined(_MSC_VER)
#define COLD_FN __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define COLD_FN __attribute__((cold)) __attribute__((noinline))
#else
#define COLD_FN
#endif

COLD_FN inline void logMsg(const char* format, ...) {
(void)format;
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

	std::cerr << "[DEBUG]: " << out.str() << std::endl;
#endif
}

#endif
