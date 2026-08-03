#include <vdfc/vdf.h>

#include "slocal/slocal_internal.h"

SLocalError slocal_read_and_parse_vdf(const char *path, VDFNode **out_node)
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
