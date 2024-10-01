#include <stdio.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc < 4 || argc > 4){
		printf("Please only pass in three parameters");
		return 0;
	}


	/*First we want to make the directory we will write to.*/
	const char* dir = argv[1];
	const char* sourceFile = argv[2];
	const char* newFile = argv[3];

	int dir_len = strlen(dir);
	int new_file_len = strlen(newFile);
	char dirCopy[dir_len];
	char newFileCopy[new_file_len];

	strcpy(dirCopy, dir);
	strcpy(newFileCopy, newFile);


	if(dirCopy[dir_len] != '/'){
		/*appending / to the end of the dir*/
		dirCopy[dir_len] = '/';
		dirCopy[dir_len + 1] = '\0';
	};
	char* dir_and_new_file = (char*)calloc(dir_len + new_file_len + 1, sizeof(char));
	strcpy(dir_and_new_file, dirCopy);
	strcat(dir_and_new_file, newFileCopy);
	

	mkdir(dir, 0777); /*allow all users on the PC to be able to read, write, exec*/

	/*concat absolute path and file name to open the file in the new directory and then create if it DNE with 0777 rwx for all*/
	int file = open(dir_and_new_file, O_RDONLY | O_CREAT, 0777); /*open to read only*/
	
	if (file == -1){
		printf("Error num: %d\n", errno);
		perror("Program");
	};
	close(file);
	
	/*cache the contents of the file into the buffer and change the 1's to A and add XYZ to end of that chunk. Do this in chunks of 100*/
	char* char_buff = (char*)calloc(100, sizeof(char));
	int source_f = open(sourceFile, O_RDWR);
	int new_f = open(dir_and_new_file, O_WRONLY);
	size_t bytes_read;
	size_t write_buff;

	if(source_f == -1){
		printf("the file we are trying to find is: %s\n", sourceFile);
		perror("Failed to open file");
		exit(1);
	};
	if(new_f == -1){
		printf("the fuile we are trying to find but failed is: %s\n", newFile);
		perror("Failed to open file");
		exit(1);
	};

	while ((bytes_read = read(source_f, char_buff, 100)) > 0) {
		char_buff[bytes_read] = '\0'; /*add null character to end of chunk*/

		printf("chunk read: %s\n", char_buff);

		for(int i = 0; i < strlen(char_buff) ; i++){
			if(char_buff[i] == '1'){ char_buff[i] = 'A'; }
		};

		write_buff = write(new_f, char_buff, strlen(char_buff)); /*write the string batch to the file.*/
		write_buff = write(new_f, "XYZ", strlen("XYZ")); /*after each batch written write the XYZ*/
	};

	close(source_f);
	close(new_f);
	
	return 0;
}
