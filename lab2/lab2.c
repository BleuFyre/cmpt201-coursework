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
    if (getline(&command, &size, stdin) == -1) { // error handling
                                                 // how to trim newline?
      free(command);
      break;
    }

    pid_t pid = fork();
    if (pid == 0) {
      // child process
      execlp(command, command, NULL); // command replaces child
                                      // possible error from execlp?
    } else {
      // parent process
      int status = 0;
      if (waitpid(pid, &status, 0) == -1) { // wait for child
        perror("waitpid");
        exit(EXIT_FAILURE);
      }

      if (WIFEXITED(status)) { // check exit status
        printf("Child exited with status: %d\n", WEXITSTATUS(status));
      } else {
        printf("Child did not exit normally.\n");
      }
    }

    free(command);
  }

  return 0;
}
