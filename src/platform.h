#ifndef L_PLATFORM_H
#define L_PLATFORM_H

#ifdef _WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

typedef struct l_plat_dl
{
#ifdef _WIN32
	HMODULE handle;
#else
	void* handle;
#endif
	int success;
} l_plat_dl;

l_plat_dl* l_plat_dl_open(char* fname);
void* l_plat_dl_read(l_plat_dl* dl, char* symbol);
void l_plat_dl_close(l_plat_dl*);

#endif
