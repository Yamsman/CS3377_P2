#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

//Recursively counts the number of directories, files, and file size for all subfolders
void dirtally_r(DIR *dir, char *pname, int *fcount, int *dcount, int *tsize) {
	//Read all entries in the directory
	int dn_len = -1;
	char *dname = NULL;
	struct dirent *ent;
	while (ent = readdir(dir)) {
		//Concatenate the parent folder's name with the dir name
		//Only resize the buffer if needed
		int len = strlen(pname) + strlen(ent->d_name) + 2;
		if (len > dn_len) {
			free(dname);
			dname = malloc(len);
			dn_len = len;
		}
		snprintf(dname, len, "%s/%s", pname, ent->d_name);

		//Stat the file
		struct stat st;
		if (stat(dname, &st)) continue;

		//Check the entry's type
		if (S_ISREG(st.st_mode)) {
			//Get the size of the file
			struct stat st;
			int res = stat(dname, &st);
			if (!res)
				(*tsize) += st.st_size;
			(*fcount)++;
		} else if (S_ISDIR(st.st_mode)) {
			//Skip . and ..
			if (!strcmp(ent->d_name, "."))  continue;
			if (!strcmp(ent->d_name, "..")) continue;

			//Call the function recursively for the directory
			DIR *newdir = opendir(dname);
			if (newdir != NULL) {
				dirtally_r(newdir, dname, fcount, dcount, tsize);
				closedir(newdir);
			}
			(*dcount)++;
		}
	}
	return;
}

int main(int argc, char **argv) {
	//Check number of arguments
	if (argc != 2) {
		printf("%s: Invalid number of arguments\n", argv[0]);
		return -1;
	}

	//Open directory
	DIR *dir = opendir(argv[1]);
	if (dir == NULL) {
		printf("%s: Unable to open %s: ", argv[0], argv[1]);
		fflush(stdout);
		perror("");
		return -1;
	}

	//Count the number of files, directories, and the total size of all files
	int f_count = 0;
	int d_count = 0;
	int t_size = 0;
	dirtally_r(dir, argv[1], &f_count, &d_count, &t_size);

	//Print the results
	printf("The total number of directories in directory %s is: %i\n", argv[1], d_count);
	printf("The total number of files in directory %s is: %i\n", argv[1], f_count);
	printf("The total number of bytes occupied by all files "
		"in directory %s is: %i\n", argv[1], t_size);

	closedir(dir);
	return 0;
}
