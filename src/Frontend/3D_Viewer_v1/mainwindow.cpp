#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  currentFrame = new QImage();

  currentSettings = new QSettings("settings.config", QSettings::IniFormat);
  loadSettings();

  setSettingsToUi();

  ui->doubleSpinBox_vertRed->setValue(ui->openGLWidget->vertRed);
  ui->doubleSpinBox_vertGreen->setValue(ui->openGLWidget->vertGreen);
  ui->doubleSpinBox_vertBlue->setValue(ui->openGLWidget->vertBlue);

  ui->doubleSpinBox_sizeOfPoints->setValue(ui->openGLWidget->pointsSize);

  ui->doubleSpinBox_backRed->setValue(ui->openGLWidget->backRed);
  ui->doubleSpinBox_backGreen->setValue(ui->openGLWidget->backGreen);
  ui->doubleSpinBox_backBlue->setValue(ui->openGLWidget->backBlue);
  ui->doubleSpinBox_backAlpha->setValue(ui->openGLWidget->backAlpha);

  ui->doubleSpinBox_facetRed->setValue(ui->openGLWidget->fRed);
  ui->doubleSpinBox_facetGreen->setValue(ui->openGLWidget->fGreen);
  ui->doubleSpinBox_facetBlue->setValue(ui->openGLWidget->fBlue);
  ui->doubleSpinBox_weightOfLine->setValue(ui->openGLWidget->lineWidth);
}

MainWindow::~MainWindow() {
  delete ui;
  delete currentSettings;
  delete currentFrame;
}

void MainWindow::on_pushButton_openFile_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Choose object file:", "/~", "OBJ (*.obj)");
  if (!fileName.isNull()) {
    int lastSep = fileName.lastIndexOf('/') + 1;
    ui->labelFileInfo->setText("File: " + fileName.sliced(lastSep));
    if (ui->openGLWidget->currentData.matrix_of_vertexes.matrix != NULL) {
      ui->openGLWidget->clearFields();
      ui->doubleSpinBox_xAngle->setValue(0.0);
      ui->doubleSpinBox_yAngle->setValue(0.0);
      ui->doubleSpinBox_zAngle->setValue(0.0);
      ui->doubleSpinBox_xAxis->setValue(0.0);
      ui->doubleSpinBox_yAxis->setValue(0.0);
      ui->doubleSpinBox_zAxis->setValue(0.0);
      ui->doubleSpinBox_scale->setValue(100.0);
    }
    ui->openGLWidget->filePath = fileName;
    ui->openGLWidget->status =
        s21_parser(ui->openGLWidget->filePath.toLatin1().data(),
                   &ui->openGLWidget->currentData);
    if (ui->openGLWidget->status == 0) {
      s21_vertexes_scaled(&ui->openGLWidget->currentData, 1);
      s21_vertexes_centering(&ui->openGLWidget->currentData);
      s21_copy_data(ui->openGLWidget->currentData, &ui->openGLWidget->drawData);
      s21_matrix_copy(ui->openGLWidget->currentData,
                      &ui->openGLWidget->drawData,
                      &ui->openGLWidget->copyMatrixStatus);
      QString tmp;
      tmp.setNum(ui->openGLWidget->currentData.f_count, 10);
      ui->labelFacets->setText("Facets: " + tmp);
      tmp.setNum(ui->openGLWidget->currentData.v_count, 10);
      ui->labelVertexes->setText("Vertexes: " + tmp);
      ui->labelStatus->setText("Status: OK");
    } else if (ui->openGLWidget->status == 1) {
      ui->labelStatus->setText("Status: Wrong File Extension!");
    } else if (ui->openGLWidget->status == 2) {
      ui->labelStatus->setText("Status: Can't open the File!");
    } else if (ui->openGLWidget->status == 3) {
      ui->labelStatus->setText("Status: Empty or broken file!");
    }
  }
}

void MainWindow::on_doubleSpinBox_xAngle_valueChanged(double arg1) {
  ui->openGLWidget->xAngle = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_yAngle_valueChanged(double arg1) {
  ui->openGLWidget->yAngle = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_zAngle_valueChanged(double arg1) {
  ui->openGLWidget->zAngle = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_xAxis_valueChanged(double arg1) {
  ui->openGLWidget->xMov = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_yAxis_valueChanged(double arg1) {
  ui->openGLWidget->yMov = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_zAxis_valueChanged(double arg1) {
  ui->openGLWidget->zMov = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_scale_valueChanged(double arg1) {
  ui->openGLWidget->scale = arg1 / 100.0;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_noPointl_clicked() {
  ui->openGLWidget->vertexesViewCheck = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_squarePoints_clicked() {
  ui->openGLWidget->vertexesViewCheck = 1;
  ui->openGLWidget->vertexesStyle = SQUARE;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_circlePoints_clicked() {
  ui->openGLWidget->vertexesViewCheck = 1;
  ui->openGLWidget->vertexesStyle = CIRCLE;
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_selectVertexesColor_clicked() {
  QColor tmp = QColorDialog::getColor(Qt::red, this);
  tmp.getRgbF(&ui->openGLWidget->vertRed, &ui->openGLWidget->vertGreen,
              &ui->openGLWidget->vertBlue);
  ui->doubleSpinBox_vertRed->setValue(ui->openGLWidget->vertRed);
  ui->doubleSpinBox_vertGreen->setValue(ui->openGLWidget->vertGreen);
  ui->doubleSpinBox_vertBlue->setValue(ui->openGLWidget->vertBlue);
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_vertRed_valueChanged(double arg1) {
  ui->openGLWidget->vertRed = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_vertGreen_valueChanged(double arg1) {
  ui->openGLWidget->vertGreen = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_vertBlue_valueChanged(double arg1) {
  ui->openGLWidget->vertBlue = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_sizeOfPoints_valueChanged(double arg1) {
  ui->openGLWidget->pointsSize = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_selectBackColor_clicked() {
  QColor tmp = QColorDialog::getColor(Qt::black, this);
  tmp.getRgbF(&ui->openGLWidget->backRed, &ui->openGLWidget->backGreen,
              &ui->openGLWidget->backBlue, &ui->openGLWidget->backAlpha);
  ui->doubleSpinBox_backRed->setValue(ui->openGLWidget->backRed);
  ui->doubleSpinBox_backGreen->setValue(ui->openGLWidget->backGreen);
  ui->doubleSpinBox_backBlue->setValue(ui->openGLWidget->backBlue);
  ui->doubleSpinBox_backAlpha->setValue(ui->openGLWidget->backAlpha);
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_backRed_valueChanged(double arg1) {
  ui->openGLWidget->backRed = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_backGreen_valueChanged(double arg1) {
  ui->openGLWidget->backGreen = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_backBlue_valueChanged(double arg1) {
  ui->openGLWidget->backBlue = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_backAlpha_valueChanged(double arg1) {
  ui->openGLWidget->backAlpha = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_selectFacetsColor_clicked() {
  QColor tmp = QColorDialog::getColor(Qt::red, this);
  tmp.getRgbF(&ui->openGLWidget->fRed, &ui->openGLWidget->fGreen,
              &ui->openGLWidget->fBlue);
  ui->doubleSpinBox_facetRed->setValue(ui->openGLWidget->fRed);
  ui->doubleSpinBox_facetGreen->setValue(ui->openGLWidget->fGreen);
  ui->doubleSpinBox_facetBlue->setValue(ui->openGLWidget->fBlue);
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_facetRed_valueChanged(double arg1) {
  ui->openGLWidget->fRed = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_facetGreen_valueChanged(double arg1) {
  ui->openGLWidget->fGreen = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_facetBlue_valueChanged(double arg1) {
  ui->openGLWidget->fBlue = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_checkBox_dottedLine_stateChanged(int arg1) {
  ui->openGLWidget->dottedLineStatus = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_weightOfLine_valueChanged(double arg1) {
  ui->openGLWidget->lineWidth = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_Ortho_clicked() {
  ui->openGLWidget->projectionType = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_Perspective_clicked() {
  ui->openGLWidget->projectionType = 1;
  ui->openGLWidget->update();
}

void MainWindow::loadSettings() {
  if (this->currentSettings->value("OpenedEarlier").toBool()) {
    ui->openGLWidget->projectionType =
        this->currentSettings->value("ProjectionType").toInt();

    ui->openGLWidget->backRed =
        this->currentSettings->value("BackgroundColor/Red").toFloat();
    ui->openGLWidget->backGreen =
        this->currentSettings->value("BackgroundColor/Green").toFloat();
    ui->openGLWidget->backBlue =
        this->currentSettings->value("BackgroundColor/Blue").toFloat();
    ui->openGLWidget->backAlpha =
        this->currentSettings->value("BackgroundColor/Alpha").toFloat();

    ui->openGLWidget->vertRed =
        this->currentSettings->value("Vertexes/Color/Red").toFloat();
    ui->openGLWidget->vertGreen =
        this->currentSettings->value("Vertexes/Color/Green").toFloat();
    ui->openGLWidget->vertBlue =
        this->currentSettings->value("Vertexes/Color/Blue").toFloat();
    ui->openGLWidget->vertexesStyle =
        this->currentSettings->value("Vertexes/Style").toFloat();
    ui->openGLWidget->vertexesViewCheck =
        this->currentSettings->value("Vertexes/View").toFloat();
    ui->openGLWidget->pointsSize =
        this->currentSettings->value("Vertexes/Size").toFloat();

    ui->openGLWidget->fRed =
        this->currentSettings->value("Facets/Color/Red").toFloat();
    ui->openGLWidget->fGreen =
        this->currentSettings->value("Facets/Color/Green").toFloat();
    ui->openGLWidget->fBlue =
        this->currentSettings->value("Facets/Color/Blue").toFloat();
    ui->openGLWidget->dottedLineStatus =
        this->currentSettings->value("Facets/DottedLine").toInt();
    ui->openGLWidget->lineWidth =
        this->currentSettings->value("Facets/Width").toFloat();

    ui->openGLWidget->screenCastType =
        this->currentSettings->value("Screencast").toInt();
  }
}

void MainWindow::saveSettings() {
  this->currentSettings->setValue("OpenedEarlier", true);

  this->currentSettings->setValue("ProjectionType",
                                  ui->openGLWidget->projectionType);

  this->currentSettings->setValue("BackgroundColor/Red",
                                  ui->openGLWidget->backRed);
  this->currentSettings->setValue("BackgroundColor/Green",
                                  ui->openGLWidget->backGreen);
  this->currentSettings->setValue("BackgroundColor/Blue",
                                  ui->openGLWidget->backBlue);
  this->currentSettings->setValue("BackgroundColor/Alpha",
                                  ui->openGLWidget->backAlpha);

  this->currentSettings->setValue("Vertexes/Color/Red",
                                  ui->openGLWidget->vertRed);
  this->currentSettings->setValue("Vertexes/Color/Green",
                                  ui->openGLWidget->vertGreen);
  this->currentSettings->setValue("Vertexes/Color/Blue",
                                  ui->openGLWidget->vertBlue);
  this->currentSettings->setValue("Vertexes/Style",
                                  ui->openGLWidget->vertexesStyle);
  this->currentSettings->setValue("Vertexes/View",
                                  ui->openGLWidget->vertexesViewCheck);
  this->currentSettings->setValue("Vertexes/Size",
                                  ui->openGLWidget->pointsSize);

  this->currentSettings->setValue("Facets/Color/Red", ui->openGLWidget->fRed);
  this->currentSettings->setValue("Facets/Color/Green",
                                  ui->openGLWidget->fGreen);
  this->currentSettings->setValue("Facets/Color/Blue", ui->openGLWidget->fBlue);
  this->currentSettings->setValue("Facets/DottedLine",
                                  ui->openGLWidget->dottedLineStatus);
  this->currentSettings->setValue("Facets/Width", ui->openGLWidget->lineWidth);

  this->currentSettings->setValue("Screencast",
                                  ui->openGLWidget->screenCastType);
}

void MainWindow::closeEvent(QCloseEvent* event) {
  saveSettings();
  if (ui->openGLWidget->currentData.matrix_of_vertexes.matrix != NULL) {
    s21_remove_data(&ui->openGLWidget->currentData);
    s21_remove_matrix(&ui->openGLWidget->drawData.matrix_of_vertexes);
  }
  event->accept();
}

void MainWindow::setSettingsToUi() {
  if (ui->openGLWidget->projectionType == 1) {
    ui->radioButton_Ortho->setChecked(false);
    ui->radioButton_Perspective->setChecked(true);
  }
  if (ui->openGLWidget->vertexesViewCheck) {
    ui->radioButton_noPointl->setChecked(false);
    if (ui->openGLWidget->vertexesStyle == SQUARE) {
      ui->radioButton_squarePoints->setChecked(true);
    } else {
      ui->radioButton_circlePoints->setChecked(true);
    }
  }
  if (ui->openGLWidget->dottedLineStatus == 2) {
    ui->checkBox_dottedLine->setChecked(true);
  }
  if (ui->openGLWidget->screenCastType != BMP_STYLE) {
    ui->radioButton_BMP->setChecked(false);
    if (ui->openGLWidget->screenCastType == JPEG_STYLE) {
      ui->radioButton_JPEG->setChecked(true);
    } else {
      ui->radioButton_GIF->setChecked(true);
    }
  }
}

void MainWindow::on_pushButton_Screencasts_clicked() {
  if (ui->openGLWidget->currentData.matrix_of_vertexes.matrix != NULL) {
    if (ui->openGLWidget->screenCastType == BMP_STYLE) {
      QString file = QFileDialog::getSaveFileName(this, "Input file name", "/",
                                                  "BMP (*.bmp)");
      if (!file.isNull()) {
        *currentFrame = ui->openGLWidget->grabFramebuffer();
        currentFrame->save(file, "BMP");
      }
    } else if (ui->openGLWidget->screenCastType == JPEG_STYLE) {
      QString file = QFileDialog::getSaveFileName(this, "Input file name", "/",
                                                  "JPEG (*.jpeg)");
      if (!file.isNull()) {
        *currentFrame = ui->openGLWidget->grabFramebuffer();
        currentFrame->save(file, "JPEG");
      }
    } else {
      currentGif = new QGifImage();
      gifTimer = new QTimer();
      currentGif->setDefaultDelay(100);
      connect(gifTimer, SIGNAL(timeout()), this, SLOT(screencastGif()));
      gifTimer->start(100);
    }
  }
}

void MainWindow::screencastGif() {
  frameCount++;
  QSize gifSize(640, 480);
  *currentFrame = ui->openGLWidget->grabFramebuffer().scaled(gifSize);
  currentGif->addFrame(*currentFrame);
  if (frameCount == 50) {
    gifTimer->stop();
    QString file = QFileDialog::getSaveFileName(this, "Input file name", "/",
                                                "GIF (*.gif)");
    if (!file.isNull()) {
      currentGif->save(file);
    }
    frameCount = 0;
    delete currentGif;
    delete gifTimer;
  }
}

void MainWindow::on_radioButton_BMP_clicked() {
  ui->openGLWidget->screenCastType = BMP_STYLE;
}

void MainWindow::on_radioButton_JPEG_clicked() {
  ui->openGLWidget->screenCastType = JPEG_STYLE;
}

void MainWindow::on_radioButton_GIF_clicked() {
  ui->openGLWidget->screenCastType = GIF_STYLE;
}
