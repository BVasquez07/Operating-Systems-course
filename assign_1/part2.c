
#include <string.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
int search (char* file, char* dir){
	DIR *dirptr=opendir(dir);
	printf("The directory is: %s\n", dir);

	if(dirptr == NULL){
		printf("Error with the directory\n");
		perror("opendir");
		return 1;
	};

	struct dirent *entry = readdir(dirptr);
	struct stat statbuf; 
	while (entry != NULL) {
		if (strlen(entry->d_name) == strlen(file) && (strcmp(entry->d_name,file) == 0)){
			stat(entry->d_name, &statbuf);
			printf("File size in bytes: %1ld\n", statbuf.st_size); /* print file size */
                        entry = readdir(dirptr);
                        closedir(dirptr);
                       	return 0; /*return success*/
		};
		entry = readdir(dirptr);/*read the curr entry (file or dir) and then move the cursor to point to the succeeding entry.*/
	};
	closedir(dirptr);
	return 1; /*return failure*/
}

int main(int argc, char *argv[]){
	if(argc < 2 || argc > 3) { //
		printf("Error in string parameters passed");
		return 1;
	}
	else{
		int out = search (argv[1], argv[2]); /*argv[0] holds the name of the program*/
		if (out == 0){
			printf("Found the file!");
		}
		else{
			printf("Couldn't find the file ;{");
		}
	
	};

}
