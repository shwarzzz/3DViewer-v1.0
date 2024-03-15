#ifndef S21_AFFINE_TRANSFORMATIONS_H

#define S21_AFFINE_TRANSFORMATIONS_H

#include "s21_parser.h"

void s21_vertexes_centering(s21_data* data);
void s21_vertexes_scaled(s21_data* data, double value);
void s21_vertexes_movement(s21_data* data, int axis, double value);
void s21_vertexes_rotation(s21_data* data, int axis, unsigned long i,
                           double angle);
double s21_max_of_three(double a, double b, double c);

#endif