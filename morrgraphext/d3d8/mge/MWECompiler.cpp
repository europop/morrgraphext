#include "MWECompiler.h"
#include <stdlib.h>

//-----------------------------------------------------------------------------

int _stdcall MWECompiler(LPBYTE lpSCDT, int sizeSCDT) {
	int substitutions = 0;
	DWORD common[] = {	0x01130000, 0x01144012, 0x01154013, 0x01164014, 0x01174015, 0x01184016, 0x01194017, 0x011B4018,
						0x011D4019, 0x01224020, 0x011A4112, 0x011C4113, 0x011E4114, 0x01204115, 0x01214116, 0x01234117,
						0x011F4118, 0x01254119, 0x01284120, 0x01294212, 0x01274213, 0x012B4214, 0x012C4215, 0x012D4216,
						0x01304217, 0x012E4218, 0x012F4219, 0x01324220, 0x40194312, 0x41174115, 0x42144213, 0x42174216,
						0x00004312, 0x40120000, 0x41144016, 0x10D40000, 0x110D10D5, 0x10FD110E, 0x10F310FC, 0x00000000 };
	bool ten = false;
	for(int i = 0; i < sizeSCDT; i++) {
		if (ten = (!lpSCDT[i] && lpSCDT[i + 1]==0x10) ? 0 < (i += 4) : ten) {
			if (lpSCDT[i]) continue;
			else ten = false;
		}
		for(LPWORD j = LPWORD(&common[35])+1; j < LPWORD(&common[39]); j++) {
			if (*LPWORD(lpSCDT + i) == *j) {
				i += 2;
				if (lpSCDT[i]==0x2C && lpSCDT[i + 1]==0x20 && lpSCDT[i + 2]==0x6F)	i += 3;
				if (lpSCDT[i]==0x20 && lpSCDT[i + 1]==0x6F)							i += 2;
				i += lpSCDT[i] + 1;
			}
		}
		if (!lpSCDT[i] && lpSCDT[i + 1]==0x10) i += 2;
		for(LPDWORD j = LPDWORD(&common[1]); j < LPDWORD(&common[29]); j++) {
			bool exist = false;
			for(LPWORD k = LPWORD(&common[33])+1; k < LPWORD(&common[35]); k++) {
				if (*LPWORD(j) == *k) exist = true;
			}
			if ( !exist && *LPWORD(lpSCDT + i) == *(LPWORD(j) - 1)) {
				for(LPWORD k = LPWORD(&common[28])+1; k < LPWORD(&common[33]); k++) {
					if (*LPWORD(j) == *k && *LPDWORD(lpSCDT + i - 4) == 0x30303030) {
						for(int n = sizeof(DWORD); n > 0; n--) {
							char buffer[12];	// 11 is maximum available length of int value.
							_itoa_s(i, buffer, 12, 10);
							int length = strlen(buffer) + 1;
							if (n < length) lpSCDT[i - n] = buffer[length - n - 1];
							else lpSCDT[i - n] = 0x30;
						}
					}
				}
				*LPWORD(lpSCDT + i) = *LPWORD(j);
				substitutions++;
			}
		}
	}
	return substitutions;
}
