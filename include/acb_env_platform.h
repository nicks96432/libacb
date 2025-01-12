#ifndef ACB_ENV_PLATFORM_H_
#define ACB_ENV_PLATFORM_H_

#if (defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__))
#define ACB_OS_WINDOWS
#else
#define ACB_OS_UNIX
#endif // (defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__))

#endif // ACB_ENV_PLATFORM_H_
