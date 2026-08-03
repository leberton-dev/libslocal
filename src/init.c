#include "slocal/slocal_data.h"
#include "slocal/slocal_errors.h"
#include <stdio.h>
#include <string.h>
#include <vdfc/node.h>
#include <vdfc/parser.h>
#include <vdfc/vdf.h>
#include <wordexp.h>

static SLocalError _slocal_expand_home_path(const char *path, char **out)
{
	wordexp_t p;

	if (wordexp(path, &p, 0) != 0)
		return (SLOCAL_WORDEXP_ERR);
	*out = strdup(p.we_wordv[0]);
	wordfree(&p);
	if (!*out)
		return (SLOCAL_MALLOC_ERR);
	return (SLOCAL_OK);
}

static SLocalError _slocal_read_and_parse_vdf(const char *path, VDFNode **out_node)
{
	char   *out;
	size_t  out_size;
	VDFcode err;

	err = vdf_read_file(path, &out, &out_size);
	if (err != VDF_OK)
		return (SLOCAL_VDF_READ_FILE_ERR);

	err = vdf_parse(out, out_node);
	free(out);
	if (err != VDF_OK)
		return (SLOCAL_VDF_PARSE_ERR);
	return (SLOCAL_OK);
}

static VDFNode *_slocal_select_user_node(VDFNode *node)
{
	VDFNode *users;
	size_t   i;

	users = vdf_get(node, "users");
	if (!users)
		return (NULL);
	i = 0;
	while (i < users->child_count)
	{
		if (vdf_get_bool(users->children[i], "MostRecent", 0))
			return (users->children[i]);
		i++;
	}
	if (users->child_count > 0)
		return (users->children[0]);
	return (NULL);
}

static SLocalError _slocal_fill_login_info(SLocalLoginInfo *login_info, VDFNode *user)
{
	const char *account_name;
	const char *persona_name;

	login_info->steamid64 = strtoull(user->key, NULL, 10);
	login_info->accountid = (unsigned int) (login_info->steamid64 - 76561197960265728ULL);
	login_info->last_login = vdf_get_long(user, "Timestamp", 0);
	login_info->is_most_recent = vdf_get_bool(user, "MostRecent", 0);

	account_name = vdf_get_string(user, "AccountName", "");
	persona_name = vdf_get_string(user, "PersonaName", "");
	login_info->account_name = strdup(account_name);
	login_info->persona_name = strdup(persona_name);
	if (!login_info->account_name || !login_info->persona_name)
		return (SLOCAL_MALLOC_ERR);
	return (SLOCAL_OK);
}

static SLocalError _slocal_add_user_info_to_data(SLocalData *data)
{
	char       *expanded_path;
	VDFNode    *node;
	VDFNode    *user;
	SLocalError err;

	err = _slocal_expand_home_path(
	    "$HOME/.local/share/Steam/config/loginusers.vdf", &expanded_path);
	if (err != SLOCAL_OK)
		return (err);

	err = _slocal_read_and_parse_vdf(expanded_path, &node);
	free(expanded_path);
	if (err != SLOCAL_OK)
		return (err);

	user = _slocal_select_user_node(node);
	if (!user)
	{
		vdf_free_node(node);
		return (SLOCAL_ERR);
	}

	err = _slocal_fill_login_info(data->login_info, user);
	vdf_free_node(node);
	return (err);
}

SLocalError slocal_init(SLocalData **data)
{
	*data = (SLocalData *) malloc(sizeof(SLocalData));
	if (!data)
	{
		return (SLOCAL_MALLOC_ERR);
	}

	(*data)->login_info = (SLocalLoginInfo *) malloc(sizeof(SLocalLoginInfo));
	if (!(*data)->login_info)
	{
		free(data);
		return (SLOCAL_MALLOC_ERR);
	}

	return (_slocal_add_user_info_to_data(*data));
}
