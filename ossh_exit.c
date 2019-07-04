/*
   Liam Boylan, Zohora Noor, Fayrouz Micheal, Thierno Diallo
   ossh Shell Project
   Exit Command

   Prints a list of all the team members, waits for the user to hit the
   return key, and prints a goodbye message.
*/

#include <stdio.h>

int main() {
	// List of team members.
	printf("Liam Boylan\nZohora Noor\nFayrouz Micheal\nThierno Diallo\n");

	// Wait for user to hit the return key.
	printf("Press ENTER to continue...");
	while(getchar() != '\n');

	// Silly text art.
	printf("   _____                 _ _                \n");
	printf("  / ____|               | | |               \n");
	printf(" | |  __  ___   ___   __| | |_  _   _  ___  \n");
	printf(" | | |_ |/ _ \\ / _ \\ / `  | ' \\| | | |/ _ \\ \n");
	printf(" | |__| |  () |  () |  (| | |) | |_| |  __/ \n");
	printf("  \\_____|\\___/ \\___/ \\__,_|_.__/\\__, |\\___| \n");
	printf("                                 __/ |      \n");
	printf("                                |___/       \n");

	return 0;
}
