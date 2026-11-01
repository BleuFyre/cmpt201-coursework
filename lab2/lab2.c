#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  while (1) { // infinite loop
    char *command = NULL;
    size_t size = 0;

    printf("Enter programs to run.\n> ");
    if (getline(&command, &size, stdin) == -1) { // read line
      free(command); // "should be freed by the user program even on failure"
      break;
    }

    command[strcspn(command, "\n")] = '\0'; // remove newline character
    pid_t pid = fork();

    if (pid == 0) {                   // child process
      execlp(command, command, NULL); // command replaces child
      perror("execlp failed\n");
      exit(EXIT_FAILURE);

    } else { // parent process
      int status = 0;
      if (waitpid(pid, &status, 0) == -1) { // wait for child
        perror("waitpid failed\n");
        exit(EXIT_FAILURE);
      }

      if (WIFEXITED(status)) { // check exit status
        printf("Child exited with status: %d\n", WEXITSTATUS(status));
      } else {
        printf("Child did not exit normally.\n");
      }
    }

    free(command); // allocated by getline
  }

  return 0;
}
