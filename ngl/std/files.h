#define ReadFromFileVar(fp, var) fread(var, sizeof(*var), 1, fp)
#define ReadFromFileSize(fp, var, size) fread(var, size, 1, fp)
// #define ReadFromFileAmt(fp, var, amt) fread(var, sizeof(*var), amt, fp)
#define ReadFromFile(fp, var, size, amt) fread(var, size, amt, fp)

#define WriteOnFileVar(fp, var) fwrite(var, sizeof(*var), 1, fp)
#define WriteOnFileSize(fp, var, size) fwrite(var, size, 1, fp)
// #define WriteOnFileAmt(fp, var, amt) fwrite(var, sizeof(*var), amt, fp)
#define WriteOnFile(fp, var, size, amt) fwrite(var, size, amt, fp)

#define SeekFileOffset(fp, amt) fseek(fp, amt, SEEK_CUR);
#define SeekFileAbsolute(fp, amt) fseek(fp, amt, SEEK_SET);

#define OpenFileReadBinary(file) fopen(file, "rb")
#define OpenFileWriteBinary(file) fopen(file, "wb")
#define OpenFileWrite(file) fopen(file, "w")
#define CloseFile(fp) fclose(fp)

enum file_state{
	FILE_READ,
	FILE_CREATED,
	FILE_NOT_FOUND,
	FILE_CLOSED,
};

struct file{
	string Path;
	void* FileHandle; 
	file_state State;
};

bool FileExists(const char* path) {
	FILE* f = fopen(path, "r");
	if(f) { fclose(f); return true; }
	return false;
}
