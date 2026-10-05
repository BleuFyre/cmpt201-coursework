#define _POSIX 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_INPUTS = 5;

// print array function, when the input is 'print'
print_array() {}

// shift array function, when there are >5 inputs
shift_array() {}

int main(void) {
  while (1) { // infinite loop
    char *input = NULL;
    size_t size = 0;

    printf("Enter input: ");
    if (getline(&input, &size, stdin) == -1) { // error handling
      free(input);
      break;
    }
  }
}
