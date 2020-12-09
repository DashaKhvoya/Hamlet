#include "Hamlet_functions.h"

int unit_tests() {

  char first[] = "bcdef";
  char second[] = "abcdef";
  String value[2] = { { first, 5 }, { second, 6 } };
  swap(value, 0, 1);
  String expected[2] = { { second, 6 }, { first, 5 } };
  for (int i = 0; i<2; i++) {
    if (value[i].length != expected[i].length && value[i].pointer != expected[i].pointer) {
      printf ("TEST_SWAP_FAILED\n");
      return 1;
    }
  }

  long int buf_size = size_of_file("test.txt");
  long int true_size_file = 15;
  if (buf_size != true_size_file) {
    printf ("TEST_SIZE_FAILED\n");
    return 1;
  }

  char* buf = (char*)calloc (buf_size + 1, sizeof(char));
  FILE* file_name = fopen ("test.txt", "r");
  fread (buf, sizeof(char), buf_size, file_name);
  int result = number_of_string(buf, buf_size);
  int expected1 = 3;
  if (result != expected1) {
    printf ("TEST_NUMBER_FAILED\n");
    return 1;
  }

  size_t quantity_of_string;
  String* pointers;
  get_pointers(&pointers, buf, buf_size, &quantity_of_string);
  char expected2[3] = {0, 6, 10};
  for (int i = 0; i < 3; i++){
      if (*(pointers[i].pointer) != buf[expected2[i]]) {
      printf ("TEST_POINTERS_FAILED\n");
      return 1;
    }
  }

  String str1 = { first, 5};
  String str2 = { second, 6};
  int expected3 = 'b'-'a';
  if (expected3 != compare_begin((const void*)&str1, (const void*)&str2)) {
    printf ("TEST_STRCM_FAILED\n");
    return 1;
  }

  int expected4 = 0;
  if (expected4 != compare_end((const void*)&str1, (const void*)&str2)) {
    printf ("TEST_STRCM_REV_FAILED\n");
    return 1;
  }

  char third[] = "aaaggfg";
  String value1[3] = { {first, 5}, {second, 6}, {third, 7} };
  quicksort(value1, 0, 2, compare_begin);
  String expected5[3] = { {third, 7}, {second, 6}, {first, 5} };
  for (int i = 0; i < 2; i++){
    if (compare_begin(&value1[i], &expected5[i]) != 0) {
      printf ("TEST_QSORT_FAILED\n");
      return 1;
    }
  }

  return 0;
}
