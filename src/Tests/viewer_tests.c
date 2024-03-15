#include <check.h>

#include "../Backend/s21_affine_transformations.h"
#include "../Backend/s21_parser.h"

START_TEST(s21_parser_test_1) {
  s21_data res = {0};
  s21_parser("./Tests/test1.txt", &res);
  ck_assert_ptr_null(res.matrix_of_vertexes.matrix);
  ck_assert_ptr_null(res.polygons);
}
END_TEST

START_TEST(s21_parser_test_2) {
  s21_data res = {0};
  char arr[] = "./Tests/test1.obj";
  double check_matrix[12][3] = {{1, 2, 3},       {2, 4, 5},    {5, 6, 7},
                                {5.53251, 2, 3}, {5, 99, -1},  {2, 3, 5},
                                {0.123, 11, -2}, {-2, -3, -4}, {-5, -6, -7},
                                {1, 2, 3},       {3, 2, 1},    {4, 4, 4}};
  double check_polygons[3][15] = {
      {0, 1, 2}, {10, 0, 2}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
  s21_parser(arr, &res);
  for (unsigned long i = 0; i < res.f_count; i++) {
    for (unsigned long j = 0; j < res.polygons[i].numbers_of_vertexes_in_facets;
         j++) {
      ck_assert_double_eq(check_polygons[i][j], res.polygons[i].vertexes[j]);
    }
  }
  for (unsigned long i = 0; i < res.v_count; i++) {
    for (unsigned long j = 0; j < 3; j++) {
      ck_assert_double_eq(check_matrix[i][j],
                          res.matrix_of_vertexes.matrix[i][j]);
    }
  }
  ck_assert_uint_eq(res.f_count, 3);
  ck_assert_uint_eq(res.v_count, 12);
  ck_assert_uint_eq(res.matrix_of_vertexes.rows, 12);
  ck_assert_uint_eq(res.matrix_of_vertexes.columns, 3);
  s21_remove_data(&res);
}
END_TEST

START_TEST(s21_parser_test_3) {
  s21_data res = {0};
  s21_data dst = {0};
  int status = 0;
  char arr[] = "./Tests/test2.obj";
  double check_matrix[3][3] = {
      {-1.2344, 5.01202, 0.4412}, {1, 2, 3}, {5, 9, 6}};
  double check_polygons[3][3] = {{0, 1, 2}, {1, 0, 2}, {2, 1, 0}};
  s21_parser(arr, &res);
  s21_copy_data(res, &dst);
  s21_matrix_copy(res, &dst, &status);
  for (unsigned long i = 0; i < dst.f_count; i++) {
    for (unsigned long j = 0; j < dst.polygons[i].numbers_of_vertexes_in_facets;
         j++) {
      ck_assert_double_eq(check_polygons[i][j], dst.polygons[i].vertexes[j]);
    }
  }
  for (unsigned long i = 0; i < dst.v_count; i++) {
    for (unsigned long j = 0; j < 3; j++) {
      ck_assert_double_eq(res.matrix_of_vertexes.matrix[i][j],
                          dst.matrix_of_vertexes.matrix[i][j]);
      ck_assert_double_eq(check_matrix[i][j],
                          res.matrix_of_vertexes.matrix[i][j]);
    }
  }
  ck_assert_uint_eq(res.f_count, 3);
  ck_assert_uint_eq(res.v_count, 3);
  ck_assert_uint_eq(res.matrix_of_vertexes.rows, 3);
  ck_assert_uint_eq(res.matrix_of_vertexes.columns, 3);
  s21_remove_data(&res);
  s21_remove_matrix(&dst.matrix_of_vertexes);
}
END_TEST

START_TEST(s21_parser_test_4) {
  s21_data res = {0};
  char arr[] = "./Tests/test3.obj";
  s21_parser(arr, &res);
  ck_assert_ptr_null(res.matrix_of_vertexes.matrix);
  ck_assert_ptr_null(res.polygons);
}
END_TEST

START_TEST(s21_parser_test_5) {
  s21_data res = {0};
  char arr[] = "nosuchfile.obj";
  s21_parser(arr, &res);
  ck_assert_ptr_null(res.matrix_of_vertexes.matrix);
  ck_assert_ptr_null(res.polygons);
}
END_TEST

START_TEST(s21_parser_test_6) {
  s21_data res = {0};
  char arr[] = "./Tests/emptyfile.obj";
  s21_parser(arr, &res);
  ck_assert_ptr_null(res.matrix_of_vertexes.matrix);
  ck_assert_ptr_null(res.polygons);
}
END_TEST

START_TEST(s21_parser_test_7) {
  s21_data res = {0};
  char arr[] = "./Tests/test4.obj";
  s21_parser(arr, &res);
  ck_assert_ptr_null(res.matrix_of_vertexes.matrix);
  ck_assert_ptr_null(res.polygons);
}
END_TEST

START_TEST(s21_parser_test_8) {
  s21_data res = {0};
  char arr[] = "./Tests/test5.obj";
  s21_parser(arr, &res);
  ck_assert_ptr_null(res.matrix_of_vertexes.matrix);
  ck_assert_ptr_null(res.polygons);
}
END_TEST

START_TEST(s21_parser_test_9) {
  s21_data res = {0};
  char arr[] = "./Tests/test6.obj";
  s21_parser(arr, &res);
  ck_assert_ptr_null(res.matrix_of_vertexes.matrix);
  ck_assert_ptr_null(res.polygons);
}
END_TEST

START_TEST(s21_affine_transformations_test_1) {
  s21_data res = {0};
  s21_data check = {0};
  char arr[] = "./Tests/test1.obj";
  s21_parser(arr, &res);
  int status = 0;
  s21_matrix_copy(res, &check, &status);
  s21_vertexes_movement(&res, 0, 1.5);
  s21_vertexes_movement(&res, 1, 0.052);
  s21_vertexes_movement(&res, 2, -2.2);
  for (int i = 0; i < res.matrix_of_vertexes.rows; i++) {
    ck_assert_double_eq(res.matrix_of_vertexes.matrix[i][0],
                        check.matrix_of_vertexes.matrix[i][0] + 1.5);
    ck_assert_double_eq(res.matrix_of_vertexes.matrix[i][1],
                        check.matrix_of_vertexes.matrix[i][1] + 0.052);
    ck_assert_double_eq(res.matrix_of_vertexes.matrix[i][2],
                        check.matrix_of_vertexes.matrix[i][2] - 2.2);
  }
  s21_remove_data(&res);
  s21_remove_matrix(&check.matrix_of_vertexes);
}
END_TEST

START_TEST(s21_affine_transformations_test_2) {
  s21_data res = {0};
  double check[3][3] = {{-0.75, -2.5, 0}, {-2.75, 2.5, 0}, {2.75, -0.9, 0}};
  char arr[] = "./Tests/test7.obj";
  s21_parser(arr, &res);
  s21_vertexes_centering(&res);
  for (int i = 0; i < res.matrix_of_vertexes.rows; i++) {
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][0], check[i][0],
                            0.000001);
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][1], check[i][1],
                            0.000001);
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][2], check[i][2],
                            0.000001);
  }
  s21_remove_data(&res);
}
END_TEST

START_TEST(s21_affine_transformations_test_3) {
  s21_data res = {0};
  double check[3][3] = {{-0.727273, -1.090909, 0.727273},
                        {-1.454545, 0.727273, 0.727273},
                        {0.545455, -0.509091, 0.727273}};
  char arr[] = "./Tests/test7.obj";
  s21_parser(arr, &res);
  s21_vertexes_scaled(&res, 1);
  for (int i = 0; i < res.matrix_of_vertexes.rows; i++) {
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][0], check[i][0],
                            0.000001);
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][1], check[i][1],
                            0.000001);
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][2], check[i][2],
                            0.000001);
  }
  s21_remove_data(&res);
}
END_TEST

START_TEST(s21_affine_transformations_test_4) {
  s21_data res = {0};
  double check[3][3] = {{-0.581119, -1.175291, 0.727273},
                        {0.993473, -0.266200, 1.454545},
                        {-0.077249, -0.884382, -0.545455}};
  char arr[] = "./Tests/test7.obj";
  s21_parser(arr, &res);
  s21_vertexes_scaled(&res, 1);
  for (int i = 0; i < res.matrix_of_vertexes.rows; i++) {
    s21_vertexes_rotation(&res, 0, i, 60);
    s21_vertexes_rotation(&res, 1, i, -90);
    s21_vertexes_rotation(&res, 2, i, 360);
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][0], check[i][0],
                            0.000001);
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][1], check[i][1],
                            0.000001);
    ck_assert_double_eq_tol(res.matrix_of_vertexes.matrix[i][2], check[i][2],
                            0.000001);
  }
  s21_remove_data(&res);
}
END_TEST

Suite* s21_affine_transformations_check_suite(void) {
  TCase* tc = tcase_create("s21_affine_transformations_check_core");
  Suite* s = suite_create("s21_affine_transformations_check_tests");

  tcase_add_test(tc, s21_affine_transformations_test_1);
  tcase_add_test(tc, s21_affine_transformations_test_2);
  tcase_add_test(tc, s21_affine_transformations_test_3);
  tcase_add_test(tc, s21_affine_transformations_test_4);

  suite_add_tcase(s, tc);

  return s;
}

Suite* s21_parser_check_suite(void) {
  TCase* tc = tcase_create("s21_parser_check_core");
  Suite* s = suite_create("s21_parser_check_tests");

  tcase_add_test(tc, s21_parser_test_1);
  tcase_add_test(tc, s21_parser_test_2);
  tcase_add_test(tc, s21_parser_test_3);
  tcase_add_test(tc, s21_parser_test_4);
  tcase_add_test(tc, s21_parser_test_5);
  tcase_add_test(tc, s21_parser_test_6);
  tcase_add_test(tc, s21_parser_test_7);
  tcase_add_test(tc, s21_parser_test_8);
  tcase_add_test(tc, s21_parser_test_9);

  suite_add_tcase(s, tc);

  return s;
}

int main() {
  int failed = 0;
  SRunner* sr;

  sr = srunner_create(s21_parser_check_suite());
  srunner_add_suite(sr, s21_affine_transformations_check_suite());

  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? 1 : 0;
}