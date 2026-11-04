#define _DEFAULT_SOURCE

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXTRA_SIZE 256
#define BLOCK_SIZE 128
#define BUF_SIZE 128
#define HEADER_SIZE 16

struct header {
  uint64_t size;       // 64 bits = 8 bytes
  struct header *next; // 64 bit system, so ptr is 8 bytes
};

void print_out(char *format, void *data, size_t data_size) { // provided print
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    perror("snprintf failed");
    exit(EXIT_FAILURE);
  }
  write(STDOUT_FILENO, buf, len);
}

int main(void) {
  // increase heap size by 256 bytes
  void *start_address = sbrk(EXTRA_SIZE);
  if (start_address == (void *)-1) {
    perror("sbrk failed");
    exit(EXIT_FAILURE);
  }

  // initialize headers
  struct header *first_block = (struct header *)start_address;
  first_block->size = BLOCK_SIZE;
  first_block->next = NULL;
  struct header *second_block = (void *)start_address + BLOCK_SIZE;
  second_block->size = BLOCK_SIZE;
  second_block->next = first_block;

  // print starting addresses
  print_out("first block:       %p\n", &first_block, sizeof(first_block));
  print_out("second block:      %p\n", &second_block, sizeof(second_block));

  // print headers
  print_out("first block size:  %lu\n", &first_block->size, sizeof(first_block->size));
  print_out("second block size: %lu\n", &second_block->size, sizeof(second_block->size));
  print_out("first block next:  %p\n", &first_block->next, sizeof(first_block->next));
  print_out("second block next: %p\n", &second_block->next, sizeof(second_block->next));

  // initialize data
  void *first_block_data = (void *)first_block + HEADER_SIZE;
  void *second_block_data = (void *)second_block + HEADER_SIZE;
  memset(first_block_data, 0, BLOCK_SIZE - HEADER_SIZE);
  memset(second_block_data, 1, BLOCK_SIZE - HEADER_SIZE);

  /*// print headers again to make sure initialization went well*/
  /*print_out("first block size: %lu\n", &first_block->size, sizeof(first_block->size));*/
  /*print_out("second block size: %lu\n", &second_block->size, sizeof(second_block->size));*/
  /*print_out("first block next: %p\n", &first_block->next, sizeof(first_block->next));*/
  /*print_out("second block next: %p\n", &second_block->next, sizeof(second_block->next));*/

  // print first block data
  for (int i = 0; i < (BLOCK_SIZE - HEADER_SIZE); i++) {
    char digit = ((char *)first_block_data)[i] + '0'; // gpt advice?
    write(STDOUT_FILENO, &digit, 1);
    write(STDOUT_FILENO, "\n", 1);
  }

  // print second block data
  for (int i = 0; i < (BLOCK_SIZE - HEADER_SIZE); i++) {
    char digit = ((char *)second_block_data)[i] + '0'; // gpt advice?
    write(STDOUT_FILENO, &digit, 1);
    write(STDOUT_FILENO, "\n", 1);
  }

  return 0;
}
