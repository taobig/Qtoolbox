#include "aboutdialog.h"

#include "constant.h"
#include <QCheckBox>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTextCodec>
#include <QVBoxLayout>

AboutDialog::AboutDialog(QWidget *parent) {
  Q_UNUSED(parent);

  labelCaption = NULL;
  labelReadme = NULL;
  setWindowTitle(tr("About"));

  this->layout = new QVBoxLayout;
  labelCaption = new QLabel(tr(TARGET) + " " + APP_VERSION);
  labelCaption->setAlignment(Qt::AlignCenter);
  labelCaption->setStyleSheet(
      "color:black;font-size:20px;font-weight:bold;margin:20px 0;");
  this->layout->addWidget(labelCaption);
  this->layout->addWidget(new QLabel(
      QString("%1 %2").arg(tr("Release Date:"), APP_VERSION_RELEASE_TIME)));
  QLabel *label = new QLabel(
      QString("<a href=\"%1\">%2</a>").arg(feedback_url, feedback_url));
  label->setTextFormat(Qt::RichText);
  label->setTextInteractionFlags(Qt::TextBrowserInteraction);
  label->setOpenExternalLinks(true);
  this->layout->addWidget(label);

  this->setMinimumWidth(400);
  this->setMinimumHeight(200);

  this->setLayout(layout);
}

AboutDialog::~AboutDialog() {
  delete labelCaption;
  labelCaption = NULL;
}
