/*
   Liam Boylan, Zohora Noor, Fayrouz Micheal, Thierno Diallo
   ossh Shell Project
   History Command

   Prints the last 5 commands from the history string array.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int number = atoi(argv[1]);	// Gets commandcounter from arguments.
	char *history[5];

	for (int i = 0; i < 5; i++)
		history[i] = NULL;

	for (int i = 0; i < argc-2; i++)
		history[i] = argv[i+2];		// Gets history array from arguments.

	int i = number;		// Iterator for one pass through history array.
	int histNum = 1;	// Number associated with age of command.
	
	do {
		if (history[i]) {	// Command.
			printf("%d %s\n", histNum, history[i]);	// Command
					// and the number associated with it.
			histNum++;
		}

		i = (i + 1) % 5;	// Keep iterator between 0 and 4.
	} while (i != number);		// Runs at most 5 times.

	return 1;
}
