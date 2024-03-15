#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>

#include "glview.hpp"
#include "qtgifimage/src/gifimage/qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private:
  QSettings* currentSettings;
  QImage* currentFrame;
  QGifImage* currentGif;
  QTimer* gifTimer;
  int frameCount = 0;

  void loadSettings();
  void saveSettings();

 private slots:
  void on_pushButton_openFile_clicked();

  void on_doubleSpinBox_xAngle_valueChanged(double arg1);

  void on_doubleSpinBox_yAngle_valueChanged(double arg1);

  void on_doubleSpinBox_zAngle_valueChanged(double arg1);

  void on_doubleSpinBox_xAxis_valueChanged(double arg1);

  void on_doubleSpinBox_yAxis_valueChanged(double arg1);

  void on_doubleSpinBox_zAxis_valueChanged(double arg1);

  void on_doubleSpinBox_scale_valueChanged(double arg1);

  void on_radioButton_noPointl_clicked();

  void on_radioButton_squarePoints_clicked();

  void on_radioButton_circlePoints_clicked();

  void on_pushButton_selectVertexesColor_clicked();

  void on_doubleSpinBox_vertRed_valueChanged(double arg1);

  void on_doubleSpinBox_vertGreen_valueChanged(double arg1);

  void on_doubleSpinBox_vertBlue_valueChanged(double arg1);

  void on_doubleSpinBox_sizeOfPoints_valueChanged(double arg1);

  void on_pushButton_selectBackColor_clicked();

  void on_doubleSpinBox_backRed_valueChanged(double arg1);

  void on_doubleSpinBox_backGreen_valueChanged(double arg1);

  void on_doubleSpinBox_backBlue_valueChanged(double arg1);

  void on_doubleSpinBox_backAlpha_valueChanged(double arg1);

  void on_pushButton_selectFacetsColor_clicked();

  void on_doubleSpinBox_facetRed_valueChanged(double arg1);

  void on_doubleSpinBox_facetGreen_valueChanged(double arg1);

  void on_doubleSpinBox_facetBlue_valueChanged(double arg1);

  void on_checkBox_dottedLine_stateChanged(int arg1);

  void on_doubleSpinBox_weightOfLine_valueChanged(double arg1);

  void on_radioButton_Ortho_clicked();

  void on_radioButton_Perspective_clicked();

  void closeEvent(QCloseEvent* event);

  void setSettingsToUi();

  void on_pushButton_Screencasts_clicked();

  void on_radioButton_BMP_clicked();

  void on_radioButton_JPEG_clicked();

  void on_radioButton_GIF_clicked();

  void screencastGif();

 private:
  Ui::MainWindow* ui;
};
#endif  // MAINWINDOW_H
