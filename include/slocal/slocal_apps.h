#ifndef SLOCAL_APP_H
#define SLOCAL_APP_H

#include <stddef.h>

typedef struct s_SLocalAppInfo
{
	unsigned int       appid;
	const char        *name;
	const char        *installdir;
	unsigned long long size_on_disk;
	long               last_updated;
	long               last_played;
	int                state_flags;
} SLocalAppInfo;

typedef struct s_SLocalAppList
{
	SLocalAppInfo *apps;
	size_t         count;
} SLocalAppList;

#endif
