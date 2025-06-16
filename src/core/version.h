#ifndef _VERSION_H
#define _VERSION_H

#include "platform.h"

#ifdef C2VM_WINDOWS
    #define C2VM_PLATFORM "Windows"
#elif defined(C2VM_LINUX)
    #define C2VM_PLATFORM "Linux"
#elif defined(C2VM_MACOS)
    #define C2VM_PLATFORM "macOS"
#else
    #define C2VM_PLATFORM "Unknown"
#endif

#ifdef C2VM_ARCH_X64
    #define C2VM_ARCH "x64"
#elif defined(C2VM_ARCH_X86)
    #define C2VM_ARCH "x86"
#elif defined(C2VM_ARCH_ARM32)
    #define C2VM_ARCH "ARM32"
#elif defined(C2VM_ARCH_ARM64)
    #define C2VM_ARCH "ARM64"
#else
    #define C2VM_ARCH "Unknown"
#endif

#define C2VM_VERSION    ("alpha v0.0.2 (" C2VM_PLATFORM "/" C2VM_ARCH ")")

static const char* c2vmVer()
{
    return C2VM_VERSION;
}

#endif