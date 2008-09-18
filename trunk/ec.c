#include <stdio.h>
#include "typedef.h"
#include "portio.h"

// Registers of the embedded controller
#define EC_DATAPORT		0x62	// EC data io-port
#define EC_CTRLPORT		0x66	// EC control io-port

// Embedded controller status register bits
#define EC_STAT_OBF		0x01    // Output buffer full 
#define EC_STAT_IBF		0x02    // Input buffer full
#define EC_STAT_CMD		0x08    // Last write was a command write (0=data)


// Embedded controller commands
// (write to EC_CTRLPORT to initiate read/write operation)
#define EC_CTRLPORT_READ		(BYTE)0x80
#define EC_CTRLPORT_WRITE		(BYTE)0x81
#define EC_CTRLPORT_QUERY		(BYTE)0x84

static BOOL isSTAT(BOOL stat, BYTE bitsmask)
{
	BYTE data;
	
	data = getPortB(EC_CTRLPORT);
	
	if (0 == (bitsmask & data))
	{
		return (FALSE == stat) ? TRUE : FALSE;
	}
	
	return (TRUE == stat) ? TRUE : FALSE;	
}


static BOOL isIOBufClean(void)
{
	return isSTAT(FALSE, EC_STAT_IBF | EC_STAT_OBF);
}

static BOOL isIBufClean(void)
{
	return isSTAT(FALSE, EC_STAT_IBF);
}

static BOOL isOBufClean(void)
{
	return isSTAT(FALSE, EC_STAT_OBF);
}

static BOOL isOBufFull(void)
{
	return isSTAT(TRUE, EC_STAT_OBF);
}

static void delaySometime(void)
{
	unsigned i, j;
	unsigned x, y;
	
	for (i = 0; i < 1000; i++)
		for(j = 0; j < 1000; j++)
		{
			x = y; 
			y = x;
		}
}

typedef enum { STAT_IOBUF_CLEAN = 0, STAT_IBUF_CLEAN, STAT_OBUF_CLEAN, STAT_OBUF_FULL } EC_STAT_TYPE;

static BOOL waitBufState(unsigned kind)
{
	const unsigned maxCnt = 5000;
	unsigned i = 0;
	BOOL fRet = FALSE;
	BOOL (*pFunc)(void);

	switch(kind) {
	case STAT_IOBUF_CLEAN:
		pFunc = isIOBufClean;
		break;
	case STAT_IBUF_CLEAN:
		pFunc= isIBufClean;
		break;
	case STAT_OBUF_CLEAN:
		pFunc = isOBufClean;
		break;
	case STAT_OBUF_FULL:
		pFunc = isOBufFull;
		break;
	default:
		printf("Failed: kind of functions in WaitBufState(),  LINE:%d, FILE: %s\r\n", __LINE__, __FILE__);
		return FALSE;
	}
	
	do
	{
		fRet = pFunc();
		if (TRUE == fRet)
		{
			break;
		}
		else
		{
			delaySometime();
		}
	} while (i++ < maxCnt);

	return fRet;
}

static BOOL waitIOBufClean(void)
{
	return waitBufState(STAT_IOBUF_CLEAN);
}

static BOOL waitIBufClean(void)
{
	return waitBufState(STAT_IBUF_CLEAN);
}

static BOOL waitOBufClean(void)
{
	return waitBufState(STAT_OBUF_CLEAN);
}

static BOOL waitOBufFull(void)
{
	return waitBufState(STAT_OBUF_FULL);
}

DEF_EXPORT BOOL ReadByteFromEC(int offset, BYTE* pdata)
{
	BOOL fRet = FALSE;
	
	if (TRUE == waitIOBufClean())
	{
		setPortB(EC_CTRLPORT, EC_CTRLPORT_READ);
		if (TRUE == waitIBufClean())
		{
			setPortB(EC_DATAPORT, offset);
			if (TRUE == waitOBufFull())
			{
				BYTE data;
				
				data = getPortB(EC_DATAPORT);
				*pdata = data;
				fRet = TRUE;
			}
			else
			{
			    printf("Failed: Timeout!! wait OBUF full in ReadByteFromEC()\r\n");
			}
		}
		else
		{
		    printf("Failed: Timeout!! wait IBUF clean. in ReadByteFromEC()\r\n");
		}
	}
	
	if (FALSE == fRet)
	{
		printf("Failed in ReadByteFromEC(), Line: %d, File: %s\r\n", __LINE__, __FILE__);
	}
	
	return fRet;
}

DEF_EXPORT BOOL WriteByteToEC(int offset, BYTE data)
{
	BOOL fRet = FALSE;
	if (TRUE == waitIOBufClean())
	{
		setPortB(EC_CTRLPORT, EC_CTRLPORT_WRITE);
		if (TRUE == waitIBufClean())
		{
			setPortB(EC_DATAPORT, offset);
			if (TRUE == waitIBufClean())
			{
				setPortB(EC_DATAPORT, data);
				if (TRUE == waitIBufClean())
				{
					fRet = TRUE;
				}
			}
		}
	}
	
	if (FALSE == fRet)
	{
		printf("Failed in WriteByteToEC(), Line: %d, File: %s\r\n", __LINE__, __FILE__);
	}
	
	return fRet;
}
