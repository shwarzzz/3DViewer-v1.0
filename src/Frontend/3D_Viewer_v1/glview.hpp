#ifndef GLVIEW_HPP
#define GLVIEW_HPP

#include <QtOpenGL>
#include <QtOpenGLWidgets>

#define SQUARE 0
#define CIRCLE 1

#define BMP_STYLE 0
#define JPEG_STYLE 1
#define GIF_STYLE 2

extern "C" {
#include "../../Backend/s21_affine_transformations.h"
#include "../../Backend/s21_parser.h"
}

class glView : public QOpenGLWidget {
  Q_OBJECT

 public:
  glView(QWidget* parent = nullptr);

  int copyMatrixStatus = 0;
  int status = -1;
  int vertexesViewCheck = 0;
  int vertexesStyle = SQUARE;
  int dottedLineStatus = 0;

  int screenCastType = BMP_STYLE;

  float backRed = 0;
  float backGreen = 0;
  float backBlue = 0;
  float backAlpha = 1;

  float vertRed = 1;
  float vertGreen = 0;
  float vertBlue = 0;

  float fRed = 0.97f;
  float fGreen = 0;
  float fBlue = 0.93f;

  float pointsSize = 10;
  double lineWidth = 1;

  double xMov = 0;
  double yMov = 0;
  double zMov = 0;

  double xAngle = 0;
  double yAngle = 0;
  double zAngle = 0;

  int projectionType = 0;

  double scale = 1;

  QString filePath;

  s21_data currentData;
  s21_data drawData;

  void initializeGL() override;
  void resizeGL(int w, int h) override;

  void paintGL() override;
  void drawGL();
  void doAllChanges();
  void changeProjectionType();

  void clearFields();
};

#endif  // GLVIEW_HPP
