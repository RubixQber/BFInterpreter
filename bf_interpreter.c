/*******************************************************************************
* Joshua Lowe
* Adv. Topics in Computer Science
* April 3, 2020
*
* This program acts as an interpreter for the "Brazilian Fumarole" language,
* taking in a file path argument and executing the code stored at that file.
* It supports an internal array of up to 50,000 characters and allows the user
* to input a theoretically infinite file, provided that there is enough memory.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define LENGTH 50000
#define VALID_CHARS "+-<>()*"

/*
  This method increments the pointer by one and throws an error if the pointer was already at the end.
  unsigned char** pointer_to_pointer:     a pointer to the "Divine Pointress"
  unsigned char array[]:                  the array of tribespeople
*/
void increment_pointer(unsigned char** pointer_to_pointer, unsigned char array[])
{
  unsigned char* pointer_to_end = &array[LENGTH - 1];
  if ((*pointer_to_pointer) == pointer_to_end)
  {
    printf("Error: can't go forward any more!\n");
    exit(3);
  }
  (*pointer_to_pointer) += 1;
  return;
}
/*
  This method decrements the pointer by one and throws an error if the pointer was already at the start.
  unsigned char** pointer_to_pointer:     a pointer to the "Divine Pointress"
  unsigned char array[]:                  the array of tribespeople
*/
void decrement_pointer(unsigned char** pointer_to_pointer, unsigned char array[])
{
  unsigned char* pointer_to_start = &array[0];
  if ((*pointer_to_pointer) == pointer_to_start)
  {
    printf("Error: can't go backward any more!\n");
    exit(3);
  }
  (*pointer_to_pointer) -= 1;
  return;
}
/*
  This method increments the value of the bag at the pointer by one.
  unsigned char* pointer:     the "Divine Pointress"
*/
void increment_value(unsigned char* pointer)
{
  (*pointer) += 1;
  return;
}
/*
  This method decrements the value of the bag at the pointer by one.
  unsigned char* pointer:     the "Divine Pointress"
*/
void decrement_value(unsigned char* pointer)
{
  (*pointer) -= 1;
  return;
}
/*
  This method locates the next corresponding closed parenthesis in the target string
  and returns its index.
  char* str:     the target string
  long size:     the size of the string
  int start:     the index of where to start searching
*/
int next_close(char* str, long size, int start)
{
  int parens = 0;
  for (int i = start; i < size/sizeof(str[0]); i++)
  {
      if (str[i] == ')')
      {
        parens -= 1;
      }
      else if (str[i] == '(')
      {
        parens += 1;
      }
      if (parens < 0)
      {
        return i;
      }
  }
  printf("Weird parentheses error.\n");
  exit(5);
}
/*
  This method locates the previous corresponding open parenthesis in the target string
  and returns its index.
  char* str:     the target string
  long size:     the size of the string
  int end:       the index of where to start searching
*/
int prev_open(char* str, long size, int end)
{
  int parens = 0;
  for (int i = end; i > 0; i--)
  {
      if (str[i] == ')')
      {
        parens -= 1;
      }
      else if (str[i] == '(')
      {
        parens += 1;
      }
      if (parens > 0)
      {
        return i;
      }
  }
  printf("Weird parentheses error.\n");
  exit(5);
}
/*
  This method determines whether the character is valid and returns a boolean.
  char c:     the character
*/
bool is_valid(char c)
{
  for (int i = 0; i < strlen(VALID_CHARS); i++)
  {
    if (c == VALID_CHARS[i] || c == 10 || c == 0 || c == 32)
    {
      return true;
    }
  }
  return false;
}
/*
  This method checks the code for matching parentheses and valid characters
  and throws an error if the code is invalid.
  char* code:         the Brazilian Fumarole code
  long file_size:     the size of the string
*/
void check_code(char* code, long file_size)
{
  int parens = 0;
  for (int i = 0; i < file_size; i++)
  {
    char current = code[i];
    if (code[i] == ')')
    {
      parens -= 1;
    }
    else if (code[i] == '(')
    {
      parens += 1;
    }
    else if (!is_valid(code[i]))
    {
      printf("Error: Invalid character \"%c\" at position %d.\n", code[i], i);
      exit(4);
    }
    if (parens < 0)
    {
      printf("Error: mismatched parentheses\n");
      exit(2);
    }
  }
  if (parens != 0)
  {
    printf("Error: mismatched parentheses\n");
    exit(2);
  }
}
/*
  This method executes Brazilian Fumarole code, looping through the code and executing each character.
  char* code:                     the BF code
  unsigned char array[]:          the array of tribespeople
  unsigned char* pointer:         the Divine Pointress
  long file_size:                 the length of the BF code
*/
void exec_code(char* code, unsigned char array[], unsigned char* pointer, long file_size)
{
  for (int i = 0; i < file_size; i++)
  {
    char current = code[i];
    unsigned char** pointer_to_pointer = &pointer;
    switch (current) {
      case '+': increment_value(pointer);
        break;
      case '-': decrement_value(pointer);
        break;
      case '>':
        increment_pointer(pointer_to_pointer, array);
        break;
      case '<':
        decrement_pointer(pointer_to_pointer, array);
        break;
      case '*': printf("%c", *pointer);
        break;
      case '(':
        if (*pointer == 0)
        {
          i = next_close(code, file_size, i + 1) - 1;
        }
        else if ((code[i + 1] == '-' || code[i + 1] == '+') && code[i + 2] == ')')
        {
          (*pointer) = 0;
          i = i + 2;
        }
        break;
      case ')':
        if (*pointer != 0)
        {
          i = prev_open(code, file_size, i - 1) - 1;
        }
        break;
    }
  }
}

int main(int argc, char *argv[])
{
  clock_t begin = clock(); //starts a timer

  char* file_path = argv[1];
  FILE* fp = fopen(file_path, "r");
  if (fp == NULL) //if no file exists at that path
  {
    printf("Error: No file exists with that path.\n");
    exit(1);
  }


  // This reads in the code stored at that file, combining each line with the next.
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char* code = malloc(file_size);
  char* x = fgets(code, file_size, fp);
  while (code[file_size - 1] == 0)
  {
    char temp[file_size];
    x = fgets(temp, file_size, fp);
    strcat(code, temp);
  }
  fclose(fp);

  unsigned char array[LENGTH] = {0};
  unsigned char* pointer = &array[0];

  check_code(code, file_size);
  exec_code(code, array, pointer, file_size);
  free(code);

  //stops the timer and prints the length of time taken
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time spent: %fs\n", time_spent);
}
