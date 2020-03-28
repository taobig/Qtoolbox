
#include "mythread.h"
#include "commonMethod.h"
#include <QDebug>
#include <QMetaEnum>

MyThread::MyThread() {}

void MyThread::setParams(QString filename,
                         QCryptographicHash::Algorithm hashAlgorithm) {
  this->filename = filename;
  this->hashAlgorithm = hashAlgorithm;
}

void MyThread::run() {
  QByteArray hashResult = fileChecksum(filename, hashAlgorithm);
  qDebug() << "MyThread:" << hashAlgorithm << QString(hashResult.toHex());
  QString result = QString(hashResult.toHex()).toUpper();
  QMetaEnum metaEnum = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
  emit calcFinished(metaEnum.valueToKey(hashAlgorithm), result);
}
