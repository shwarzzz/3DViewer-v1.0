#ifndef S21_PARSER_H

#define S21_PARSER_H
#define S21_EXTENSION ".obj"

#define INCORRECT_MATRIX 1

#define OK 0
#define WRONG_FILE_EXTENSION 1
#define FILE_NOT_OPENED 2
#define EMPTY_OR_BROKEN_FILE 3

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
  unsigned long current_v_count;
  unsigned long x_max;
  unsigned long y_max;
  unsigned long z_max;
  unsigned long x_min;
  unsigned long y_min;
  unsigned long z_min;
} matrix_t;

typedef struct facets {
  int* vertexes;
  unsigned long numbers_of_vertexes_in_facets;
} s21_polygon;

typedef struct data {
  unsigned long v_count;
  unsigned long f_count;
  matrix_t matrix_of_vertexes;
  s21_polygon* polygons;
} s21_data;

int s21_parser(char* path, s21_data* data);

s21_polygon* s21_read_file(FILE* file, s21_data* data);

int s21_v_or_f_conditions_check(FILE* file, char* c, int flag);
int s21_add_vert_to_matrix(s21_data* data, FILE* file, char* c);
int s21_add_facets_to_data(s21_data* data, s21_polygon* facets, FILE* file,
                           char* c, unsigned long cur_facet);

int s21_isdigit(char c);
double s21_convert_to_double(char* str, int sign);

void s21_vertexes_and_facets_counting(FILE* file, s21_data* data);
int s21_file_extension_check(char* path);
void s21_find_max_and_min(s21_data* data);

int s21_create_matrix(int rows, int columns, matrix_t* result);
void s21_remove_matrix(matrix_t* A);
void s21_matrix_copy(s21_data src, s21_data* dst, int* status);

void s21_remove_data(s21_data* data);
void s21_copy_data(s21_data src, s21_data* dst);

#endif
