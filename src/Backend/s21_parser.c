#include "s21_parser.h"

// Парсер
int s21_parser(char* path, s21_data* data) {
  int flag = OK;
  data->f_count = 0;
  data->v_count = 0;
  if (s21_file_extension_check(path)) {
    FILE* file = fopen(path, "r");
    if (file) {
      s21_vertexes_and_facets_counting(file, data);
      if (data->v_count != 0 && data->f_count != 0) {
        s21_create_matrix(data->v_count, 3, &data->matrix_of_vertexes);
        file = fopen(path, "r");
        data->polygons = s21_read_file(file, data);
        fclose(file);
        if (data->polygons == NULL) {
          s21_remove_matrix(&data->matrix_of_vertexes);
          data->f_count = 0;
          data->v_count = 0;
          flag = EMPTY_OR_BROKEN_FILE;
        } else {
          s21_find_max_and_min(data);
        }
      } else {
        flag = EMPTY_OR_BROKEN_FILE;
      }
    } else {
      flag = FILE_NOT_OPENED;
    }
  } else {
    flag = WRONG_FILE_EXTENSION;
  }
  return flag;
}
// Второе чтение - запись данных в структуру
s21_polygon* s21_read_file(FILE* file, s21_data* data) {
  char c = 0;
  int flag = 0;
  int stop = 0;
  s21_polygon* polygons = calloc(data->f_count, sizeof(s21_polygon));
  for (unsigned long i = 0; i < data->f_count; i++) {
    polygons[i].vertexes = calloc(10, sizeof(int));
  }
  unsigned long current_facet = 0;
  while (c != EOF && stop != 2) {
    c = fgetc(file);
    int cond = s21_v_or_f_conditions_check(file, &c, flag);
    if (cond == 1) {
      stop = s21_add_vert_to_matrix(data, file, &c);
    } else if (cond == 2) {
      stop = s21_add_facets_to_data(data, polygons, file, &c, current_facet);
      current_facet++;
    }
    if (c == '\n') {
      flag = 0;
    } else {
      flag = 1;
    }
  }
  if (stop == 2) {
    for (unsigned long i = 0; i < data->f_count; i++) {
      free(polygons[i].vertexes);
    }
    free(polygons);
    polygons = NULL;
  }
  return polygons;
}
// Запись данных для полигонов в стркутуру
int s21_add_facets_to_data(s21_data* data, s21_polygon* facets, FILE* file,
                           char* c, unsigned long cur_facet) {
  int flag = 0;
  unsigned long count = 0;
  while ((*c != '\n' && *c != '\0') && flag != 2 && *c != EOF) {
    flag = 0;
    char num[1000] = "";
    int i = 0;
    int sign = 1;
    while (*c != ' ' && *c != '\n' && *c != '\0' && *c != EOF) {
      *c = fgetc(file);
    }
    if (*c == ' ') {
      while (!flag && *c != EOF) {
        *c = fgetc(file);
        if (i == 0 && *c == '-') {
          sign = -1;
        } else if (s21_isdigit(*c) != 0 || *c == '.') {
          num[i] = *c;
          if (*c == '.') {
            flag = 2;
          }
          i++;
        } else if (i > 0) {
          if (i > 1 && num[0] == '0' && s21_isdigit(num[1])) {
            flag = 2;
          } else {
            flag++;
          }
        } else {
          flag = 3;
        }
      }
      if (flag == 1) {
        num[i] = '\0';
        double res = s21_convert_to_double(num, sign);
        if (count > 0 && count % 10 == 0) {
          int* tmp =
              realloc(facets[cur_facet].vertexes, (count + 10) * sizeof(int));
          if (tmp == NULL) {
            flag = 2;
          } else {
            facets[cur_facet].vertexes = tmp;
          }
        }
        if (flag != 2) {
          if (res < 0) {
            res = res + data->matrix_of_vertexes.current_v_count + 1;
          }
          facets[cur_facet].vertexes[count] = res - 1;
          count++;
          if (res - 1 >= data->v_count) {
            flag = 2;
          }
        }
      }
    }
  }
  if (flag != 2) {
    facets[cur_facet].numbers_of_vertexes_in_facets = count;
  }
  return flag;
}
// Содержится ли данные о точке или полигоне в строке
int s21_v_or_f_conditions_check(FILE* file, char* c, int flag) {
  int res = 0;
  if (flag == 0) {
    if (*c == 'v') {
      *c = fgetc(file);
      if (*c == ' ') {
        res = 1;
      }
    } else if (*c == 'f') {
      *c = fgetc(file);
      if (*c == ' ') {
        res = 2;
      }
    }
  } else if (*c == '\n') {
    *c = fgetc(file);
    if (*c == 'v') {
      *c = fgetc(file);
      if (*c == ' ') {
        res = 1;
      }
    } else if (*c == 'f') {
      *c = fgetc(file);
      if (*c == ' ') {
        res = 2;
      }
    }
  }
  return res;
}
// Запись точек в матрицу
int s21_add_vert_to_matrix(s21_data* data, FILE* file, char* c) {
  int flag = 0;
  int col = 0;
  while (*c != EOF && col != 3 && (*c != '\n' && *c != '\0') && flag != 2) {
    flag = 0;
    char num[1000] = "";
    int i = 0;
    int dot_count = 0;
    int sign = 1;
    while (*c != EOF && *c != ' ' && *c != '\n' && *c != '\0') {
      *c = fgetc(file);
    }
    if (*c == ' ') {
      while (*c != EOF && !flag) {
        *c = fgetc(file);
        if (i == 0 && *c == ' ') {
          continue;
        } else if (i == 0 && *c == '-') {
          sign = -1;
        } else if (s21_isdigit(*c) != 0 || *c == '.') {
          num[i] = *c;
          if (*c == '.') {
            dot_count++;
          }
          i++;
        } else {
          if ((i > 0 && num[i - 1] == '.') || dot_count > 1 ||
              (i > 1 && num[0] == '0' && num[1] != '.')) {
            flag = 2;
          } else {
            flag++;
          }
        }
      }
      if (flag == 1) {
        num[i] = '\0';
        double res = s21_convert_to_double(num, sign);
        data->matrix_of_vertexes
            .matrix[data->matrix_of_vertexes.current_v_count][col % 3] = res;
        col++;
      }
    }
  }
  if (flag != 2 && col == 3) {
    data->matrix_of_vertexes.current_v_count++;
  } else {
    flag = 2;
  }
  return flag;
}
// Проверка символа на цифру
int s21_isdigit(char c) {
  int res = 0;
  if (c >= 48 && c <= 57) {
    res = 1;
  }
  return res;
}
// Перевод обработанной строки в число
double s21_convert_to_double(char* str, int sign) {
  int dot_check = 0;
  double count = 1;
  double res = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == '.') {
      dot_check++;
      count = 0;
    } else {
      res = res * 10 + (str[i] - '0');
      count++;
    }
  }
  if (dot_check) {
    res /= pow(10, count);
  }
  res *= sign;
  return res;
}
// Первый проход - считаем количесвто полигонов и точек
void s21_vertexes_and_facets_counting(FILE* file, s21_data* data) {
  char c = '\0';
  int flag = 0;
  while (c != EOF) {
    c = fgetc(file);
    int cond = s21_v_or_f_conditions_check(file, &c, flag);
    if (cond == 1) {
      data->v_count++;
    } else if (cond == 2) {
      data->f_count++;
    }
    if (c == '\n') {
      flag = 0;
    } else {
      flag = 1;
    }
  }
  fclose(file);
}
// Проверка файла на формат
int s21_file_extension_check(char* path) {
  size_t len = strlen(path);
  int res = len > 3 ? 1 : 0;
  int j = 0;
  if (res) {
    for (size_t i = len - 4; i < len; i++) {
      if (S21_EXTENSION[j] != path[i]) {
        res = 0;
      }
      j++;
    }
  }
  return res;
}
// Создание матрицы
int s21_create_matrix(int rows, int columns, matrix_t* result) {
  int res = OK;
  if (rows > 0 && columns > 0) {
    result->columns = columns;
    result->rows = rows;
    result->current_v_count = 0;
    result->x_max = 0;
    result->x_min = 0;
    result->y_max = 0;
    result->y_min = 0;
    result->z_max = 0;
    result->z_min = 0;
    result->matrix = (double**)calloc(rows, sizeof(double*));
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double*)calloc(columns, sizeof(double));
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}
// Удаление матрицы
void s21_remove_matrix(matrix_t* A) {
  if (A) {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i] != NULL) {
        free(A->matrix[i]);
      }
    }
    if (A->matrix != NULL) {
      free(A->matrix);
    }
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
    A->current_v_count = 0;
    A->x_max = 0;
    A->x_min = 0;
    A->y_max = 0;
    A->y_min = 0;
    A->z_max = 0;
    A->z_min = 0;
  }
}
// Удаление данных
void s21_remove_data(s21_data* data) {
  s21_remove_matrix(&data->matrix_of_vertexes);
  for (unsigned long i = 0; i < data->f_count; i++) {
    free(data->polygons[i].vertexes);
  }
  free(data->polygons);
  data->f_count = 0;
  data->v_count = 0;
}
// Копирование данных из структуры
void s21_copy_data(s21_data src, s21_data* dst) {
  dst->f_count = src.f_count;
  dst->v_count = src.v_count;
  dst->polygons = src.polygons;
}
// Копирование матрицы
void s21_matrix_copy(s21_data src, s21_data* dst, int* status) {
  if (*status) {
    s21_remove_matrix(&dst->matrix_of_vertexes);
  }
  *status = 1;
  s21_create_matrix(src.matrix_of_vertexes.rows, src.matrix_of_vertexes.columns,
                    &dst->matrix_of_vertexes);
  for (unsigned long i = 0; i < src.v_count; i++) {
    dst->matrix_of_vertexes.matrix[i][0] = src.matrix_of_vertexes.matrix[i][0];
    dst->matrix_of_vertexes.matrix[i][1] = src.matrix_of_vertexes.matrix[i][1];
    dst->matrix_of_vertexes.matrix[i][2] = src.matrix_of_vertexes.matrix[i][2];
  }
  dst->matrix_of_vertexes.x_max = src.matrix_of_vertexes.x_max;
  dst->matrix_of_vertexes.x_min = src.matrix_of_vertexes.x_min;
  dst->matrix_of_vertexes.y_max = src.matrix_of_vertexes.y_max;
  dst->matrix_of_vertexes.y_min = src.matrix_of_vertexes.y_min;
  dst->matrix_of_vertexes.z_max = src.matrix_of_vertexes.z_max;
  dst->matrix_of_vertexes.z_min = src.matrix_of_vertexes.z_min;
  dst->matrix_of_vertexes.current_v_count =
      src.matrix_of_vertexes.current_v_count;
}
// Поиск max и min для каждой оси
void s21_find_max_and_min(s21_data* data) {
  for (unsigned long i = 0; i < data->v_count; i++) {
    if (data->matrix_of_vertexes.matrix[i][0] >
        data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.x_max][0]) {
      data->matrix_of_vertexes.x_max = i;
    }
    if (data->matrix_of_vertexes.matrix[i][0] <
        data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.x_min][0]) {
      data->matrix_of_vertexes.x_min = i;
    }
    if (data->matrix_of_vertexes.matrix[i][1] >
        data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.y_max][1]) {
      data->matrix_of_vertexes.y_max = i;
    }
    if (data->matrix_of_vertexes.matrix[i][1] <
        data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.y_min][1]) {
      data->matrix_of_vertexes.y_min = i;
    }
    if (data->matrix_of_vertexes.matrix[i][2] >
        data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.z_max][2]) {
      data->matrix_of_vertexes.z_max = i;
    }
    if (data->matrix_of_vertexes.matrix[i][2] <
        data->matrix_of_vertexes.matrix[data->matrix_of_vertexes.z_min][2]) {
      data->matrix_of_vertexes.z_min = i;
    }
  }
}
