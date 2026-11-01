#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_INPUTS = 5;

int main(void) {
  char *history[MAX_INPUTS];
  int count = 0;

  // history loop
  while (1) {
    char *input = NULL;
    size_t size = 0;

    // read input
    printf("Enter input: ");
    if (getline(&input, &size, stdin) == -1) {
      free(input);
      break;
    }

    // trim newline, for print control and strcmp()
    input[strcspn(input, "\n")] = '\0';

    // ignore empty case (optional)
    if (strcmp(input, "") == 0)
      continue;

    // adjust history
    if (count < MAX_INPUTS) { // history not full yet
      history[count] = strdup(input);
      count++;

    } else { // history is full, shift array
      free(history[0]);
      for (int i = 0; i < MAX_INPUTS - 1; i++) {
        history[i] = history[i + 1];
      }
      history[MAX_INPUTS - 1] = strdup(input);
    }

    // print condition
    if (strcmp(input, "print") == 0) {
      for (int i = 0; i < count; i++) {
        printf("%s\n", history[i]);
      }
    }

    // clear condition (optional)
    if (strcmp(input, "clear") == 0) {
      for (int i = 0; i < count; i++) {
        free(history[i]);
      }
      count = 0;
    }

    free(input);
  }

  return 0;
}
