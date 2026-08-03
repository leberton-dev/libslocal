#include "slocal/slocal.h"
#include <stdio.h>

void print_login_info_context(SLocalLoginInfo *info)
{
	printf("\n\n=== PRINTING LOGIN INFO ===\n\n");
	printf("SteamId: %llu\n", info->steamid64);
	printf("AccountId: %u\n", info->accountid);
	printf("AccountName: %s\n", info->account_name);
	printf("PersonaName: %s\n", info->persona_name);
	printf("LastLogin: %ld\n", info->last_login);
	printf("IsMostRecent: %d\n", info->is_most_recent);
	printf("\n\n");
}

int main(void)
{
	SLocalError err;
	SLocalData *data;

	err = slocal_init(&data);
	if (err != SLOCAL_OK)
	{
		return (err);
	}

	print_login_info_context(data->login_info);

	return (0);
}
