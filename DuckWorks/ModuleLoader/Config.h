#pragma once

#ifdef MODULELOADER_EXPORTS
#define MODULELOADER_API __declspec(dllexport)
#else
#define MODULELOADER_API __declspec(dllimport)
#endif