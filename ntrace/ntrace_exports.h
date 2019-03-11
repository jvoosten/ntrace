#pragma once

#if defined(WIN32)
#if defined(NTRACE_EXPORTS)
#define NTRACE_EXPORT __declspec(dllexport)
#else
#define NTRACE_EXPORT __declspec(dllimport)
#endif
#define NTRACE_CALL __cdecl
#else
// Define as nothing
#define NTRACE_EXPORT
#define NTRACE_CALL
#endif
