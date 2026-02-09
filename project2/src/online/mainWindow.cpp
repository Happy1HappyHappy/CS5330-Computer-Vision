#include "mainWindow.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QScrollArea>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  process = new QProcess(this);
  connect(process, &QProcess::readyReadStandardOutput, this,
          &MainWindow::handleProcessOutput);
  connect(process,
          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
          &MainWindow::handleProcessFinished);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

  // Top control panel
  // Use QGridLayout for compact, multi-row layout
  QGridLayout *controlsLayout = new QGridLayout();

  // Row 0: Load Image
  browseButton = new QPushButton("Load Target Image", this);
  connect(browseButton, &QPushButton::clicked, this, &MainWindow::browseImage);
  controlsLayout->addWidget(browseButton, 0, 0);

  targetImagePathLabel = new QLabel("No image selected", this);
  controlsLayout->addWidget(targetImagePathLabel, 0, 1);

  // Row 1: Method
  controlsLayout->addWidget(new QLabel("Method:", this), 1, 0);
  methodComboBox = new QComboBox(this);
  methodComboBox->addItem("Baseline");
  methodComboBox->addItem("RG Histogram (rghist)");
  methodComboBox->addItem("RGB Histogram (rgbhist)");
  methodComboBox->addItem("Multi Histogram (multihist)");
  methodComboBox->addItem("Multi Center Focus");
  methodComboBox->addItem("DNN (SSD)");
  methodComboBox->addItem("CIE + Gabor");
  controlsLayout->addWidget(methodComboBox, 1, 1);
  connect(methodComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::updateWeightFields);

  // Row 2: N
  controlsLayout->addWidget(new QLabel("N (Top Matches):", this), 2, 0);
  nSpinBox = new QSpinBox(this);
  nSpinBox->setRange(1, 50);
  nSpinBox->setValue(3);
  controlsLayout->addWidget(nSpinBox, 2, 1);

  // Row 3: W1
  w1Label = new QLabel("W1:", this);
  controlsLayout->addWidget(w1Label, 3, 0);
  weightSpinBox1 = new QDoubleSpinBox(this);
  weightSpinBox1->setRange(0.01, 100.0);
  weightSpinBox1->setSingleStep(0.1);
  weightSpinBox1->setValue(1.0);
  controlsLayout->addWidget(weightSpinBox1, 3, 1);

  // Row 4: W2
  w2Label = new QLabel("W2:", this);
  controlsLayout->addWidget(w2Label, 4, 0);
  weightSpinBox2 = new QDoubleSpinBox(this);
  weightSpinBox2->setRange(0.01, 100.0);
  weightSpinBox2->setSingleStep(0.1);
  weightSpinBox2->setValue(1.0);
  controlsLayout->addWidget(weightSpinBox2, 4, 1);

  // Row 5: W3
  w3Label = new QLabel("W3:", this);
  controlsLayout->addWidget(w3Label, 5, 0);
  weightSpinBox3 = new QDoubleSpinBox(this);
  weightSpinBox3->setRange(0.01, 100.0);
  weightSpinBox3->setSingleStep(0.1);
  weightSpinBox3->setValue(1.0);
  controlsLayout->addWidget(weightSpinBox3, 5, 1);

  // Row 6: Search
  searchButton = new QPushButton("Search", this);
  connect(searchButton, &QPushButton::clicked, this, &MainWindow::runSearch);
  controlsLayout->addWidget(searchButton, 6, 0, 1, 2); // Span 2 columns

  // Add controls layout to main layout
  mainLayout->addLayout(controlsLayout);

  // Set explicit geometry or size policy to prevent excessive width?
  // Usually layouts handle it, but we can set a max width on controls if
  // needed. For now, let's trust QGridLayout.

  updateWeightFields(); // Set initial visibility

  // Target Image Display
  targetImageLabel = new QLabel(this);
  targetImageLabel->setFixedSize(200, 200);
  targetImageLabel->setScaledContents(true);
  targetImageLabel->setStyleSheet(
      "border: 1px solid gray; background-color: #eee;");
  targetImageLabel->setAlignment(Qt::AlignCenter);
  targetImageLabel->setText("Target Image");

  QHBoxLayout *targetDisplayLayout = new QHBoxLayout();
  targetDisplayLayout->addWidget(targetImageLabel);
  targetDisplayLayout->addStretch();
  mainLayout->addLayout(targetDisplayLayout);

  // Results Area
  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  resultsContainer = new QWidget();
  resultsLayout = new QGridLayout(resultsContainer);
  scrollArea->setWidget(resultsContainer);
  mainLayout->addWidget(scrollArea);

  // Console Log
  logConsole = new QTextEdit(this);
  logConsole->setReadOnly(true);
  logConsole->setMaximumHeight(150);
  mainLayout->addWidget(logConsole);
}

void MainWindow::browseImage() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open Image", QDir::currentPath() + "/data/olympus",
      "Images (*.png *.xpm *.jpg *.jpeg *.tif)");
  if (!fileName.isEmpty()) {
    currentTargetImagePath = fileName;
    targetImagePathLabel->setText(fileName);
    QPixmap pix(fileName);
    targetImageLabel->setPixmap(pix.scaled(targetImageLabel->size(),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));
    logConsole->append("Selected target: " + fileName);
  }
}

void MainWindow::runSearch() {
  if (currentTargetImagePath.isEmpty()) {
    QMessageBox::warning(this, "Error", "Please select a target image first.");
    return;
  }

  clearResults();
  logConsole->append("Starting search...");
  outputBuffer.clear(); // Clear buffer
  searchButton->setEnabled(false);

  QString method = methodComboBox->currentText();
  QStringList args;
  QString program = "./bin/matcher";

  QString N = QString::number(nSpinBox->value());

  // Get weights
  QString W1 = QString::number(weightSpinBox1->value());
  QString W2 = QString::number(weightSpinBox2->value());
  QString W3 = QString::number(weightSpinBox3->value());

  // Note: The GUI runs from project root

  if (method == "Baseline") {
    // -d baseline:whole:ssd:<W>=data/fv_baseline_whole.csv
    args << "-t" << currentTargetImagePath;
    args << "-d" << "baseline:whole:ssd:" + W1 + "=data/fv_baseline_whole.csv";
    args << "-n" << N;
  } else if (method == "RG Histogram (rghist)") {
    // -d rghist2d:whole:hist_ix:<W>=data/fv_rghist2d_whole.csv
    args << "-t" << currentTargetImagePath;
    args << "-d"
         << "rghist2d:whole:hist_ix:" + W1 + "=data/fv_rghist2d_whole.csv";
    args << "-n" << N;
  } else if (method == "RGB Histogram (rgbhist)") {
    // -d rgbhist3d:whole:hist_ix:<W>=data/fv_rgbhist3d_whole.csv
    args << "-t" << currentTargetImagePath;
    args << "-d"
         << "rgbhist3d:whole:hist_ix:" + W1 + "=data/fv_rgbhist3d_whole.csv";
    args << "-n" << N;
  } else if (method == "Multi Histogram (multihist)") {
    // -d rgbhist3d:up:hist_ix:<W>=data/fv_rgbhist3d_up.csv
    // -d rgbhist3d:bottom:hist_ix:<W>=data/fv_rgbhist3d_bottom.csv
    args << "-t" << currentTargetImagePath;
    args << "-d" << "rgbhist3d:up:hist_ix:" + W1 + "=data/fv_rgbhist3d_up.csv";
    args << "-d"
         << "rgbhist3d:bottom:hist_ix:" + W2 + "=data/fv_rgbhist3d_bottom.csv";
    args << "-n" << N;
  } else if (method == "Multi Center Focus") {
    args << "-t" << currentTargetImagePath;
    args << "-d"
         << "rghist2d:center:hist_ix:" + W1 + "=data/fv_rghist2d_center.csv";
    args << "-d"
         << "rgbhist3d:whole:hist_ix:" + W2 + "=data/fv_rgbhist3d_whole.csv";
    args << "-d"
         << "cielab:center:hist_ix:" + W3 + "=data/fv_cielab_center.csv";
    args << "-n" << N;
  } else if (method == "DNN (SSD)") {
    args << "-t" << currentTargetImagePath;
    args << "-d" << "baseline:whole:ssd:" + W1 + "=data/ResNet18_olym.csv";
    args << "-n" << N;
  } else if (method == "CIE + Gabor") {
    args << "-t" << currentTargetImagePath;
    args << "-d" << "cielab:whole:hist_ix:" + W1 + "=data/fv_cielab_whole.csv";
    args << "-d" << "gabor:whole:cosine:" + W2 + "=data/fv_gabor_whole.csv";
    args << "-n" << N;
  }

  logConsole->append("Executing: " + program + " " + args.join(" "));
  process->start(program, args);
}

void MainWindow::handleProcessOutput() {
  QByteArray output = process->readAllStandardOutput();
  outputBuffer.append(output); // Buffer it
  logConsole->append(QString::fromLocal8Bit(output));
}

void MainWindow::handleProcessFinished(int exitCode,
                                       QProcess::ExitStatus exitStatus) {
  searchButton->setEnabled(true);
  if (exitStatus == QProcess::CrashExit) {
    logConsole->append("Process crashed!");
  } else {
    logConsole->append("Process finished with exit code " +
                       QString::number(exitCode));

    // Parse the full buffer
    QString outputStr = QString::fromLocal8Bit(outputBuffer);
    QTextStream stream(&outputStr);
    QString line;
    bool parsingMatches = false;
    int index = 0;

    while (stream.readLineInto(&line)) {
      if (line.startsWith("Top")) {
        parsingMatches = true;
        continue;
      }

      if (parsingMatches) {
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.size() >= 2) {
          QString filename = parts[0];
          bool ok;
          float distance = parts[1].toFloat(&ok);
          if (ok) {
            displayResult(filename, index++, distance);
          }
        }
      }
    }
  }
}

void MainWindow::clearResults() {
  QLayoutItem *child;
  while ((child = resultsLayout->takeAt(0)) != nullptr) {
    if (child->widget()) {
      delete child->widget();
    }
    delete child;
  }
}

void MainWindow::displayResult(const QString &imagePath, int index,
                               float distance) {
  QWidget *resultWidget = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout(resultWidget);

  QLabel *imgLabel = new QLabel();
  imgLabel->setFixedSize(200, 200);
  imgLabel->setScaledContents(true);

  // Handle relative paths. The CLI outputs paths relative to execution dir
  // (project root) e.g. "data/olympus/pic.1016.jpg"
  QString fullPath = QDir::currentPath() + "/" + imagePath;
  QPixmap pix(fullPath);
  if (!pix.isNull()) {
    imgLabel->setPixmap(pix.scaled(imgLabel->size(), Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation));
  } else {
    imgLabel->setText("Failed to load:\n" + imagePath);
  }

  QLabel *infoLabel = new QLabel(QString("Match #%1\nDist: %2\n%3")
                                     .arg(index + 1)
                                     .arg(distance)
                                     .arg(imagePath));
  infoLabel->setWordWrap(true);

  layout->addWidget(imgLabel);
  layout->addWidget(infoLabel);

  // Add to grid layout (e.g. 3 columns)
  int row = index / 3;
  int col = index % 3;
  // ... existing code ...
  resultsLayout->addWidget(resultWidget, row, col);
}

void MainWindow::updateWeightFields() {
  QString method = methodComboBox->currentText();
  bool showW1 = false;
  bool showW2 = false;
  bool showW3 = false;

  QString l1 = "W1";
  QString l2 = "W2";
  QString l3 = "W3";

  if (method == "Baseline") {
    // Single feature, no weight
  } else if (method == "RG Histogram (rghist)") {
    // Single feature, no weight
  } else if (method == "RGB Histogram (rgbhist)") {
    // Single feature, no weight
  } else if (method == "Multi Histogram (multihist)") {
    showW1 = true;
    showW2 = true;
    l1 = "rgbhist3d (up) weight";
    l2 = "rgbhist3d (bottom) weight";
  } else if (method == "Multi Center Focus") {
    showW1 = true;
    showW2 = true;
    showW3 = true;
    l1 = "rghist2d (center) weight";
    l2 = "rgbhist3d (whole) weight";
    l3 = "cielab (center) weight";
  } else if (method == "DNN (SSD)") {
    // Single feature, no weight
  } else if (method == "CIE + Gabor") {
    showW1 = true;
    showW2 = true;
    l1 = "cielab weight";
    l2 = "gabor weight";
  }

  w1Label->setVisible(showW1);
  w1Label->setText(l1 + ":");
  weightSpinBox1->setVisible(showW1);

  w2Label->setVisible(showW2);
  w2Label->setText(l2 + ":");
  weightSpinBox2->setVisible(showW2);

  w3Label->setVisible(showW3);
  w3Label->setText(l3 + ":");
  weightSpinBox3->setVisible(showW3);
}
