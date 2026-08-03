#ifndef S_LOCAL
#define S_LOCAL

#include "slocal/slocal_data.h"
#include "slocal/slocal_errors.h"
#include <vdfc/vdf.h>

SLocalError slocal_init(SLocalData **data);
SLocalError slocal_scan_apps(SLocalData *data);

#endif
