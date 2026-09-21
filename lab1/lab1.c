#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  while (1) { // infinite loop
    char *input = NULL;
    size_t size = 0;

    printf("Please enter some text: ");
    if (getline(&input, &size, stdin) == -1) { // error handling
      free(input);
      break;
    }

    printf("Tokens:\n");

    char *saveptr;
    char *token = strtok_r(input, " ", &saveptr);

    while (token != NULL) {
      printf("  %s\n", token);
      token = strtok_r(NULL, " ", &saveptr);
    }

    free(input);
  }
}
