#pragma once

// DLL Defines
#ifdef switchStatement
#define EXPORT __declspec(dllimport)
#else
#define EXPORT __declspec(dllexport)
#endif // switchstatement

EXPORT void TestDLLFunc();