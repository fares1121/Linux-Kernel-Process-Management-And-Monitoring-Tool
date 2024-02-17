#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#define BUFFER_SIZE 128
#define MAX_PROCESSES 100
#define MAX_ARGS 20
#define MAX_PATH_LENGTH 200

int main() {
	char executable_paths[MAX_PROCESSES][MAX_PATH_LENGTH];
	char* args[MAX_PROCESSES][MAX_ARGS + 2]; // +2 for executable path and NULL terminator
	int num_processes;
	FILE *proc_file;
	
	// Get the number of processes the user want to launch
	printf("How many processes would you like to launch? (up to %d): \n", MAX_PROCESSES);
	scanf("%d", &num_processes);

	// If num_processes is out of range, display an error message and return 1
	if ((num_processes <= 0) || (num_processes > MAX_PROCESSES)) {
		printf("Invalid number of processes.\n");
		return 1;
	}

	printf("Please specifiy the executable paths and arguments for each process:\n");

	// Get executable paths and arguments for each process from user input
	for (int i = 0; i < num_processes; i++) {
		printf("Process %d:\n", (i + 1));
		printf("Executable path: \n");
		scanf("%s", executable_paths[i]);

		args[i][0] = strdup(executable_paths[i]); // Allocate memory for the executable path

		printf("Arguments (up to %d, enter 'done' to finish): \n", MAX_ARGS);

		for (int j = 1; j <= MAX_ARGS; j++) {
			char arg[MAX_PATH_LENGTH];
			scanf("%s", arg);

			if (strcmp(arg, "done") == 0) {
				args[i][j] = NULL;
				break;
			}
			
			args[i][j] = strdup(arg); // Allocate memory for each argument
		}	

	}

	// Launch processes
	for (int i = 0; i < num_processes; i++) {
		pid_t pid = fork();

		if (pid == -1) { // If fork() failed
			perror("fork");

			// Free allocated memory before exiting
			for (int j = 0; j <= MAX_ARGS; j++) {
				free(args[i][j]);
			}

			return 1;
		} else if (pid == 0) { // Child process
			
			// Write PID to /proc/pid
			if ((proc_file = fopen("/proc/pid", "w")) == NULL) {
				perror("fopen");

				// Free allocated memory before exiting
				for (int j = 0; j <= MAX_ARGS; j++) {
					free(args[i][j]);
				}

				exit(1);
			}
			
			// Add the PID of the forked process to the /proc/pid file
			fprintf(proc_file, "%d", getpid());
			// Close /proc/pid file
			fclose(proc_file);

			execv(executable_paths[i], args[i]);
			perror("execv"); // Only reaches here if execv fails

			// Free allocated memory before exiting
			for (int j = 0; j <= MAX_ARGS; j++) {
				free(args[i][j]);
			}

			exit(1);
		}

	}

	// Wait for all child processes to finish
	for (int i = 0; i < num_processes; i++) {
		int status;
		wait(&status);
		printf("Process %d exited with status %d\n", (i + 1), status);

		// Free allocated memory after process exits
		for (int j = 0; j <= MAX_ARGS; j++) {
			free(args[i][j]);
		}

	}

	return 0;
}
