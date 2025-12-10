#pragma once
#include <windows.h>

#define ANTIDEBUG(times) AntiDebug<times, __COUNTER__>::make();

template <size_t N>
class AntiDebugMethod
{
public:
    static bool use();
};
template <>
class AntiDebugMethod<0>
{
public:
    __forceinline static bool use()
    {
        if (IsDebuggerPresent())
        {
            return true;
        }
    }
};
template <>
class AntiDebugMethod<1>
{
public:
    __forceinline static bool use()
    {
        BOOL b;
        CheckRemoteDebuggerPresent(GetCurrentProcess(), &b);
        if (b)
        {
            return true;
        }
    }
};
template <>
class AntiDebugMethod<2>
{
public:
    __forceinline static bool use()
    {
        DWORD64 t1,t2;
        t1 = __rdtsc();
        t2 = __rdtsc();
        if (t2-t1>0x10)
        {
            return true;
        }
    }
};


template <size_t TIMES, size_t SEED>
class AntiDebug
{
public:
    constexpr static size_t N = 3;
    __forceinline static void make()
    {
        AntiDebugMethod<SEED%N>::use();
        AntiDebug<TIMES - 1, (SEED+1)^(__COUNTER__*123456)>::make();
    }
};

template <size_t SEED>
class AntiDebug<0, SEED>
{
public:
    __forceinline static void make()
    {
        //end
    }
};