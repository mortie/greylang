#include "../platform.h"

#include <dlfcn.h>
#include <stdlib.h>

l_plat_dl l_plat_dl_open(char* fname)
{
	l_plat_dl dl = {
		.handle = dlopen(fname, RTLD_LAZY)
	};
	dl.success = dl.handle != NULL;

	return dl;
}

void* l_plat_dl_read(l_plat_dl dl, char* symbol)
{
	return dlsym(dl.handle, symbol);
}

void l_plat_dl_close(l_plat_dl dl)
{
	dlclose(dl.handle);
}
