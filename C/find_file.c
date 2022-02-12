/* This program lists all items in the specified directory then recursively
searches for all instances of the specified filename and prints those locations.

compile this program with: gcc -W -Wall -o find_file find_file.c
then run the program with: ./find_file . myFile.txt
to recursively search the current directory (.) for a file named myFile.txt */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>

void usage(int argc, char** argv);
void find_file(char* dir_name, char* file_to_find);

int main(int argc, char** argv)
{
	DIR* dp;
	struct dirent* dirp;

	// check if this application is being used properly
	usage(argc, argv);

	// check to see if provided directory is accessible
	errno = 0;
	dp = opendir(argv[1]);
	if(dp == NULL) {
		switch(errno) {
			case EACCES:
				fprintf(stderr, "Permission denied\n");
				break;
			case ENOENT:
				fprintf(stderr, "Directory does not exist\n");
				break;
			case ENOTDIR:
				fprintf(stderr, "'%s' is not a directory\n", argv[1]);
				break;	
		}
	}

	// print all files in the directory
	int cnt = 0;
	while((dirp = readdir(dp)) != NULL) {
		fprintf(stdout, "%d: %s", cnt, dirp->d_name);
		if(dirp->d_type == DT_DIR) {
			printf("\t directory");
		}
		printf("\n");
		cnt++;
	}

	// close the directory 
	closedir(dp);


	// now, recursivey traverse the directory structure to find the provided
	// file name
	char* file_to_find = argv[2];
	find_file(argv[1], file_to_find);

	return 0;
}


void usage(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: ./%s directory_name file_to_find\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

void find_file(char* dir_name, char* file_to_find)
{
	// first get a pointer to the specified directory
	DIR* dir_pointer;
	struct dirent* dirp;
	dir_pointer = opendir(dir_name);

	// guard against a null pointer
	if(dir_pointer != NULL) {

		// loop over each item in the directory
		while((dirp = readdir(dir_pointer)) != NULL) {

			// if the item is a directory, not . or .., then recurse
			if(dirp->d_type == DT_DIR && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
				
				// add directory prefix if not referencing from root directory
				if(dirp->d_name[0] != '.') {
					// prepend the parent directory to the current directory for recursion
					char next_directory_name[500] = {};
			       		strcat(next_directory_name, dir_name);
					strcat(next_directory_name, "/");
					strcat(next_directory_name, dirp->d_name);

					// pass the next directory to find_file for recursion
					find_file(next_directory_name, file_to_find);
				}
				// if referencing from root, pass directory directly to find_file
				else {					
					find_file(dirp->d_name, file_to_find);
				}
			}
			// otherwise, if the item is a file matching file_to_find, print that it's found
			else if(strcmp(dirp->d_name, file_to_find) == 0 && dirp->d_type == DT_REG) {
				fprintf(stdout, "Found %s in %s\n", file_to_find, dir_name);
			}
		}
	}

	// close the directory
	closedir(dir_pointer);
}
