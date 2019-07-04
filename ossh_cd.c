/*
   Liam Boylan, Zohora Noor, Fayrouz Micheal, Thierno Diallo
   ossh Shell Project
   Cd Command

   Prints the current working directory (path).
*/

#include <stdio.h>
#include <unistd.h>

#define MAX_DIR_LEN 1024

int main() {
	char cwd[MAX_DIR_LEN];	// To store current directory.

	// Prints current directory;
	getcwd(cwd, MAX_DIR_LEN);
	printf("%s\n", cwd);

	return 1;
}
