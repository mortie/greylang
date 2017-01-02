#include "../platform.h"

#include <stdlib.h>

l_plat_dl *l_plat_dl_open(char *fname)
{
	l_plat_dl *dl = malloc(sizeof(*dl));
#ifdef _WIN32
	dl->handle = LoadLibrary(fname);
	dl->success = dl->handle != NULL;
#else
	dl->handle = dlopen(fname, RTLD_LAZY);
	dl->success = dl->handle != NULL;
#endif

	return dl;
}

void *l_plat_dl_read(l_plat_dl *dl, char *symbol)
{
#ifdef _WIN32
	return GetProcAddress(dl->handle, symbol);
#else
	return dlsym(dl->handle, symbol);
#endif
}

void l_plat_dl_close(l_plat_dl *dl)
{
#ifdef _WIN32
	FreeLibrary(dl->handle);
#else
	dlclose(dl->handle);
#endif

	free(dl);
}
