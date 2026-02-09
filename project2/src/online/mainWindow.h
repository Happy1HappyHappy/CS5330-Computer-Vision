#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QProcess>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void browseImage();
  void runSearch();
  void updateWeightFields();
  void handleProcessOutput();
  void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
  void setupUI();
  void displayResult(const QString &imagePath, int index, float distance);
  void clearResults();

  // UI Elements
  QLabel *targetImageLabel;
  QLabel *targetImagePathLabel;
  QComboBox *methodComboBox;
  QSpinBox *nSpinBox;
  QDoubleSpinBox *weightSpinBox1; // W1
  QDoubleSpinBox *weightSpinBox2; // W2
  QDoubleSpinBox *weightSpinBox3; // W3
  QLabel *w1Label;
  QLabel *w2Label;
  QLabel *w3Label;
  QPushButton *browseButton;
  QPushButton *searchButton;
  QWidget *resultsContainer;
  QGridLayout *resultsLayout;
  QTextEdit *logConsole;

  // Process
  QProcess *process;
  QString currentTargetImagePath;
  QByteArray outputBuffer;
};

#endif // MAINWINDOW_H
