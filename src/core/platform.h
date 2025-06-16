#ifndef _PLATFORM_H
#define _PLATFORM_H

#if defined(_WIN32) || defined(_WIN64)
    #define C2VM_WINDOWS
#elif defined(__linux__)
    #define C2VM_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
    #define C2VM_MACOS
#endif

#ifdef C2VM_WINDOWS
    #define VM_EXPORT __declspec(dllexport)
    #define VM_IMPORT __declspec(dllimport)
    #define VM_CALL __cdecl
#else
    #define VM_EXPORT __attribute__((visibility("default")))
    #define VM_IMPORT
    #define VM_CALL
#endif

#ifdef VM_BUILD
    #define VM_API VM_EXPORT
#else
    #define VM_API VM_IMPORT
#endif

#ifdef C2VM_WINDOWS
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

#if defined(__x86_64__) || defined(_M_X64)
    #define C2VM_ARCH_X64
#elif defined(__i386) || defined(_M_IX86)
    #define C2VM_ARCH_X86
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7S__)
    #define C2VM_ARCH_ARM32
#elif defined(__aarch64__)
    #define C2VM_ARCH_ARM64
#endif


#if defined(__kiwios__) || defined(__kiwios__)
    #define C2VM_KIWIOS
#endif

#endif