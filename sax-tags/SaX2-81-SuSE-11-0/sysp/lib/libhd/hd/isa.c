#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hd.h"
#include "hd_int.h"
#include "isa.h"

/**
 * @defgroup ISAint ISA
 * @ingroup libhdBUSint
 * @brief ISA bus scan functions
 *
 * @{
 */

#if defined(__i386__)

void hd_scan_isa(hd_data_t *hd_data)
{
  if(!hd_probe_feature(hd_data, pr_isa)) return;

  hd_data->module = mod_isa;

  /* some clean-up */
  remove_hd_entries(hd_data);
  // hd_data->isa = NULL;
}

#endif /* defined(__i386__) */

/** @} */

