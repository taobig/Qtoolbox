#ifndef COMMONMETHOD_H
#define COMMONMETHOD_H

#include <QString>
#include <QWidget>
#include <QCryptographicHash>


int TwriteFile(QString fileName, QString qstr);
void criticalMessageBox(QWidget *parent, QString title, QString content);
void infoMessageBox(QWidget *parent, QString title, QString content);
QByteArray stringChecksum(const QString string, QCryptographicHash::Algorithm hashAlgorithm);
QByteArray fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm);



#endif // COMMONMETHOD_H
