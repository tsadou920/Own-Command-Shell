/*
   Liam Boylan, Zohora Noor, Fayrouz Micheal, Thierno Diallo
   ossh Shell Project
   Clear Command

   Clears the screen and prints the names of all the files and directories
   in the current directory.
*/

#include <stdio.h>
#include <dirent.h>

int main() {
	DIR *d;		// Directory stream.
	struct dirent *dir;	// To store directory info.
	d = opendir(".");	// Opens current directory.

	printf("\e[1;1H\e[2J");		// Clears the screen by printing
			// newlines until all the previous lines are gone.
	if (d) {
		while ((dir = readdir(d)) != NULL)	// Read next entry from directory.
			// Only prints the name of the entry if it is
			// a file or a directory.
			if (dir->d_type == DT_REG || dir->d_type == DT_DIR)
				printf("%s ", dir->d_name);

		closedir(d);	// Close directory.
	}

	printf("\n");

	return 1;
}
