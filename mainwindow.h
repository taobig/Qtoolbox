#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lineedit.h"
#include "mythread.h"
#include <QCheckBox>
#include <QComboBox>
#include <QMainWindow>
#include <QMetaProperty> //for QMetaEnum
#include <QPlainTextEdit>
#include <QSystemTrayIcon>
#include <QTextEdit>

class QLineEdit;
class QLabel;
class QGridLayout;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QApplication *, QWidget *parent = 0);
  ~MainWindow();
  void initDockerIcon();
  void initTrayIcon();
  void initMenu(QMenuBar *menuBar);

private:
  void changeEvent(QEvent *); //窗口状态事件变化
  /**
   * @brief calcHash
   * @param sourceButton  0-calc string, 1-calc file
   */
  void calcHash(int sourceButton);
  void setCalcButtonState(bool disable);
  void resetThreadInfo();

private:

  const QString UNIT_MINUTE = "minute";
  const QString UNIT_SECOND = "second";

  Ui::MainWindow *ui;
  QMenu *menuii;
  QAction *restore;
  QAction *mini;
  QAction *quit;
  QSystemTrayIcon *trayIcon;

  LineEdit p[20]; // LineEdit自定义的一个类
  QLabel *tooltipLabel;
  QWidget *widget;

  QApplication *myapp;
  int ipaddr_num;
  QLineEdit *timerEdit;
  QComboBox *countdownUnitComboBox;
  QLineEdit *timerContentEdit;
  QPushButton *timerButton;

  QTimer *timer;

  QPlainTextEdit *sourceTextEdit;
  QPushButton *selectFileButton;
  QLineEdit *selectedFile;
  QCheckBox *checkBoxMD5;
  QCheckBox *checkBoxSHA1;
  QCheckBox *checkBoxSHA256;
  QCheckBox *checkBoxCRC32;
  QLineEdit *referenceValueForMD5;
  QLineEdit *referenceValueForSHA1;
  QLineEdit *referenceValueForSHA256;
  QLineEdit *referenceValueForCRC32;
  QLineEdit *compareResultForMD5;
  QLineEdit *compareResultForSHA1;
  QLineEdit *compareResultForSHA256;
  QLineEdit *compareResultForCRC32;

  QPushButton *calcStringHashButton;
  QPushButton *calcFileHashButton;

  MyThread *calcFileMD5Thread = NULL;
  MyThread *calcFileSHA1Thread = NULL;
  MyThread *calcFileSHA256Thread = NULL;
  int activeThreadCounter = 0;

signals:
  void showSignal(QString str);

public slots:
  void copyToClip();
  void timeoutSlot();
  void setTimerSlot();
  void cancelTimerSlot();

  void myMenuAct();
  void MenuAbout();
  void MenuAboutQt();

  void calcFileHashFinished(const char *, QString);

private slots:
  void slot_trayiconDoubleClick(QSystemTrayIcon::ActivationReason reason);
  void miniWindows();     //最小化窗口
  void restoreWindows();  //还原窗口
  void quitApplication(); //退出系统
  void plainTextEditChanged();
  void selectFileButtonClicked();
  void calcStringHash();
  void calcFileHash();
};

#endif // MAINWINDOW_H
