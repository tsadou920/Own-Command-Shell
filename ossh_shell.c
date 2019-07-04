/*
   Liam Boylan, Zohora Noor, Fayrouz Micheal, Thierno Diallo
   ossh Shell Project
   Shell Command

   Prints a list of the available commands and brief descriptions.
*/

#include <stdio.h>

int main() {
	// List of commands.
	printf("Commands:\n");
	printf("shell\t\t\tLists custom commands.\n");
	printf("history*\t\tView the last 5 successfully executed commands.\n");
	printf("exit*\t\t\tPrints list of team members, waits for return key, outputs exit messages, then terminates the shell.\n");
	printf("cd*\t\t\tMoves to the parent directory, prints the path, then returns to the intial directory.\n");
	printf("clear*\t\t\tClears the terminal screen and prints a list of directories and files in the current directory.\n");

	return 1;
}
