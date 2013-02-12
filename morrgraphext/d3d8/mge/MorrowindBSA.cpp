#include "d3d8header.h" //**/#include "proxydx/d3d8header.h" #include "support/strsec.h"
#include <map>
#include "Configuration.h"
#include "MorrowindBSA.h"

struct BSAEntry {
	HANDLE file;
	DWORD position;
	DWORD size;
};
struct BSAHash3 {
	union {
		struct {
			DWORD value1;
			DWORD value2;
		};
		__int64 LValue;
	};
};

CRITICAL_SECTION cache_crit_sec;
static std::map<__int64, BSAEntry> BSAEntriesStatics, BSAEntries;
static std::map<__int64, IDirect3DTexture9*> BSALoadedTextures;

static BSAHash3 HashString(const char* str) {
	BSAHash3 result;

	unsigned int len = (unsigned int)strlen(str);

	//Use GhostWheel's code to hash the string
	unsigned int l = (len>>1);
	unsigned int sum, off, temp, i, n;

	for(sum = off = i = 0; i < l; i++) {
		sum ^= (((unsigned int)(str[i]))<<(off&0x1F));
		off += 8;
	}
	result.value1 = sum;

	for(sum = off = 0; i < len; i++) {
		temp = (((unsigned int)(str[i]))<<(off&0x1F));
		sum ^= temp;
		n = temp & 0x1F;
		sum = (sum << (32-n)) | (sum >> n);  // binary "rotate right"
		off += 8;
	}
	result.value2 = sum;
	return result;
}

static void OpenBSA(const char* fileName, std::map<__int64, BSAEntry> *bsaEntries) {
	HANDLE bsa = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(bsa == INVALID_HANDLE_VALUE) return;

	DWORD hashOffset, numFiles, numberOfBytesRead;

	if(	ReadFile(bsa, &hashOffset, sizeof(DWORD), &numberOfBytesRead, NULL) == NULL ? true : numberOfBytesRead != sizeof(DWORD) || hashOffset != 0x100 ? true :
		ReadFile(bsa, &hashOffset, sizeof(DWORD), &numberOfBytesRead, NULL) == NULL ? true : numberOfBytesRead != sizeof(DWORD) ? true :
		ReadFile(bsa, &numFiles,   sizeof(DWORD), &numberOfBytesRead, NULL) == NULL ? true : numberOfBytesRead != sizeof(DWORD) ? true : false) {
		CloseHandle(bsa);
		return;
	}
	DWORD dwBufferSize = numFiles * 2 * sizeof(DWORD);
	DWORD dwOffset = 3 * sizeof(DWORD);

	LPDWORD cBufferSize = new DWORD[numFiles<<1];
	PINT64  cBufferHash = new INT64[numFiles];

	if( SetFilePointer(bsa, dwOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER ? true :
		ReadFile(bsa, cBufferSize,  dwBufferSize, &numberOfBytesRead, NULL) == NULL ? true : numberOfBytesRead != dwBufferSize ? true :
		SetFilePointer(bsa, dwOffset += hashOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER ? true :
		ReadFile(bsa, cBufferHash,  dwBufferSize, &numberOfBytesRead, NULL) == NULL ? true : numberOfBytesRead != dwBufferSize ? true : false) {
		delete[] cBufferSize;
		delete[] cBufferHash;
		CloseHandle(bsa);
		return;
	}
	dwOffset += dwBufferSize;

	for(DWORD i = 0; i < numFiles; i++) {
		BSAEntry entry;
		entry.file = bsa;
		entry.size = cBufferSize[i<<1];
		entry.position = cBufferSize[(i<<1) + 1] + dwOffset;		
		(*bsaEntries)[cBufferHash[i]] = entry;
	}
	delete[] cBufferSize;
	delete[] cBufferHash;
}

void FindAllBSA(const char* startPath, std::map<__int64, BSAEntry> *bsaEntries) {
	char path[MAX_PATH];
	WIN32_FIND_DATA data;
	strcpy_s(path, MAX_PATH, startPath);
	strcat_s(path, MAX_PATH, "*.bsa");
	HANDLE hBSA = FindFirstFile(path, &data);	// TODO: add sorting by date for statics bsa and bsa list from Morrowind.ini for datafiles bsa.
	if(hBSA == INVALID_HANDLE_VALUE) return;
	do {
		strcpy(path, startPath);
		strcat(path, data.cFileName);
		OpenBSA(path, bsaEntries);
	} while(FindNextFile(hBSA, &data));
	FindClose(hBSA);
}

void InitBSAs() {
	if(BSAEntries.size() > 0 || BSAEntriesStatics.size() > 0) return;
	InitializeCriticalSection(&cache_crit_sec);
	FindAllBSA(DATA_FILES, &BSAEntries);
	FindAllBSA(DL_STATICS, &BSAEntriesStatics);
}

static RetData GetBSAEntry(BSAHash3 hash, std::map<__int64, BSAEntry> *bsaEntries) {
	std::map<__int64, BSAEntry>::const_iterator it = bsaEntries->find(hash.LValue);
	if(it == bsaEntries->end()) return RetData(0,0);
	BSAEntry entry = it->second;
	OVERLAPPED overlapped = {0};
	overlapped.Offset = entry.position;
	char *buf = new char[entry.size];
	DWORD read;
	ReadFile(entry.file, buf, entry.size, &read, &overlapped);
	if(read != entry.size) { delete[] buf; return RetData(0,0); }
	return RetData(buf, entry.size);
}

IDirect3DTexture9* SynchronizeTexturesCache(BSAHash3 hash, IDirect3DTexture9 *texture) {
	IDirect3DTexture9* tex = texture;
	std::map<__int64, IDirect3DTexture9*>::const_iterator it;
	//------------------------------
	EnterCriticalSection(&cache_crit_sec);
	it = BSALoadedTextures.find(hash.LValue);
	if(it != BSALoadedTextures.end()) tex = it->second;
	else if(texture) BSALoadedTextures[hash.LValue] = texture;
	LeaveCriticalSection(&cache_crit_sec);
	//------------------------------
	if(tex != texture) {
		tex->AddRef();
		if(texture)
			texture->Release();
	}
	return tex;
}

static IDirect3DTexture9* BSALoadTexture2(IDirect3DDevice9* device, const char* filename, int skipMipLevels) {
	HANDLE file;
	D3DFORMAT format;
	D3DXIMAGE_INFO info;
	char pathbuf[MAX_PATH];
	int div = (1<<skipMipLevels);
	IDirect3DTexture9* tex = NULL;
	BSAHash3 hash = HashString(filename);

	//First check if the texture is already loaded
	if(tex = SynchronizeTexturesCache(hash, NULL)) return tex;

	//Next check the distant land folder
	strcpy(pathbuf, DL_STATICS);
	strcat_s(pathbuf, MAX_PATH, filename);
	file=CreateFile(pathbuf, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(file!=INVALID_HANDLE_VALUE) {
		CloseHandle(file);
		if(!FAILED(D3DXCreateTextureFromFile(device, pathbuf, &tex))) {
			//**/D3DXCreateTextureFromFileEx(device, pathbuf, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 0, 0, &tex)
			return SynchronizeTexturesCache(hash, tex); //Want to cache this even if the texture load failed;
		} else tex=0;
	}

	//Then check the DistantLand Statics BSAs
	RetData rd = GetBSAEntry(hash, &BSAEntriesStatics);
	if(rd.ptr) {
		D3DXCreateTextureFromFileInMemoryEx(device, rd.ptr, rd.size, 0, 0, D3DX_FROM_FILE, 0, D3DFMT_UNKNOWN,
					D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &tex);
		delete[] rd.ptr;
		return SynchronizeTexturesCache(hash, tex); //Want to cache this even if the texture load failed;
	}

	//Then check the normal folder
	strcpy(pathbuf, DATA_FILES);
	strcat_s(pathbuf,MAX_PATH,filename);
	file=CreateFile(pathbuf, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(file!=INVALID_HANDLE_VALUE) {
		CloseHandle(file);
		if((skipMipLevels!=0) && (!FAILED(D3DXGetImageInfoFromFile(pathbuf, &info)))) {
			if(info.Format == D3DFMT_DXT1 || info.Format == D3DFMT_X8R8G8B8) format = D3DFMT_DXT1;
			else format = D3DFMT_DXT3;
			if(!FAILED(D3DXCreateTextureFromFileEx(device, pathbuf, info.Width/div, info.Height/div,
				D3DX_DEFAULT, 0, format, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &tex))) {
				return SynchronizeTexturesCache(hash, tex); //Want to cache this even if the texture load failed;
			} else tex=0;
		}
		if(!FAILED(D3DXCreateTextureFromFile(device, pathbuf, &tex))) {
			//**/D3DXCreateTextureFromFileEx(device, pathbuf, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 0, 0, &tex)
			return SynchronizeTexturesCache(hash, tex); //Want to cache this even if the texture load failed;
		} else tex=0;
	}

	//Finally check the BSAs
	rd = GetBSAEntry(hash, &BSAEntries);
	if(rd.ptr) {
		if((skipMipLevels!=0) && (!FAILED(D3DXGetImageInfoFromFileInMemory(rd.ptr, rd.size, &info)))) {
			if (rd.size>6 && ((char*)rd.ptr)[1]==0 && (((char*)rd.ptr)[2]==2 || ((char*)rd.ptr)[2]==10)) ((char*)rd.ptr)[5]=((char*)rd.ptr)[6]=0;
			if(info.Format == D3DFMT_DXT1 || info.Format == D3DFMT_X8R8G8B8) format = D3DFMT_DXT1;
			else format = D3DFMT_DXT3;
			if(!FAILED(D3DXCreateTextureFromFileInMemoryEx(device, rd.ptr, rd.size, info.Width/div, info.Height/div,
				D3DX_DEFAULT, 0, format, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &tex))) {
				delete[] rd.ptr;
				return SynchronizeTexturesCache(hash, tex); //Want to cache this even if the texture load failed;
			} else tex=0;
		}
		D3DXCreateTextureFromFileInMemoryEx(device,rd.ptr, rd.size,0,0,D3DX_FROM_FILE,0,D3DFMT_UNKNOWN,
					D3DPOOL_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&tex);
   //**/D3DXCreateTextureFromFileInMemoryEx(device,rd.ptr, rd.size,0,0,0,0,D3DFMT_UNKNOWN, D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&tex)
		delete[] rd.ptr;
		return SynchronizeTexturesCache(hash, tex); //Want to cache this even if the texture load failed;
	}

	//File not found
	return 0;
}

IDirect3DTexture9* BSALoadTexture(IDirect3DDevice9* device, const char* filename, int skipMipLevels) {
	char fname[MAX_PATH];
	strcpy(fname, "textures\\");
	strcat(fname, filename);
	// try loading dds first
	DWORD len=strlen(fname);
	if(fname[len-1]!='s'||fname[len-2]!='d'||fname[len-3]!='d') {
		char fname2[MAX_PATH];
		strcpy(fname2, fname);
		fname2[len-1]='s';
		fname2[len-2]='d';
		fname2[len-3]='d';
		IDirect3DTexture9* ret=BSALoadTexture2(device, fname2, skipMipLevels);
		if(ret) return ret;
	}
	return BSALoadTexture2(device, fname, skipMipLevels);
}

IDirect3DTexture9* BSALoadTexture(const char* filename) {
	return BSALoadTexture(RealDevice, filename, MipSkip);
}

void ClearBSATextureCache() {
	BSALoadedTextures.clear();
}

void ClearTexturesCache() {
	std::map<__int64, IDirect3DTexture9*>::const_iterator it;
	for(it = BSALoadedTextures.begin(); it != BSALoadedTextures.end(); ++it) {
		if(it->second)
			it->second->Release();
	}
	ClearBSATextureCache();
}

RetData GetNif(const char *filename) {
	char name[MAX_PATH] = "meshes\\";
	RetData rd(0,0);
	strlwr(strncat(name, filename, MAX_PATH));
//
//
//
	BSAHash3 hash = HashString(name);
	rd = GetBSAEntry(hash, &BSAEntries);
	return rd;
}

void BSACacheStats(int *total, int *memuse) {
    __int64 texMemUsage = 0;

    for(std::map<__int64, IDirect3DTexture9*>::const_iterator i = BSALoadedTextures.begin(); i != BSALoadedTextures.end(); ++i)
    {
        D3DSURFACE_DESC texdesc;
        i->second->GetLevelDesc(0, &texdesc);

        int bpp = 32;
        if(texdesc.Format == D3DFMT_DXT1) bpp = 4;
        if(texdesc.Format == D3DFMT_DXT3 || texdesc.Format == D3DFMT_DXT5) bpp = 8;

        texMemUsage += (texdesc.Width * texdesc.Height * bpp / 8) * 4 / 3;
    }

    *total = BSALoadedTextures.size();
    *memuse = (int)(texMemUsage / 1048576.0);
}
