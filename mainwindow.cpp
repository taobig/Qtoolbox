#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "commonMethod.h"
#include "constant.h"
#include "mythread.h"
#include <QCheckBox>
#include <QClipboard>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGridLayout>
#include <QKeySequence>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTextCodec>
#include <QTextEdit>
#include <QThread>
#include <QTimer>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>

void MainWindow::cancelTimerSlot() {
  this->timer->stop();

  this->timerEdit->setReadOnly(false);
  this->timerContentEdit->setReadOnly(false);
  this->timerButton->setText(tr("Countdown Reminder"));
  disconnect(timerButton, SIGNAL(clicked()), this, SLOT(cancelTimerSlot()));
  connect(timerButton, SIGNAL(clicked()), this, SLOT(setTimerSlot()));
}
void MainWindow::setTimerSlot() {
  QString str = this->timerEdit->text();
  if (str == "" && str != "0")
    QMessageBox::about(this, tr("Error"), tr("Please enter a countdown time"));
  else {
    bool ok;
    int dec = str.toInt(&ok, 10);
    if (ok == true && dec != 0) {
      this->timerEdit->setReadOnly(true);
      this->timerContentEdit->setReadOnly(true);
      this->timerButton->setText(tr("Cancel countdown"));
      disconnect(timerButton, SIGNAL(clicked()), this, SLOT(setTimerSlot()));
      connect(timerButton, SIGNAL(clicked()), this, SLOT(cancelTimerSlot()));

      this->timer = new QTimer(this);
      connect(this->timer, SIGNAL(timeout()), this,
              SLOT(timeoutSlot())); // timeoutslot()为自定义槽
      dec = (dec * 1000 * 60);      // msec==>min
      // dec = (dec*1000);//msec==>seconds
      this->timer->start(dec);
      // int QTimer::start ( int msec, bool sshot = FALSE )
      //如果sshot为真，这个定时器将只会被激活一次，否则它将会持续到它被停止。
    } else {
      QMessageBox::about(this, tr("Countdown Reminder"),
                         tr("Please enter a countdown time"));
    }
  }
}

void MainWindow::timeoutSlot() {
  QMessageBox msgBox;
  msgBox.setText(timerContentEdit->text());
  msgBox.setInformativeText("<p style='color:red'>" + tr("Never remind?") +
                            "</p>");
  //  msgBox.setIconPixmap(QPixmap(":/images/xxxx.png"));
  QPushButton *abortButton = msgBox.addButton(tr("Yes"), QMessageBox::YesRole);
  QPushButton *continueButton = msgBox.addButton(tr("No"), QMessageBox::NoRole);
  //  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(abortButton);
  int ret = msgBox.exec();

  Q_UNUSED(ret);
  //    switch (ret) {
  //      case QMessageBox::Yes:
  //        qDebug("OK");
  //        this->timer->stop();

  //        break;
  //      case QMessageBox::No:
  //        qDebug("No");
  //          // Don't Save was clicked
  //          break;
  //      case QMessageBox::Cancel:
  //          // Cancel was clicked
  //          break;
  //      default:
  //          // should never be reached
  //          break;
  //    }
  //对于自定义的button
  if (msgBox.clickedButton() == abortButton) {
    qDebug("abort");
    cancelTimerSlot();
  } else if (msgBox.clickedButton() == continueButton) {
    qDebug("continue");
  }
}

void MainWindow::myMenuAct() {
  const QUrl AuthorUrl("http://localhost/");
  QDesktopServices::openUrl(AuthorUrl);
}

void MainWindow::MenuAbout() {
  AboutDialog aboutDlg;
  aboutDlg.exec();
}

void MainWindow::MenuAboutQt() { QMessageBox::aboutQt(this, "About QT"); }

void MainWindow::copyToClip() {
  QObject *send = QObject::sender();
  for (int i = 0; i < this->ipaddr_num; i++) {
    if (send == p[i].button) {
      QString str = p[i].line->text();
      QClipboard *cb = QApplication::clipboard();
      cb->setText(str);

      QMessageBox::information(this, tr("Success"),
                               tr("Copy successfully") + "\nIP:" + str);
      break;
    }
  }
}

MainWindow::MainWindow(QApplication *app, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QCoreApplication::setApplicationVersion(QString(APP_VERSION));
  this->setWindowTitle(tr(TARGET) + " - " + tr(APP_VERSION));
  this->myapp = app;

  //    ui->centralWidget->setFixedWidth(800);
  ui->centralWidget->setMinimumWidth(1024);
  QGridLayout *glayout = new QGridLayout(ui->centralWidget);

  QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
  QLabel *labelHostName = new QLabel(tr("Host Name:") + info.hostName());
  glayout->addWidget(labelHostName);

  QString sysUsername = qEnvironmentVariable("USER");
  if (sysUsername.isEmpty()) {
    sysUsername = qEnvironmentVariable("USERNAME");
  }
  glayout->addWidget(new QLabel(tr("User Name:") + sysUsername));

  // Important to note that the user home directory is not always the same as
  // username. – Linkan Jan 16 '19 at 9:30
  //    QString userDirName = QDir::home().dirName();
  //    if(sysUsername != userDirName){
  //        glayout->addWidget(new QLabel(tr("User Dir:")+userDirName));
  //    }
  QString userHomeDirPath = QDir::home().path();
  glayout->addWidget(new QLabel(tr("Home Path:") + userHomeDirPath));

  QTabWidget *tabWidget = new QTabWidget();
  glayout->addWidget(tabWidget);

  QWidget *tabWidget1 = new QWidget(tabWidget);
  QWidget *tabWidget2 = new QWidget(tabWidget);
  QWidget *tabWidget3 = new QWidget(tabWidget);
  //    QWidget* tabWidget4 = new QWidget(tabWidget);
  QGridLayout *tabLayout1 = new QGridLayout(tabWidget1);
  QGridLayout *tabLayout2 = new QGridLayout(tabWidget2);
  QGridLayout *tabLayout3 = new QGridLayout(tabWidget3);
  //    QGridLayout *tabLayout4 = new QGridLayout(tabWidget4);

  QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
  int i = 0; //可以用 this->ipaddr_num替换i
  this->ipaddr_num = 0;
  //    foreach(QHostAddress address, info.addresses()){}
  //    //在in.addresses()找主机的地址;在OSX下获取不到IP
  foreach (QHostAddress address, ipAddressesList) //在in.addresses()找主机的地址
  {
    if (address.protocol() == QAbstractSocket::IPv6Protocol) {
      continue;
    }
    if (address == QHostAddress::LocalHost) {
      continue;
    }
    if (address == QHostAddress::LocalHostIPv6) {
      continue;
    }

    QString str = address.toString();

    // QLineEdit *edit=new QLineEdit;
    p[i].line = new QLineEdit;
    // edit->setText(str);
    p[i].line->setText(str);
    p[i].line->setReadOnly(true);
    // layout->addWidget(edit);
    tabLayout1->addWidget(p[i].line, i + 6, 0);

    p[i].button = new QPushButton(tr("Copy"));
    tabLayout1->addWidget(p[i].button, i + 6, 1);

    connect(p[i].button, SIGNAL(clicked()), this, SLOT(copyToClip()));
    this->ipaddr_num++;
    i++;
  }

  this->tooltipLabel = new QLabel(tr(""));
  tabLayout1->addWidget(this->tooltipLabel, i + 7, 0);
  tabWidget->addTab(tabWidget1, tr("Machine Info"));

  QLabel *labelss = new QLabel(tr("Content of countdown reminder"));
  tabLayout2->addWidget(labelss, 1, 0, 1, 1);
  this->timerContentEdit = new QLineEdit;
  tabLayout2->addWidget(timerContentEdit, 1, 1, 1, 2);

  labelss = new QLabel(tr("Countdown time (in Muinutes)"));
  tabLayout2->addWidget(labelss, 2, 0, 1, 1);
  this->timerEdit = new QLineEdit;
  // timeEdit->setText(str);
  tabLayout2->addWidget(timerEdit, 2, 1, 1, 1);

  timerButton = new QPushButton(tr("Start"));
  tabLayout2->addWidget(timerButton, 3, 0, 1, 3);
  connect(timerButton, SIGNAL(clicked()), this, SLOT(setTimerSlot()));

  tabWidget->addTab(tabWidget2, tr("Countdown Reminder"));

  sourceTextEdit = new QPlainTextEdit(tabWidget3);
  sourceTextEdit->setPlaceholderText("input text");
  connect(sourceTextEdit, SIGNAL(textChanged()), this,
          SLOT(plainTextEditChanged()));
  selectFileButton = new QPushButton(tr("Select File"), tabWidget3);
  connect(selectFileButton, SIGNAL(clicked()), this,
          SLOT(selectFileButtonClicked()));
  selectedFile = new QLineEdit(tabWidget3);
  selectedFile->setReadOnly(true);
  selectedFile->setDisabled(true);
  QLabel *labelType = new QLabel(tr("Type"), tabWidget3);
  checkBoxMD5 = new QCheckBox("MD5", tabWidget3);
  checkBoxMD5->setChecked(true);
  checkBoxSHA1 = new QCheckBox("SHA1", tabWidget3);
  checkBoxSHA1->setChecked(true);
  checkBoxSHA256 = new QCheckBox("SHA256", tabWidget3);
  checkBoxSHA256->setChecked(true);
  //    checkBoxCRC32 = new QCheckBox("CRC32", tabWidget3);
  //    checkBoxCRC32->setChecked(true);

  //参考值
  QLabel *labelReferenceValue = new QLabel(tr("ReferenceValue"), tabWidget3);
  referenceValueForMD5 = new QLineEdit(tabWidget3);
  referenceValueForSHA1 = new QLineEdit(tabWidget3);
  referenceValueForSHA256 = new QLineEdit(tabWidget3);
  //    referenceValueForCRC32 = new QLineEdit(tabWidget3);

  QLabel *labelResult = new QLabel(tr("Result"), tabWidget3);
  compareResultForMD5 = new QLineEdit(tabWidget3);
  compareResultForMD5->setReadOnly(true);
  compareResultForSHA1 = new QLineEdit(tabWidget3);
  compareResultForSHA1->setReadOnly(true);
  compareResultForSHA256 = new QLineEdit(tabWidget3);
  compareResultForSHA256->setReadOnly(true);
  //    compareResultForCRC32 = new QLineEdit(tabWidget3);
  //    compareResultForCRC32->setReadOnly(true);

  int maxColumn = 3;
  int tabWidget3RowIndex = 0;
  tabLayout3->addWidget(sourceTextEdit, tabWidget3RowIndex, 0, 1, maxColumn);
  ++tabWidget3RowIndex;

  tabLayout3->addWidget(selectFileButton, tabWidget3RowIndex, 0, 1, maxColumn);
  ++tabWidget3RowIndex;

  tabLayout3->addWidget(selectedFile, tabWidget3RowIndex, 0, 1, maxColumn);
  ++tabWidget3RowIndex;

  tabLayout3->addWidget(labelType, tabWidget3RowIndex, 0);
  tabLayout3->addWidget(labelReferenceValue, tabWidget3RowIndex, 1);
  tabLayout3->addWidget(labelResult, tabWidget3RowIndex, 2);

  ++tabWidget3RowIndex;
  tabLayout3->addWidget(checkBoxMD5, tabWidget3RowIndex, 0);
  tabLayout3->addWidget(referenceValueForMD5, tabWidget3RowIndex, 1);
  tabLayout3->addWidget(compareResultForMD5, tabWidget3RowIndex, 2);
  ++tabWidget3RowIndex;
  tabLayout3->addWidget(checkBoxSHA1, tabWidget3RowIndex, 0);
  tabLayout3->addWidget(referenceValueForSHA1, tabWidget3RowIndex, 1);
  tabLayout3->addWidget(compareResultForSHA1, tabWidget3RowIndex, 2);
  ++tabWidget3RowIndex;
  tabLayout3->addWidget(checkBoxSHA256, tabWidget3RowIndex, 0);
  tabLayout3->addWidget(referenceValueForSHA256, tabWidget3RowIndex, 1);
  tabLayout3->addWidget(compareResultForSHA256, tabWidget3RowIndex, 2);
  ++tabWidget3RowIndex;
  //    tabLayout3->addWidget(checkBoxCRC32, tabWidget3RowIndex, 0);
  //    tabLayout3->addWidget(referenceValueForCRC32, tabWidget3RowIndex, 1);
  //    tabLayout3->addWidget(compareResultForCRC32, tabWidget3RowIndex, 2);
  //    ++tabWidget3RowIndex;

  calcStringHashButton = new QPushButton(tr("CALC String Hash"), tabWidget3);
  connect(calcStringHashButton, SIGNAL(clicked()), this,
          SLOT(calcStringHash()));
  calcStringHashButton->setFixedWidth(160);
  tabLayout3->addWidget(calcStringHashButton, tabWidget3RowIndex, 1, 1,
                        maxColumn, Qt::AlignRight);
  calcFileHashButton = new QPushButton(tr("CALC File Hash"), tabWidget3);
  connect(calcFileHashButton, SIGNAL(clicked()), this, SLOT(calcFileHash()));
  calcFileHashButton->setFixedWidth(120);
  tabLayout3->addWidget(calcFileHashButton, tabWidget3RowIndex, 1, 1, maxColumn,
                        Qt::AlignJustify);

  ++tabWidget3RowIndex;

  tabWidget->addTab(tabWidget3, tr("HASH Tools"));

  initDockerIcon();
  initTrayIcon();
  // QMenuBar *menuBar = new QMenuBar;
  // initMenu(menuBar);
  // glayout->setMenuBar(menuBar);
  initMenu(ui->menuBar);
  glayout->setMenuBar(ui->menuBar);

  this->show();
}

MainWindow::~MainWindow() {
  delete ui;
  delete quit;
  delete mini;
  delete restore;
  delete menuii;
  delete trayIcon;
}

void MainWindow::initDockerIcon() {
#ifdef Q_OS_MAC
  //    this->setWindowIcon(QIcon(":/icons/icon.icns"));
  //    this->centralWidget()->setWindowIcon(QIcon(":/icons/icon.icns"));
  myapp->setWindowIcon(QIcon(":/icons/icon.icns"));
#endif
}

void MainWindow::initTrayIcon() {
  trayIcon = new QSystemTrayIcon(QIcon(":/images/balloon.green.png"), this);
  menuii = new QMenu(this);
  restore = new QAction(tr("Restore"), this);
  mini = new QAction(tr("Minimize"), this);
  quit = new QAction(tr("Exit"), this);
  menuii->addAction(restore);
  menuii->addAction(mini);
  menuii->addAction(quit);
  trayIcon->setContextMenu(menuii);
  connect(restore, SIGNAL(triggered()), this, SLOT(restoreWindows()));
  connect(mini, SIGNAL(triggered()), this, SLOT(miniWindows()));
  connect(quit, SIGNAL(triggered()), this, SLOT(quitApplication()));
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(slot_trayiconDoubleClick(QSystemTrayIcon::ActivationReason)));
  trayIcon->show();
}

void MainWindow::initMenu(QMenuBar *menuBar) {
  QMenu *menu = new QMenu(tr("Menu"));
  menuBar->addMenu(menu);
  menu->addAction(tr("Quit"), this->myapp, SLOT(quit()));

  QMenu *menu2 = new QMenu(tr("Tools"));
  menuBar->addMenu(menu2);
  menu2->addAction(tr("Go to localhost"), this, SLOT(myMenuAct()));

  QMenu *menuHelp = new QMenu(tr("Help"));
  menuBar->addMenu(menuHelp);
  menuHelp->addAction(tr("About"), this, SLOT(MenuAbout()));
  menuHelp->addAction(tr("About QT"), this, SLOT(MenuAboutQt()));
}

void MainWindow::changeEvent(QEvent *event) {
  if (event->type() == QEvent::WindowStateChange) {
    if (windowState() & Qt::WindowMinimized) {
      QTimer::singleShot(0, this, SLOT(hide()));
    }
  } else if (event->type() == QEvent::Close) {
    qApp->quit();
  }
  QMainWindow::changeEvent(event);
}
void MainWindow::slot_trayiconDoubleClick(
    QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
  case QSystemTrayIcon::DoubleClick:
    this->show();
    this->setWindowState(Qt::WindowActive);
  default:;
  }
}
void MainWindow::miniWindows() { this->hide(); }
void MainWindow::restoreWindows() { showNormal(); }
void MainWindow::quitApplication() { qApp->quit(); }

void MainWindow::plainTextEditChanged() {
  compareResultForMD5->setText("");
  compareResultForSHA1->setText("");
  compareResultForSHA256->setText("");
  //    compareResultForCRC32->setText("");
}
void MainWindow::selectFileButtonClicked() {
  // QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
  // "/debug", tr("default (*.ini)"));
  QString file = QFileDialog::getOpenFileName(this, tr("Open File"));
  if (!file.isNull()) {
    this->selectedFile->setText(file);
  } else {
    // cancel选择
    //        criticalMessageBox(this,tr("Error"),  tr("select file error"));
  }
}
void MainWindow::calcStringHash() { this->calcHash(1); }

void MainWindow::calcFileHash() {
  if (selectedFile->text().length() == 0) {
    infoMessageBox(this, tr("Error"), tr("Please select file first"));
    return;
  }
  this->calcHash(2);
}

/**
 * @brief MainWindow::calcHash
 * @param sourceButton   0-calc string, 1-calc file
 */
void MainWindow::calcHash(int sourceButton) {
  setCalcButtonState(true);
  resetThreadInfo();
  plainTextEditChanged();

  if (sourceButton == 1) {
    QString sourceText = sourceTextEdit->toPlainText();
    if (this->checkBoxMD5->isChecked()) {
      QByteArray bb = stringChecksum(sourceText, QCryptographicHash::Md5);
      QString result = QString(bb.toHex()).toUpper();
      if (result == referenceValueForMD5->text().toUpper()) {
        compareResultForMD5->setText("OK");
      } else {
        compareResultForMD5->setText(result);
      }
    }
    if (this->checkBoxSHA1->isChecked()) {
      QByteArray bb = stringChecksum(sourceText, QCryptographicHash::Sha1);
      QString result = QString(bb.toHex()).toUpper();
      if (result == referenceValueForSHA1->text().toUpper()) {
        compareResultForSHA1->setText("OK");
      } else {
        compareResultForSHA1->setText(result);
      }
    }
    if (this->checkBoxSHA256->isChecked()) {
      QByteArray bb = stringChecksum(sourceText, QCryptographicHash::Sha256);
      QString result = QString(bb.toHex()).toUpper();
      if (result == referenceValueForSHA256->text().toUpper()) {
        compareResultForSHA256->setText("OK");
      } else {
        compareResultForSHA256->setText(result);
      }
    }
    //    if(this->checkBoxCRC32->isChecked()){

    //    }
    setCalcButtonState(false);
  } else if (sourceButton == 2) {
    if (this->checkBoxMD5->isChecked()) {
      if (calcFileMD5Thread == NULL) {
        calcFileMD5Thread = new MyThread();
        QObject::connect(calcFileMD5Thread,
                         SIGNAL(calcFinished(const char *, QString)), this,
                         SLOT(calcFileHashFinished(const char *, QString)));
      }
      calcFileMD5Thread->setParams(selectedFile->text(),
                                   QCryptographicHash::Md5);
      //            calcFileMD5Thread->start();
      ++this->activeThreadCounter;
    }
    if (this->checkBoxSHA1->isChecked()) {
      if (calcFileSHA1Thread == NULL) {
        calcFileSHA1Thread = new MyThread();
        QObject::connect(calcFileSHA1Thread,
                         SIGNAL(calcFinished(const char *, QString)), this,
                         SLOT(calcFileHashFinished(const char *, QString)));
      }
      calcFileSHA1Thread->setParams(selectedFile->text(),
                                    QCryptographicHash::Sha1);
      //            calcFileSHA1Thread->start();
      ++this->activeThreadCounter;
    }
    if (this->checkBoxSHA256->isChecked()) {
      if (calcFileSHA256Thread == NULL) {
        calcFileSHA256Thread = new MyThread();
        QObject::connect(calcFileSHA256Thread,
                         SIGNAL(calcFinished(const char *, QString)), this,
                         SLOT(calcFileHashFinished(const char *, QString)));
      }
      calcFileSHA256Thread->setParams(selectedFile->text(),
                                      QCryptographicHash::Sha256);
      //            calcFileSHA256Thread->start();
      ++this->activeThreadCounter;
    }
    //    if(this->checkBoxCRC32->isChecked()){

    //    }

    qDebug() << "activeThreadCounter:" << this->activeThreadCounter;
    if (this->checkBoxMD5->isChecked()) {
      calcFileMD5Thread->start();
    }
    if (this->checkBoxSHA1->isChecked()) {
      calcFileSHA1Thread->start();
    }
    if (this->checkBoxSHA256->isChecked()) {
      calcFileSHA256Thread->start();
    }
  }
}

void MainWindow::calcFileHashFinished(const char *hashAlgorithmName,
                                      QString result) {
  QMetaEnum metaEnum = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
  QCryptographicHash::Algorithm hashAlgorithm =
      QCryptographicHash::Algorithm(metaEnum.keyToValue(hashAlgorithmName));

  if (this->checkBoxMD5->isChecked() &&
      (hashAlgorithm == QCryptographicHash::Md5)) {
    if (!result.isEmpty()) {
      if (result == referenceValueForMD5->text().toUpper()) {
        compareResultForMD5->setText("OK");
      } else {
        compareResultForMD5->setText(result);
      }
      --this->activeThreadCounter;
    }
  }
  if (this->checkBoxSHA1->isChecked() &&
      (hashAlgorithm == QCryptographicHash::Sha1)) {
    if (!result.isEmpty()) {
      if (result == referenceValueForSHA1->text().toUpper()) {
        compareResultForSHA1->setText("OK");
      } else {
        compareResultForSHA1->setText(result);
      }
      --this->activeThreadCounter;
    }
  }
  if (this->checkBoxSHA256->isChecked() &&
      (hashAlgorithm == QCryptographicHash::Sha256)) {
    if (!result.isEmpty()) {
      if (result == referenceValueForSHA256->text().toUpper()) {
        compareResultForSHA256->setText("OK");
      } else {
        compareResultForSHA256->setText(result);
      }
      --this->activeThreadCounter;
    }
  }
  //    if(this->checkBoxCRC32->isChecked()){

  //    }

  qDebug() << "activeThreadCounter:" << this->activeThreadCounter;
  if (this->activeThreadCounter == 0) {
    setCalcButtonState(false);
  }
}

void MainWindow::setCalcButtonState(bool disable) {
  sourceTextEdit->setDisabled(disable);
  selectFileButton->setDisabled(disable);

  checkBoxMD5->setDisabled(disable);
  checkBoxSHA1->setDisabled(disable);
  checkBoxSHA256->setDisabled(disable);

  calcStringHashButton->setDisabled(disable);
  calcFileHashButton->setDisabled(disable);
}

void MainWindow::resetThreadInfo() { this->activeThreadCounter = 0; }
