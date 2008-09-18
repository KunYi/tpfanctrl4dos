
#include <stdio.h>
#include <stdlib.h>

#include "ec.h"
#include "tpec.h"

static void printtitle(void)
{
	char bd[] = "*************************************************************\r\n";
	printf(bd);
	printf("**              Thinkpad Fan control for DOS               **\r\n");
	printf("** The program base on \"tp4xfancontrol\", more information  **\r\n");
	printf("** the following link.                                     **\r\n");
	printf("** link: http://sourceforge.net/projects/tp4xfancontrol/   **\r\n");
	printf(bd);
}

typedef struct
{
	int offset;
	int size;
	BYTE* data;
} EC_DATA_ROW;

typedef struct _TP_DATA_
{
	BYTE	fan;
	BYTE	fanspeed[2];
	BYTE	tmpCPU;
	BYTE	tmpAPS;
	BYTE	tmpPCCARD;
	BYTE	tmpGPU;
	BYTE	tmpBAT1;
	BYTE	tmpBAT2;
} TP_ECDATA;

static TP_ECDATA tpdata;

static EC_DATA_ROW  tpspec[] = 
{ 
	{ TPEC_FAN, 1, &tpdata.fan },
	{ TPEC_FANSPEED, 2, &(tpdata.fanspeed[0]) },
	{ TPEC_TEMP_CPU, 1, &tpdata.tmpCPU },
	{ TPEC_TEMP_APS, 1, &tpdata.tmpAPS },
	{ TPEC_TEMP_PCCARD, 1, &tpdata.tmpPCCARD },
	{ TPEC_TEMP_GPU, 1, &tpdata.tmpGPU },
	{ TPEC_TEMP_BAT1, 1, &tpdata.tmpBAT1 },
	{ TPEC_TEMP_BAT2, 1, &tpdata.tmpBAT2 }
};

static void ReadAllDataFromEC(void)
{
	int i;
	for (i = 0; i < sizeof(tpspec)/sizeof(EC_DATA_ROW); i++)
	{
		int size = 0;
		do
		{
			ReadByteFromEC((tpspec[i].offset + size), (tpspec[i].data + size));
			size++;
		} while(size < tpspec[i].size);
	}
}

static void printAllData(void)
{
	printf("Fan : %d(0x%x)\r\n", tpdata.fan, tpdata.fan);
	printf("Speed of Fan : %d RPM\r\n", ((tpdata.fanspeed[1] << 8) | tpdata.fanspeed[0]));
	printf("==== Temperture list  ====\r\n");
	printf("  CPU: %d\r\n",tpdata.tmpCPU);
	printf("  APS: %d\r\n",tpdata.tmpAPS);
	// printf("  PCCARD: %d\r\n",tpdata.tmpPCCARD);
	printf("  GPU: %d\r\n",tpdata.tmpGPU);
	printf("  BAT1: %d\r\n",tpdata.tmpBAT1);
	printf("  BAT2: %d\r\n",tpdata.tmpBAT2);
}

void SetFan(int step)
{
	BYTE value = 0x40;
	BYTE data;
	
	switch(step) {
	case 0:	case 1:	case 2:	case 3:
	case 4:	case 5:	case 6:	case 7:
		value = step;
		break;
	case 8:
		value = 0x40;
		break;
	case 9:
		value = 0x80;
		break;
	}
		
	WriteByteToEC(TPEC_FAN, value);
	data = 0;
	ReadByteFromEC(TPEC_FAN, &data);
	if (data != value)
		WriteByteToEC(TPEC_FAN, value);

}

int main(int argv, char* argc[])
{
	int mode = 7; // default max. speed

	printtitle();
	
	if (argv > 1)
	{
		mode = atoi(argc[1]);
	}
	
	SetFan(mode);
	printf("Now!! Reading EC Setting\r\n");	
	ReadAllDataFromEC();
	printAllData();
	return 0;
}
