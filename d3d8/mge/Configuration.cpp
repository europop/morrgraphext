#define _CONF

#pragma setlocale("C")

#include "Configuration.h"
#include "ConfigInternal.h"
#include "INIData.h"
#include <stdlib.h>

double getSettingValue (char* sval, iniSetting set) {
	tdictionary *dict = set.dictionary;
	if (dict) for (unsigned int i = 0; i < dict->length; ++i) {
		tdictent dictent = dict->dictent [i];
		if (!_stricmp (sval, dictent.key)) {
			if (set.flags & MINMAX) {
				if (dictent.value > set.max) return set.max;
				if (dictent.value < set.min) return set.min;
			}
			return dictent.value;
		}
	}
	if (!(set.flags & DICTONLY)) {
		char *endptr;
		double temp = strtod(sval, &endptr);
		if (sval + strlen (sval) != endptr) return atof (set.defval);
		else {
			if (set.flags & MINMAX) {
				if (temp > set.max) return set.max;
				if (temp < set.min) return set.min;
			}
			return temp;
		}
	} else if (dict) for (unsigned int i = 0; i < dict->length; ++i) {
		tdictent dictent = dict->dictent [i];
		if (!_stricmp (set.defval, dictent.key)) return dictent.value;
	}
	return NAN;
}

void utf8cpyToA_s (char *_Dst, rsize_t _SizeInBytes, const char *_Src) {
	size_t size = MultiByteToWideChar (CP_UTF8, 0, _Src, -1, NULL, NULL);
	WCHAR *buffer = new WCHAR [size];
	MultiByteToWideChar (CP_UTF8, 0, _Src, -1, buffer, size);
	WideCharToMultiByte (CP_ACP, 0, buffer, -1, _Dst, _SizeInBytes, NULL, NULL);
	delete [] buffer;
}

bool LoadSettings () {
	int buffersize = 16384;
	char *buffer = new char[buffersize];
	char *pointer;
	GetPrivateProfileSectionNames (buffer, buffersize, mgeini);
	pointer = buffer;
	WizardPlugins = NULL;
	bool initialized = false;
	while (*pointer != '\0') {
		if (_stricmp(pointer, siniRendState) == 0) {
			initialized = true;
		} else if (_stricmp(pointer, siniDLWizardPlugins) == 0) {
			WizardPlugins = new std::vector<std::string>;
		}
		pointer += strlen (pointer) + 1;
	}
	if (!initialized) { delete[] buffer; return false; }
	for (int i = 0; i < countof (iniSettings); ++i) {
		iniSetting set = iniSettings [i];
		GetPrivateProfileString (set.section, set.key, set.defval, buffer, buffersize, mgeini);
		if (set.type == t_string) utf8cpyToA_s ((char*)set.variable, set.bit_size, buffer);
		else if (set.type != t_set) {
			double temp = getSettingValue (buffer, set);
			switch (set.type) {
				case t_bit:
					if (temp == 1) BitSet (set.variable, set.bit_size);
					else BitReset (set.variable, set.bit_size);
					break;
				case t_bool:
					*(bool*)set.variable = (temp == 1);
					break;
				case t_uint8:
					*(unsigned __int8*)set.variable = (unsigned __int8)temp;
					break;
				case t_int8:
					*(signed __int8*)set.variable = (signed __int8)temp;
					break;
				case t_uint16:
					*(unsigned __int16*)set.variable = (unsigned __int16)temp;
					break;
				case t_int16:
					*(signed __int16*)set.variable = (signed __int16)temp;
					break;
				case t_uint32:
					*(unsigned __int32*)set.variable = (unsigned __int32)temp;
					break;
				case t_int32:
					*(signed __int32*)set.variable = (signed __int32)temp;
					break;
				case t_float:
					*(float*)set.variable = (float)temp;
					break;
				case t_double:
					*(double*)set.variable = temp;
					break;
			}
		}
	}
	if (WizardPlugins) {
		GetPrivateProfileSection(siniDLWizardPlugins, buffer, buffersize, mgeini);
		pointer = buffer;
		while (*pointer != '\0') {
			std::string plugin;
			int linelength = strlen(pointer) + 1;
			LPSTR separator = strstr(pointer, ">");
			if(separator++) {
				plugin = separator + strspn(separator, " \t");
				plugin += "\\";
				do { *separator = '\0';
				} while (separator > pointer ? strpbrk(--separator, "> \t") : false );
			} plugin += pointer;
			((std::vector<std::string>*)WizardPlugins)->push_back(plugin);
			pointer += linelength;
		}
		if (pointer == buffer) { delete WizardPlugins; WizardPlugins = NULL; }
	}
	delete[] buffer;
	return true;
}

LPVOID GetSectionComments(LPCSTR section, LPCSTR filename) {
	std::vector<std::string>* list = NULL;
	HANDLE hIniFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(hIniFile == INVALID_HANDLE_VALUE) return NULL;
	DWORD dwBytesRead, dwSize = GetFileSize(hIniFile, NULL);
	char *lpBuffer = new char[dwSize];
	if(ReadFile(hIniFile, lpBuffer, dwSize, &dwBytesRead, NULL) ? dwBytesRead == dwSize : false) {
		LPSTR lpSection = strlwr(strcat(strcat(strcpy(new char[strlen(section) + 3], "["), section), "]"));
		//list = new std::vector<std::string>();
	}
	CloseHandle(hIniFile);
	delete[] lpBuffer;
	return list;
}

bool SetDistantLand(bool EnableDL) {
	return WritePrivateProfileString("Distant Land", "Distant Land", EnableDL ? "True" : "False", "MGE3/MGE.ini") ? true : false;
}