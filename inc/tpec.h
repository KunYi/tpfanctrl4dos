#ifndef _THINKPAD_EC_
#define _THINKPAD_EC_

#define TPEC_FAN			0x2F	// 1 byte (binary xyzz zzz)
#define TPEC_FANSPEED 		0x84	// 16 bit word, lo/hi byte
#define TPEC_TEMP_CPU		0x78	// 8 temp sensor bytes from here
#define TPEC_TEMP_APS		0x79
#define TPEC_TEMP_PCCARD	0x7A
#define TPEC_TEMP_GPU		0x7B
#define TPEC_TEMP_BAT1		0x7C
#define TPEC_RSR1			0x7D
#define TPEC_TEMP_BAT2		0x7E
#define TPEC_RSR2			0x7F

#define TP_ECOFFSET_TEMP1    (char)0xC0 // 4 temp sensor bytes from here

#endif /* end of _THINKPAD_EC_ */
