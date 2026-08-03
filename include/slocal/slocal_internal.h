#ifndef INTERNAL_H
#define INTERNAL_H

#include <vdfc/node.h>

#include "slocal/slocal_errors.h"

SLocalError slocal_read_and_parse_vdf(const char *path, VDFNode **out_node);

#endif
