#include "Hamlet_functions.h"

int main (int argc, char* argv[]) {
  const char* input  = NULL;
  const char* output = NULL;

  int main_mode = main_arguments (argc, argv, &input, &output);

  switch (main_mode) {
    case RELEASE: {
      printf ("Hello! Today I want to sort by line from the beginning ");
      printf ("and from the end of William Shakespeare's 'Hamlet'\n");

      Text text = {};
      read_buf(&text, input);

      quicksort (text.pointers, 0, text.quantity_of_string - 1, compare_begin);
      write_text (&text, output);

      qsort (text.pointers, text.quantity_of_string, sizeof(String), compare_end);
      write_text (&text, output);

      printf_text (&text, output);

      free_text(&text);
    } break;

    case NOT_ARGUMENT: {
      return 1;
    } break;

    case U_TEST: {
      //unit_tests();
    } break;
  }

  return 0;
}
