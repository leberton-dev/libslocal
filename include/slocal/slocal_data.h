#ifndef S_LOCAL_DATA
#define S_LOCAL_DATA

typedef struct s_SLocalLoginInfo
{
	unsigned long long steamid64;
	unsigned int       accountid;
	const char        *account_name;
	const char        *persona_name;
	long               last_login;
	int                is_most_recent;
} SLocalLoginInfo;

typedef struct s_SLocalData
{
	SLocalLoginInfo *login_info;
} SLocalData;

#endif
