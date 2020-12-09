#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

const int RELEASE = 0;
const int U_TEST = 1;
const int NOT_ARGUMENT = 2;

struct String {
  char*  pointer;
  size_t length;
};

struct Text {
  size_t buf_size;
  char* buf;
  size_t quantity_of_string;
  String* pointers;
};

int unit_tests();
int main_arguments(int argc, char* argv[], const char** input, const char** output);

long int size_of_file(const char* file_name);
int      read_buf(Text* text, const char* input);

long int number_of_string(Text* text);
int      get_pointers(String** pointers, char* buf, size_t buf_size, size_t quantity_of_string);
int      get_pointers(Text* text);

int  my_isalpha(char c);
int  my_strcmp(char* str1, size_t str_beg1, size_t str_end1, char* str2, size_t str_beg2, size_t str_end2);
int  compare_begin(const void* str1, const void* str2);
int  compare_end(const void* str1, const void* str2);
void swap(String* pointers, int str1, int str2);
void quicksort(String* pointers, int left, int right, int (*compare)(const void* a, const void* b));

int  write_file(String* pointers, size_t quantity_of_strings, const char* output);
int  write_text(Text* text, const char* output);
int  printf_text(char* buf, long int buf_size, const char* output);
int  printf_text(Text* text, const char* output);
void free_text(Text* text);
