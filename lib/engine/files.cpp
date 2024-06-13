char *getFileData_mustFree(const char *path, long int *fileSizeOut){

	FILE *fileHandle = fopen(path, "rb");

	if(fileHandle == NULL){
		printf("COULD NOT READ FILE: %s\n", path);
        
		return NULL;
	}

	fseek(fileHandle, 0, SEEK_END);
	long fileSize = ftell(fileHandle);
	fseek(fileHandle, 0, SEEK_SET);

	char *data = (char *)malloc(fileSize + 1);
	fread(data, fileSize, 1, fileHandle);
	fclose(fileHandle);

	data[fileSize] = 0;

	*fileSizeOut = fileSize;


	return data;

}

String *getBufferLines_mustFree(char *data, long int dataSize, int *numberOfLines_out){

	if(data == NULL){
		return NULL;
	}
	
	int numberOfLines = 1;
	for(int i = 0; i < dataSize; i++){
		if(data[i] == *"\n"){
			numberOfLines++;
		}
	}

	String *lines = (String *)malloc(numberOfLines * sizeof(String));
	memset(lines, 0, numberOfLines * sizeof(String));

	int currentLine = 0;
	int currentChar = 0;
	for(int i = 0; i < dataSize; i++){

		if(data[i] == *"\n"){
			if(lines[currentLine][currentChar - 1] == *"\r"){
				lines[currentLine][currentChar - 1] = *"\0";
			}
			currentLine++;
			currentChar = 0;
		}else{
			if(currentChar < STRING_SIZE){
				lines[currentLine][currentChar] = data[i];
			}
			currentChar++;
		}

	}

	//free(data);

	*numberOfLines_out = numberOfLines;

	return lines;


}

String *getFileLines_mustFree(const char *path, int *numberOfLines_out){

	long int dataSize;

	char *data = getFileData_mustFree(path, &dataSize);

	String *lines = getBufferLines_mustFree(data, dataSize, numberOfLines_out);

	free(data);

	return lines;

}

void writeDataToFile(const char *path, char *data_p, long int fileSize){

	FILE *fileHandle = NULL;

	fileHandle = fopen(path, "w");

	if(fileHandle == NULL){
		fprintf(stderr, "File Write Error: %s\n", strerror(errno));
	}

	for(int i = 0; i < fileSize; i++){
		fputc(data_p[i], fileHandle);
	}
	
	fclose(fileHandle);

}

#ifdef __ANDROID__
String *getDirectoryFileNames_mustFree(const char *path, int *output_n_files, AssetManager *assetManager_p){

    AAssetDir *directory_p = AAssetManager_openDir(assetManager_p, path);

    int n_files = 0;

    const char *fileNameConst = AAssetDir_getNextFileName(directory_p);

    while(fileNameConst != NULL){
        n_files++;
        fileNameConst = AAssetDir_getNextFileName(directory_p);
    }

    String *fileNames = (String *)malloc(sizeof(String) * n_files);

    AAssetDir_rewind(directory_p);

    for(int i = 0; i < n_files; i++){
        fileNames[i] = string(AAssetDir_getNextFileName(directory_p));
    }

    *output_n_files = n_files;

    return fileNames;

}
#else
String *getDirectoryFileNames_mustFree(const char *path, int *output_n_files, AssetManager *assetManager_p){

    DIR *dataDir = opendir(path);
    struct dirent* dirEntry;

    if(dataDir == NULL){
        printf("Could not read directory %s\n", path);
        return NULL;
    }

    int n_files = 0;

    while((dirEntry = readdir(dataDir)) != NULL){
        if(strcmp(dirEntry->d_name, ".") != 0
           && strcmp(dirEntry->d_name, "..") != 0){
            n_files++;
        }
    }

    closedir(dataDir);

    String *fileNames = (String *)malloc(n_files * sizeof(String));

    dataDir = opendir(path);

    int n = 0;

    while((dirEntry = readdir(dataDir)) != NULL){
        if(strcmp(dirEntry->d_name, ".") != 0
           && strcmp(dirEntry->d_name, "..") != 0){
            fileNames[n] = string(dirEntry->d_name);
            n++;
        }
    }

    closedir(dataDir);

    *output_n_files = n_files;

    return fileNames;

}
#endif

#ifdef __ANDROID__
char *getAssetData_mustFree(const char *path, long int *fileSizeOut, AssetManager *assetManager_p){

    AAsset *asset_p = AAssetManager_open(assetManager_p,path,AASSET_MODE_BUFFER);

    if(asset_p == NULL){
        printf("COULD NOT READ FILE: %s\n", path);
        return NULL;
    }

    long int fileSize = AAsset_getLength(asset_p);
    char *buffer = (char *)AAsset_getBuffer(asset_p);

    char *data = (char *)malloc(fileSize + 1);

    memcpy(data, buffer, fileSize);

    data[fileSize] = 0;

    *fileSizeOut = fileSize;

    AAsset_close(asset_p);

    return data;

}
#else
char *getAssetData_mustFree(const char *path, long int *fileSizeOut, AssetManager *assetManager_p){
	return getFileData_mustFree(path, fileSizeOut);
}
#endif

String *getAssetLines_mustFree(const char *path, int *numberOfLines_out, AssetManager *assetManager_p){

	long int dataSize;

	char *data = getAssetData_mustFree(path, &dataSize, assetManager_p);

	String *lines = getBufferLines_mustFree(data, dataSize, numberOfLines_out);

	free(data);

	return lines;

}
