
#include "commonMethod.h"
#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

int TwriteFile(QString fileName, QString qstr) {
  QFile file(fileName);
  if (!file.open(QIODevice::Append | QIODevice::Text))
    return 1;
  /*
QByteArray line;
      while (!file.atEnd()) {
          //QByteArray line = file.readLine();
          line = file.readLine();
          //process_line(line);
      }
      */
  QDateTime dateTime = QDateTime::currentDateTime();
  QString qStrTime = dateTime.toString("yyyy-MM-dd hh:mm:ss ");
  const char *str = qPrintable(qStrTime + qstr + "\r\n");
  file.write(str);

  file.close();

  /*
f.open(IO_);//一般不要IO_ReadWrite,很容易出现赃数据
//如果要在文件的后面添加内容要IO_WriteOnly|IO_Append
//如果要清空原来的内容,只要IO_WriteOnly
           //IO_Translate用来读windows文件,linux下的回车换行是\n,windows下是\r\n
2.写文件

QTextStream s( &f );
s<<"Hello,world!!!"<<"\n";
3.读一行文件
readLine(),每读一行自动下移一行
atEnd()判断是否到文件末尾
4.关闭文件f.close();
*/
  return 0;
}

void criticalMessageBox(QWidget *parent, QString title, QString content) {
  QMessageBox::critical(parent, title, content);
}

void infoMessageBox(QWidget *parent, QString title, QString content) {
  QMessageBox::information(parent, title, content);
}

QByteArray stringChecksum(const QString string,
                          QCryptographicHash::Algorithm hashAlgorithm) {
  QByteArray ba, bb;
  QCryptographicHash md(hashAlgorithm);
  ba.append(string.toUtf8());
  md.addData(ba);
  return md.result();
}

QByteArray fileChecksum(const QString &fileName,
                        QCryptographicHash::Algorithm hashAlgorithm) {
  QFile f(fileName);
  if (f.open(QFile::ReadOnly)) {
    QCryptographicHash hash(hashAlgorithm);
    if (hash.addData(&f)) {
      return hash.result();
    }
  }
  return QByteArray();
}
