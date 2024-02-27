#pragma once

#ifdef _WIN32
#ifdef RPRF_EXPORT_API
#define RPRF_API __declspec(dllexport)
#else
#define RPRF_API __declspec(dllimport)
#endif
#else
#define RPRF_API __attribute__((visibility("default")))
#endif

