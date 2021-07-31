#include "header.h"

int main(int argc, char *argv[]) {
  FILE *f = NULL;
  /*Index used to loop through the input files*/
  int index;
  /*Line counter*/
  int IC = 100;
  int DC = 0;
  /*Check if there are no arguments*/
  if (argc == 1) {
    /*Print error to screen*/
    printf("You did not provide any input files.\n");
  } else {
    for (index = 1; index < argc; index++) {
      printf("%s.as: ", argv[index]);
    }
  }
  return 0;
}