struct RetData {
	void* ptr;
	unsigned int size;

	RetData(void* _ptr, unsigned int _size) { ptr=_ptr; size=_size; }
};

void InitBSAs();
IDirect3DTexture9* BSALoadTexture(const char* filename);
IDirect3DTexture9* BSALoadTexture(IDirect3DDevice9* device, const char* filename, int skipMipLevels);
void ClearBSATextureCache();
void ClearTexturesCache();
RetData GetNif(const char *filename);
