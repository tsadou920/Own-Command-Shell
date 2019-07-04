/*
   Liam Boylan, Zohora Noor, Fayrouz Micheal, Thierno Diallo
   ossh (Operating Systems Shell) Shell Project
   Main File

   Features: shell (lists commands), history* (print last 5
   executed commands), cd* (prints the path of the parent
   directory), clear* (clears the terminal screen and lists files),
   exit* (exits the terminal), catches interrupt (ctrl+c) and outputs message

   Procedure:
   Compile the other 5 ossh_ files first	(gcc ossh_cd.c -o ossh_cd)
   Compile this one				(gcc ossh.c -o ossh)
   Run						(./ossh)

   Note: Each ossh_ file must be compiled first before
   running the main ossh executable.

   Note: All executables must be in the same directory.

   Citations:
   https://brennan.io/2015/01/16/write-a-shell-in-c/	(Helpful tips)
*/

#include <stdio.h>		// For io operations.
#include <unistd.h>		// For fork, pipe, read, write.
#include <stdlib.h>		// For size_t, free.
#include <sys/wait.h>		// For waitpid.
#include <string.h>		// For string functions.
#include <signal.h>		// For the signal handler.

#define MAX_DIR_LEN 1024	// Max length of current directory path.

char shellDir[MAX_DIR_LEN];	// Directory of the shell's executables,
				// captured at launch.

// All command functions return 0 if the exit-command was successful or
// any errors with the other commands, return 1 othrewise.

// Shell command.
int shellCmd() {
	char* args[] = {"ossh_shell", NULL};	// Args for exec func.
	int status;
	pid_t pid, wPid;
	
	pid = fork();	// Create child process.
		
	if (pid == -1) {	// Error with fork.
		perror("fork");
		return 0;
	} else if (pid == 0) {	// Child process.
		char tempDir[MAX_DIR_LEN];	// Stores the directory of the shell command.
		strcpy(tempDir, shellDir);	// Copies the directory containing the command files.
		strcat(tempDir, "/ossh_shell");		// Appends the name of the command.
		
		execvp(tempDir, args);	// Run shell-command file.
		printf("Execution failed\n");	// Error if failed.
		return 0;
	} else {		// Parent process.
		wPid = waitpid(pid, NULL, 0);	// Waits for command child
						// to terminate.
		if (wPid == -1) {	// Error with waitpid.
			perror("waitpid");
			return 0;
		}

		return 1;
	}
}

// Exit command.
int exitCmd() {
	char* args[] = {"ossh_exit", NULL};
	int status;
	pid_t pid, wPid;
	int fd[2];	// File descriptor for pipe.
	
	// Message to send through pipe.
	char message[] = "Returning control to original shell...";

	if (pipe(fd) == -1) {	// Pipe, message if error.
		perror("pipe");
		return 0;
	}

	pid = fork();
		
	if (pid == -1) {
		perror("fork");
		return 0;
	} else if (pid == 0) {		// Child process.
		close(fd[0]);	// Close read end of pipe.
		// Write message to pipe.
		write(fd[1], message, strlen(message) + 1);
		close(fd[0]);	// Close write end of pipe.

		char tempDir[MAX_DIR_LEN];	// Stores the directory of the command file.
		strcpy(tempDir, shellDir);	// Copies the directory of all the command files.
		strcat(tempDir, "/ossh_exit");		// Appends the name of the command.
		
		execvp(tempDir, args);	// Executes exit-command file.
		printf("Execution failed\n");
		return 0;
	} else {		// Parent process.
		wPid = waitpid(pid, NULL, 0);
		
		if (wPid == -1) {
			perror("waitpid");
			return 0;
		}

		close(fd[1]);		// Closes write end of pipe.
		char buff[100];		// Buffer.
		read(fd[0], buff, 100);		// Reads from pipe.
		close(fd[0]);		// Closes read end of pipe.
		printf("%s\n", buff);	// Prints to the screen the message
			// acquired from the pipe after the exit-command is successfully
			// ran but before the shell is terminated.

		return 0;	// Returns 0 on success to terminate the shell.
	}
}

// History command.
int historyCmd(char *history[], int number) {
	int status;
	pid_t pid, wPid;
	char argNum[50];	// Stores typecast number var.

	sprintf(argNum, "%d", number);	// Typecast number var from int to char*
				// [ (char *) number isn't possible ].
	pid = fork();	// Create new process.
		
	if (pid == -1) {	// Fork error;
		perror("fork");
		return 0;
	} else if (pid == 0) {	// Child process.
		char tempDir[MAX_DIR_LEN];	// Stores the path of the history-command file.
		strcpy(tempDir, shellDir);	// Copies the directory containing the command file.
		strcat(tempDir, "/ossh_history");	// Appends the name of the file.
		
		// Executes the history command file and sends arguments one at a time.
		// argNum is commandCounter, each command in the history array is passed
		// one at a time.
		execl(tempDir, "ossh_history", argNum, history[0],
				history[1], history[2], history[3], history[4], (char *)NULL);
		printf("Execution failed\n");	// Error message for execl.
		return 0;
	} else {		// Parent process.
		wPid = waitpid(pid, NULL, 0);	// Waits for the child to end.
			
		if (wPid == -1) {	// Error message for waitpid.
			perror("waitpid");
			return 0;
		}

		return 1;
	}
}

// Cd command.
int cdCmd() {
	char* args[] = {"ossh_cd", NULL};	// Arguments for execvp.
	int status;
	pid_t pid, wPid;

	// Changes the directory in the parent process to remain in the
	// parent directory.
	// If we changed the directory in the child process (cd command file)
	// then it would only be temporary.
	chdir("..");

	pid = fork();	// Create child process.
		
	if (pid == -1) {	// Error with fork.
		perror("fork");
		return 0;
	} else if (pid == 0) {		// Child process.
		char tempDir[MAX_DIR_LEN];	// Stores the path of the cd-command file.
		strcpy(tempDir, shellDir);	// Copies the path containing the command file.
		strcat(tempDir, "/ossh_cd");	// Appends the name of the cd command file.
		
		execvp(tempDir, args);		// Executes cd.
		printf("Execution failed\n");	// Prints if execvp error.
		return 0;
	} else {		// Parent process.
		wPid = waitpid(pid, NULL, 0);	// Waits for child to end.
		
		if (wPid == -1) {	// Waitpid error message.
			perror("waitpid");
			return 0;
		}

		return 1;
	}
}

// Clear command.
int clearCmd() {
	char* args[] = {"ossh_clear", NULL};
	int status;
	pid_t pid, wPid;

	pid = fork();
		
	if (pid == -1) {
		perror("fork");
		return 0;
	} else if (pid == 0) {		// Child process.
		char tempDir[MAX_DIR_LEN];
		strcpy(tempDir, shellDir);
		strcat(tempDir, "/ossh_clear");
		
		execvp(tempDir, args);
		printf("Execution failed\n");
		return 0;
	} else {		// Parent process.
		wPid = waitpid(pid, NULL, 0);
		
		if (wPid == -1) {
			perror("waitpid");
			return 0;
		}

		return 1;
	}
}

// Signal handler to print special message when keyboard interrupt (ctrl+c)
// is pressed.
void sighandler(int signal) {
	printf("\nSignal %d was caught\n", signal);
	printf("Liam Boylan\nZohora Noor\nFayrouz Micheal\nThierno Diallo\n");
	printf("Shell terminated by interrupt signal\n");
	
	exit(1);
}

int main() {
	int keepGoing = 1;		// Loop condition.
	char *line = NULL;		// Stores command from user.
	size_t numOfChars, len = 0;	// Length of command.
	char *histArray[5];		// Array for command history.
	int commandCounter = 0;		// Count the number of commands.

	// Initialize each element in the array with NULL.
	for (int i = 0; i < 5; i++)
		histArray[i] = NULL;

	// Captures the initial directory for use later with executing
	// the commands in the same directory.
	getcwd(shellDir, MAX_DIR_LEN);

	signal(SIGINT, sighandler);	// Signal handler for interrupt.

	printf("Type \"shell\" for more info.\n");

	// Main control loop.
	while (keepGoing) {
		char dir[MAX_DIR_LEN];		// To store the current working directory (path).
		getcwd(dir, MAX_DIR_LEN);	// Gets path to print at prefix.
		printf("%s # ", dir);		// Command prompt.
		numOfChars = getline(&line, &len, stdin);	// Stores line
								// of user input.
		strtok(line, "\n");	// Gets rid of newline char from line.

		free(histArray[commandCounter]);	// Sets NULL to free so no
						// operation will be performed.
		histArray[commandCounter] = strdup(line);	// Stores the
						// command into the history array.
		commandCounter = (commandCounter + 1) % 5;	// Keeps command counter
								// between 0 and 4;

		// Command switch statement.
		if (strcmp(line, "\n") == 0)	// New command prompt when nothing is
			;			// entered.
		else if (strcmp(line, "shell") == 0)	// Shell command.
			keepGoing = shellCmd();
		else if (strcmp(line, "history*") == 0)	// History command.
			keepGoing = historyCmd(histArray, commandCounter);
		else if (strcmp(line, "cd*") == 0)	// Cd command.
			keepGoing = cdCmd();
		else if (strcmp(line, "clear*") == 0)	// Clear command.
			keepGoing = clearCmd();
		else if (strcmp(line, "exit*") == 0)	// Exit command.
			keepGoing = exitCmd();
		else	printf("Command not found\n");	// Other cases.
	}

	// De-allocate memory.
	free(line);
	for (int i = 0; i < 5; i++) {
		free(histArray[i]);
		histArray[i] = NULL;
	}

	return 0;
}
