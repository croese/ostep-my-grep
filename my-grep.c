#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void grep(char* needle, char* filename, FILE* fd)
{
  char* line = NULL;
  size_t n = 0;
  ssize_t nread = 0;

  while ((nread = getline(&line, &n, fd)) != -1) {
    if (strstr(line, needle)) {
      printf("%s: %s", filename, line);
    }
  }

  free(line);
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    printf("USAGE: %s <search-term> [file...]\n", argv[0]);
    exit(1);
  }

  char* needle = argv[1];
  if (argc == 2) {
    grep(needle, "stdin", stdin);
  } else {
    for (int i = 2; i < argc; i++) {
      FILE* fd = fopen(argv[i], "r");
      if (!fd) {
        perror("fopen");
        exit(1);
      }

      grep(needle, argv[i], fd);

      if (ferror(fd)) {
        perror("getline");
        fclose(fd);
        exit(1);
      }

      fclose(fd);
    }
  }

  return 0;
}
