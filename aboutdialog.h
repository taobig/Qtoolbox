#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>

class QWidget;
class QLabel;
class QLineEdit;
class QCheckBox;

class AboutDialog : public QDialog {
  Q_OBJECT

public:
  AboutDialog(QWidget *parent = 0);
  ~AboutDialog();

private:
  QString requestInfo;
  QVBoxLayout *layout;
  QLabel *labelCaption;
  QLabel *labelReadme;

signals:
  void myclose();
};
#endif // ABOUTDIALOG_H
