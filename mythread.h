#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QCryptographicHash>
#include <QThread>
class MyThread : public QThread {

  Q_OBJECT

private:
  QString filename = NULL;
  QCryptographicHash::Algorithm hashAlgorithm = QCryptographicHash::Md5;

public:
  MyThread();
  void setParams(QString filename, QCryptographicHash::Algorithm hashAlgorithm);
  virtual void run();

signals:
  void calcFinished(const char *, QString);
};

#endif // MYTHREAD_H
