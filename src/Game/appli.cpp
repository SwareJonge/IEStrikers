#include <Shade/shd.h>

// might need to put this at the end of the file

const u32 shd_init_flag = UNK_0x400 | ENABLE_WIIMOTE | ENABLE_HBM | DISABLE_DEBUG;
const long shd_pktbf_sz = 0x180000; // is 0x200000 in EU and 2012