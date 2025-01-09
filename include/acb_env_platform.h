#ifndef ACB_ENV_PLATFORM_H_
#define ACB_ENV_PLATFORM_H_

#if (defined(_WIN64) || defined(__LP64__) || defined(__LLP64__))
#define __ACB_ARCH_X64__
#else
#define __ACB_ARCH_X86__
#endif // (defined(_WIN64) || defined(__LP64__) || defined(__LLP64__))

#if (defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__))
#define __ACB_OS_WINDOWS__
#else
#define __ACB_OS_UNIX__
#endif // (defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__))

#endif // ACB_ENV_PLATFORM_H_
