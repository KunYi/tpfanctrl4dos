#ifndef _BIOS_EC_
#define _BIOS_EC_

#include "typedef.h"

BOOL ReadByteFromEC(int offset, BYTE* pdata);
BOOL WriteByteToEC(int offset, BYTE data);

#endif /* end of _BIOS_EC_ */
