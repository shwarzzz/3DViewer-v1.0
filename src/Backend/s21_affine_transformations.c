#include "s21_affine_transformations.h"

// Поворот оси на угол
void s21_vertexes_rotation(s21_data* data, int axis, unsigned long i,
                           double angle) {
  angle = angle * (M_PI / 180.0);
  double tmp_x = data->matrix_of_vertexes.matrix[i][0];
  double tmp_y = data->matrix_of_vertexes.matrix[i][1];
  double tmp_z = data->matrix_of_vertexes.matrix[i][2];
  if (axis == 0) {
    data->matrix_of_vertexes.matrix[i][1] =
        tmp_y * cos(angle) - tmp_z * sin(angle);
    data->matrix_of_vertexes.matrix[i][2] =
        tmp_y * sin(angle) + tmp_z * cos(angle);
  } else if (axis == 1) {
    data->matrix_of_vertexes.matrix[i][0] =
        tmp_x * cos(angle) - tmp_z * sin(angle);
    data->matrix_of_vertexes.matrix[i][2] =
        tmp_x * sin(angle) + tmp_z * cos(angle);
  } else {
    data->matrix_of_vertexes.matrix[i][0] =
        tmp_x * cos(angle) - tmp_y * sin(angle);
    data->matrix_of_vertexes.matrix[i][1] =
        tmp_x * sin(angle) + tmp_y * cos(angle);
  }
}
// Сдвиг по осям
void s21_vertexes_movement(s21_data* data, int axis, double value) {
  for (unsigned long i = 0; i < data->v_count; i++) {
    data->matrix_of_vertexes.matrix[i][axis] += value;
  }
}
// Масштабирование
void s21_vertexes_scaled(s21_data* data, double value) {
  double x =
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.x_max][0] -
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.x_min][0];
  double y =
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.y_max][1] -
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.y_min][1];
  double z =
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.z_max][2] -
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.z_min][2];
  double scale = (value - (value * -1)) / s21_max_of_three(x, y, z);
  for (unsigned long i = 0; i < data->v_count; i++) {
    data->matrix_of_vertexes.matrix[i][0] *= scale;
    data->matrix_of_vertexes.matrix[i][1] *= scale;
    data->matrix_of_vertexes.matrix[i][2] *= scale;
  }
}
// Центровка объекта по всем осям
void s21_vertexes_centering(s21_data* data) {
  double centrX =
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.x_min][0] +
      (data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.x_max][0] -
       data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.x_min][0]) /
          2;
  double centrY =
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.y_min][1] +
      (data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.y_max][1] -
       data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.y_min][1]) /
          2;
  double centrZ =
      data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.z_min][2] +
      (data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.z_max][2] -
       data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.z_min][2]) /
          2;
  for (unsigned long i = 0; i < data->v_count; i++) {
    data->matrix_of_vertexes.matrix[i][0] -= centrX;
    data->matrix_of_vertexes.matrix[i][1] -= centrY;
    data->matrix_of_vertexes.matrix[i][2] -= centrZ;
  }
}
// Максимум из трех чисел
double s21_max_of_three(double a, double b, double c) {
  double res = a;
  if (b > a && b > c) {
    res = b;
  } else if (c > a) {
    res = c;
  }
  return res;
}