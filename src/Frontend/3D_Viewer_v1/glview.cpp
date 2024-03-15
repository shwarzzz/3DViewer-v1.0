#include "glview.hpp"

glView::glView(QWidget* parent) : QOpenGLWidget(parent) {
  currentData.f_count = 0;
  currentData.v_count = 0;
  currentData.matrix_of_vertexes.matrix = NULL;
  setWindowTitle("Kitty go!");
}

void glView::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void glView::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void glView::paintGL() {
  glClearColor(backRed, backGreen, backBlue, backAlpha);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (currentData.f_count > 0 && currentData.v_count > 0 &&
      currentData.matrix_of_vertexes.matrix != NULL) {
    drawGL();
  }
}

void glView::drawGL() {
  changeProjectionType();
  glMatrixMode(GL_MODELVIEW);
  doAllChanges();
  for (unsigned long i = 0; i < drawData.f_count; i++) {
    glLineWidth(lineWidth);
    if (dottedLineStatus) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(8, 4);
    } else {
      glDisable(GL_LINE_STIPPLE);
    }
    glBegin(GL_LINE_LOOP);
    glColor3f(fRed, fGreen, fBlue);
    for (unsigned long j = 0;
         j < drawData.polygons[i].numbers_of_vertexes_in_facets; j++) {
      double x = drawData.matrix_of_vertexes
                     .matrix[drawData.polygons[i].vertexes[j]][0];
      double y = drawData.matrix_of_vertexes
                     .matrix[drawData.polygons[i].vertexes[j]][1];
      double z = drawData.matrix_of_vertexes
                     .matrix[drawData.polygons[i].vertexes[j]][2];
      glVertex3f(x, y, z);
    }
    glEnd();
  }
  if (vertexesStyle == SQUARE) {
    glDisable(GL_POINT_SMOOTH);
  } else {
    glEnable(GL_POINT_SMOOTH);
  }
  if (vertexesViewCheck) {
    glPointSize(pointsSize);
    glBegin(GL_POINTS);
    for (unsigned long i = 0; i < drawData.v_count; i++) {
      glColor3f(vertRed, vertGreen, vertBlue);
      double x = drawData.matrix_of_vertexes.matrix[i][0];
      double y = drawData.matrix_of_vertexes.matrix[i][1];
      double z = drawData.matrix_of_vertexes.matrix[i][2];
      glVertex3f(x, y, z);
    }
    glEnd();
  }
  s21_matrix_copy(currentData, &drawData, &copyMatrixStatus);
}

void glView::clearFields() {
  this->xMov = 0;
  this->yMov = 0;
  this->zMov = 0;
  this->xAngle = 0;
  this->yAngle = 0;
  this->zAngle = 0;
  this->scale = 1;
  s21_remove_data(&currentData);
  s21_remove_matrix(&drawData.matrix_of_vertexes);
  this->copyMatrixStatus = 0;
}

void glView::doAllChanges() {
  double x =
      drawData.matrix_of_vertexes.matrix[drawData.matrix_of_vertexes.x_max][0] -
      drawData.matrix_of_vertexes.matrix[drawData.matrix_of_vertexes.x_min][0];
  double y =
      drawData.matrix_of_vertexes.matrix[drawData.matrix_of_vertexes.y_max][1] -
      drawData.matrix_of_vertexes.matrix[drawData.matrix_of_vertexes.y_min][1];
  double z =
      drawData.matrix_of_vertexes.matrix[drawData.matrix_of_vertexes.z_max][2] -
      drawData.matrix_of_vertexes.matrix[drawData.matrix_of_vertexes.z_min][2];
  double factor = (scale - (scale * -1)) / s21_max_of_three(x, y, z);

  for (unsigned long index = 0; index < drawData.v_count; index++) {
    drawData.matrix_of_vertexes.matrix[index][0] *= factor;
    drawData.matrix_of_vertexes.matrix[index][1] *= factor;
    drawData.matrix_of_vertexes.matrix[index][2] *= factor;

    drawData.matrix_of_vertexes.matrix[index][0] += xMov;
    drawData.matrix_of_vertexes.matrix[index][1] += yMov;
    drawData.matrix_of_vertexes.matrix[index][2] += zMov;

    s21_vertexes_rotation(&drawData, 0, index, xAngle);
    s21_vertexes_rotation(&drawData, 1, index, yAngle);
    s21_vertexes_rotation(&drawData, 2, index, zAngle);
  }
}

void glView::changeProjectionType() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float winWidthMin = currentData.matrix_of_vertexes
                          .matrix[currentData.matrix_of_vertexes.x_min][0];
  float winWidthMax = currentData.matrix_of_vertexes
                          .matrix[currentData.matrix_of_vertexes.x_max][0];
  float winHeightMin = currentData.matrix_of_vertexes
                           .matrix[currentData.matrix_of_vertexes.y_min][1];
  float winHeightMax = currentData.matrix_of_vertexes
                           .matrix[currentData.matrix_of_vertexes.y_max][1];
  float fov = 60 * M_PI / 180;
  float heapHeight = currentData.matrix_of_vertexes
                         .matrix[currentData.matrix_of_vertexes.y_max][1] /
                     (2 * tan(fov / 2));
  if (projectionType == 0) {
    glOrtho(winWidthMin * 2, winWidthMax * 2, winHeightMin * 2,
            winHeightMax * 2, heapHeight, 5);
  } else {
    glFrustum(winWidthMin, winWidthMax, winHeightMin, winHeightMax, heapHeight,
              5);
  }
  glTranslated(0, 0, -heapHeight * 3);
}
