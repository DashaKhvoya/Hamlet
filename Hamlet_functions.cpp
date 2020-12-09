//! @author Daria Baranchikova 29.09.2020

#include "Hamlet_functions.h"

//---------------------------------------------------
//! Handlink of command line arguments
//!
//! @param [in]  argc    number of command line arguments
//! @param [in]  argv    command line arguments
//! @param [in]  input   name of input file
//! @param [in]  output  name of output file
//!
//! @return handlink result
//---------------------------------------------------

int main_arguments(int argc, char* argv[], const char** input, const char** output) {
  assert (input);
  assert (output);

  if (argc>1) {
    if (strcmp (argv[1], "--test") == 0)
      return U_TEST;

    for (int i = 1; i < argc; i++) {
      if (strcmp (argv[i], "--input")  == 0 || strcmp (argv[i], "-i") == 0) {
        if (i == argc - 1) {
          printf ("Please, enter argument");
          return NOT_ARGUMENT;
        }

        i++;
        *input = argv[i];
      }

      if (strcmp (argv[i], "--output") == 0 || strcmp (argv[i], "-o") == 0) {
        if (i == argc - 1) {
          printf ("Please, enter argument");
          return NOT_ARGUMENT;
        }

        i++;
        *output = argv[i];
      }
    }
  }


  if (*input  == NULL)
    *input  = "Hamlet.txt";
  if (*output == NULL)
    *output = "Hamlet_ready.txt";

  return RELEASE;
}


//---------------------------------------------------
//! Measuring file sizes
//!
//! @param [in]   file_name      name of file
//! @param [out]  stbuf.st_size  size of file
//!
//! @return size of file
//---------------------------------------------------

long int size_of_file(const char* file_name) {
  assert (file_name);

  struct stat stbuf = {};
  int res = stat (file_name, &stbuf);

  if (res != 0)  {
    printf ("Error getting file size %s\n", file_name);
    assert (res != 0);
  }

  return stbuf.st_size;
}

//---------------------------------------------------
//! Reading text to buffer
//!
//! @param [in]  buf_size  size of the file
//! @param [in]  buf       an array where the contests of the file are written
//! @param [in]  input     name of input file
//!
//! @return reading success
//---------------------------------------------------

int read_buf(Text* text, const char* input) {
  assert (text);
  assert (input);

  FILE* file_name = fopen (input, "r");

  if (file_name == NULL) {
    printf ("Open error of file %s\n", input);
    return 3;
  }

  text -> buf_size = size_of_file (input);
  text -> buf = (char*)calloc ((*text).buf_size + 1, sizeof(char));

  if (text -> buf == NULL) {
    printf ("Error. Place for buf not found");
    return 1;
  }

  text -> buf_size = fread (text -> buf, sizeof(char),text -> buf_size, file_name);
  text -> buf[text -> buf_size] = '\0';
  text -> quantity_of_string = number_of_string(text);
  get_pointers(text);

  fclose (file_name);

  return 0;
}

//---------------------------------------------------
//! Finding the number of lines in the original text
//!
//! @param [in]  buf       an array where the contests of the file are written
//! @param [in]  buf_size  size of the file
//!
//! @return number of strings in the original text
//!
//! @note the function replace '\n' with '\0'
//---------------------------------------------------

long int number_of_string(Text* text) {
  assert (text -> buf);
  assert (text -> buf_size > 0);

  long int number = 0;

  for (int i = 0; i < text -> buf_size; i++) {
    if (text -> buf[i] == '\n') {
      number += 1;
      text -> buf[i] = '\0';
    }
  }

  return number;
}

//---------------------------------------------------

int get_pointers(Text* text) {
  return get_pointers(&text -> pointers, text -> buf, text -> buf_size, text -> quantity_of_string);
}

//---------------------------------------------------
//! Creating an array of strings
//!
//! @param [out]  pointers             an array of pointers to the beginning of the lines of the original text
//! @param [in]   buf                  an array where the contests of the file are written
//! @param [in]   buf_size             size of the file
//! @param [out]  quantity_of_strings  number of strings in original text
//!
//! @return creating success
//---------------------------------------------------

int get_pointers(String** pointers, char* buf, size_t buf_size, size_t quantity_of_string) {
  assert (pointers);
  assert (buf);
  assert (quantity_of_string);

  *pointers = (String*)calloc (quantity_of_string, sizeof(String));
  if (pointers == NULL) {
    printf ("Error. Place for pointers not found");
    return 1;
  }

  (*pointers)[0].pointer = &buf[0];

  int j = 1;

  for (int i = 1; i < buf_size; i++) {
    if (buf[i] == '\0') {
      (*pointers)[j - 1].length = &buf[i] - (*pointers)[j - 1].pointer;
      (*pointers)[j].pointer = &buf[i + 1];
      ++j;
    }
  }

  (*pointers)[j].length = &buf[buf_size - 1] - (*pointers)[j].pointer ;

  return 0;
}

//---------------------------------------------------
//! Cheking if a symbol is a letter
//!
//! @param [in]  c  symbol
//!
//! @return symbol is a letter or not
//---------------------------------------------------

int my_isalpha(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

//---------------------------------------------------
//! Comparing two strings from the beginning of a string
//!
//! @param  [in]  str1      pointer to the beginning of the first  line
//! @param  [in]  str_beg1  begin index of string str1
//! @param  [in]  str_end1  end index of string str1
//! @param  [in]  str2      pointer to the beginning of the second line
//! @param  [in]  str_beg2  begin index of string str2
//! @param  [in]  str_end2  end index of string str2
//!
//! @return  <0, 0, >0 if str1 < str2, str1 == str2, str1 > str2
//---------------------------------------------------

int my_strcmp(char* str1, size_t str_beg1, size_t str_end1, char* str2, size_t str_beg2, size_t str_end2) {
  assert(str1);
  assert(str2);

  int step = 0;
  if (str_beg1 < str_end1) {
    step = 1;
  } else {
    step = -1;
  }

  size_t i = str_beg1, j = str_beg2;

  while (true) {
    while (i != str_end1 && !(my_isalpha(str1[i]))) {
      i = i + step;
    }
    while (j != str_end2 && !(my_isalpha(str2[j]))) {
      j = j + step;
    }

    if (i == str_end1 || j == str_end2 || tolower(str1[i]) != tolower(str2[j])) {
      break;
    }

    i = i + step;
    j = j + step;
  }

  return (int)(tolower(str1[i]) - tolower(str2[j]));
}

//---------------------------------------------------
//! Comparator to compare from the beginning of a string
//!
//! @param  [in]  str1  pointer to the first line
//! @param  [in]  str2  pointer to the second line
//!
//! @return result of the function my_strcmp
//---------------------------------------------------

int compare_begin(const void* str1, const void* str2) {
    assert(str1);
    assert(str2);

    String line1 = *(String*)str1;
    String line2 = *(String*)str2;

    return my_strcmp(line1.pointer, 0, line1.length - 1, line2.pointer, 0, line2.length - 1);
}

//---------------------------------------------------
//! Comparator to compare from the end of a string
//!
//! @param  [in]  str1  pointer to the first line
//! @param  [in]  str2  pointer to the first line
//!
//! @return result of the function my_strcmp
//---------------------------------------------------

int compare_end(const void* str1, const void* str2) {
    assert(str1);
    assert(str2);

    String line1 = *(String*)str1;
    String line2 = *(String*)str2;

    return my_strcmp(line1.pointer, line1.length - 1, 0, line2.pointer, line2.length - 1, 0);
}

//---------------------------------------------------
//! Swap the two specified array elements
//!
//! @param [in]  pointers  an array of pointers to the beginning of the lines of the original text
//! @param [in]  str1      elemet of array pointers
//! @param [in]  str2      elemet of array pointers
//---------------------------------------------------

void swap(String* pointers, int str1, int str2) {
  assert (pointers);

  String temp  = pointers[str1];
  pointers[str1] = pointers[str2];
  pointers[str2] = temp;
}

//---------------------------------------------------
//! Quicksort algorithm for lexicographic sorting of text from beginning
//!
//! @param [out]  pointers  an array of pointers to the beginning of the lines of the original text
//! @param [in]   left      left element of sorting
//! @param [in]   right     right element of sorting
//! @param [in]   compare   comparator for string comparison
//---------------------------------------------------

void quicksort(String* pointers, int left, int right, int (*compare)(const void* a, const void* b)) {
  int begin = left;

  if (left >= right)
    return;

  if (right - left == 1) {
    if (compare ((const void*) &pointers[right], (const void*) &pointers[left]) < 0) {
      swap (pointers, right, left);
    }
  }

  for (int i = left + 1; i <= right; i++) {
    if (compare ((const void*) &pointers[i], (const void*) &pointers[left]) < 0) {
      swap (pointers, ++begin, i);
    }
  }

  swap     (pointers, left,  begin);
  quicksort(pointers, left,  begin - 1, compare);
  quicksort(pointers, begin + 1, right, compare);
}

//---------------------------------------------------

int write_text(Text* text, const char* output) {
  return write_file(text -> pointers, text -> quantity_of_string, output);
}

//---------------------------------------------------
//! Printing text
//!
//! @param [in]  pointers             an array of pointers to the beginning of the lines of the original text
//! @param [in]  quantity_of_strings  number_of_strings in original text
//! @param [in]  output               name of output file
//!
//! @return return 0 on success
//---------------------------------------------------

int write_file(String* pointers, size_t quantity_of_strings, const char* output) {
  assert (pointers);
  assert (output);

  FILE* hFile = fopen (output, "a");

  for (int i = 0; i < quantity_of_strings; i++) {
    fprintf (hFile, "%s\n", pointers[i].pointer);
  }

  fprintf (hFile, "\n-----------------------------------------------------\n\n");

  fclose (hFile);

  return 0;
}

//---------------------------------------------------
//! Printing original text
//!
//! @param [in]  buf       an array where the contests of the file are written
//! @param [in]  buf_size  size of the file
//! @param [in]  output        name of output file
//!
//! @note the function replace '\0' with '\n'
//!
//! @return return 0 on success
//---------------------------------------------------

int printf_text(char* buf, long int buf_size, const char* output) {
  assert (buf);
  assert (output);
  assert (buf_size > 0);

  FILE* hFile = fopen (output, "a");

  for (int i = 0; i < buf_size; i++) {
    if (buf[i] == '\0') {
      buf[i] = '\n';
    }
    fputc (buf[i], hFile);
  }

  fprintf (hFile, "\n-----------------------------------------------------\n\n");

  fclose (hFile);

  return 0;
}

//---------------------------------------------------

int printf_text(Text* text, const char* output) {
  return printf(text -> buf, text -> buf_size, output);
}

//---------------------------------------------------

void free_text(Text* text) {
  free(text -> buf);
  free(text -> pointers);
}
