#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

void display_prompt() {
    printf("SimpleShell> ");
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        display_prompt();

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            // "end of file" condition (Ctrl+D) or some error occurred in reading input
            printf("\n");
            break;
        }

        // Remove the newline character from the end of the command
        command[strcspn(command, "\n")] = '\0';

        pid_t pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("Fork error");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            execlp(command, command, NULL);
            // If the above line is executed, there was an error in executing the command
            perror("Command error");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0) {
                    printf("Command '%s' returned non-zero exit status: %d\n", command, exit_status);
                }
            } else {
                printf("Command '%s' did not terminate normally\n", command);
            }
        }
    }

    return 0;
}
