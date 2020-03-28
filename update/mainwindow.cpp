#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTextCodec>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>


#include <QFile>
#include <QProcess>

void MainWindow::goQuit()
{
    //QStringList qlist(update_exe_file+qStr_extension_name);
    //QProcess::startDetached( this->destName, qlist);
    //可以带参数表示更新后第一次启动
    QProcess::startDetached( this->destName);
    //app->exit(0);
   // app->quit();
   QApplication::exit(0);
}

MainWindow::MainWindow(QApplication *app, QString tempFileName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);




    QTextCodec* codec = QTextCodec::codecForName("UTF8");
    //QMessageBox::information(0, codec->toUnicode("更新成功 "), tempFileName);


    this->setWindowTitle(codec->toUnicode("正在更新"));
    this->show();

    QVBoxLayout *layout=new QVBoxLayout;
    if(tempFileName == "")
    {
        //QMessageBox::information(0, codec->toUnicode("error"), "none"+tempFileName);
        QLabel *label=new QLabel(codec->toUnicode("我很脆弱的，请不要这么操作") );
        layout->addWidget(label);
        QWidget *widget = new QWidget(this);
        widget->setLayout(layout);
        setCentralWidget(widget);
    }
    else
    {

        QString strInfo;
        QFile updateFile(tempFileName);
        QString ff(tempFileName);
        int i = ff.indexOf('.',0);
        i = ff.indexOf('.',i+1);
        this->destName = ff.mid(0, i);

        QFile oldFile(this->destName);
        oldFile.rename("goo");
        if( updateFile.rename(this->destName) )
        {
            updateFile.close();
            oldFile.remove();
            oldFile.close();
            strInfo = codec->toUnicode("更新成功");
            //QMessageBox::information(0, codec->toUnicode("更新成功"), codec->toUnicode("更新成功") );
        }
        else
        {
            updateFile.remove();
            updateFile.close();
            oldFile.rename(this->destName);
            oldFile.close();
            strInfo = codec->toUnicode("更新失败,请重新尝试");
            QMessageBox::information(0, codec->toUnicode("更新失败"), codec->toUnicode("更新失败,请重新尝试"));
        }


        QLabel *label=new QLabel( strInfo);
        layout->addWidget(label);

        QWidget *widget = new QWidget(this);

        QPushButton *btn = new QPushButton(codec->toUnicode("关闭更新管理器并重启启动应用程序"));
        layout->addWidget(btn);
        //不用new，则pushButton不能生成。。为什么？？？
        //QPushButton btn(strInfo);
        //layout->addWidget(&btn);

       widget->setLayout(layout);
       setCentralWidget(widget);


       connect(btn, SIGNAL(clicked()), this, SLOT(goQuit()) );

    /*
        QVBoxLayout *layout=new QVBoxLayout;
        //QLineEdit *edit=new QLineEdit;
        //edit->setText(str);
        //layout->addWidget(edit);

        QLabel *label=new QLabel(tempFileName);//codec->toUnicode("选择你要复制的ip地址，点击对应ip地址的下方的[复制]按钮。"));
        layout->addWidget(label);

       QMenuBar *menuBar = new QMenuBar;
       QMenu* menu = new QMenu(codec->toUnicode("菜单"));
       menuBar->addMenu(menu);
       //menu->addAction( codec->toUnicode("退出程序"), app, SLOT(quit()) );

       //QAction *action = menuBar->addAction( codec->toUnicode("开始计时(计时功能)"), this, SLOT(myMenuAct()) );
       menuBar->addAction( codec->toUnicode("开始计时(计时功能)"), this, SLOT(myMenuAct()) );

       //QAction *actionChat = menuBar->addAction( codec->toUnicode("开启聊天"), this, SLOT(MenuChat()) );
       menuBar->addAction( codec->toUnicode("开启聊天"), this, SLOT(MenuChat()) );


       QMenu* menuHelp = new QMenu(codec->toUnicode("帮助"));
       menuBar->addMenu(menuHelp);
       //menu->addAction( codec->toUnicode(""), app, SLOT(quit()) );
       //QAction *actionFeedback = menuHelp->addAction( codec->toUnicode("反馈建议"), this, SLOT(MenuFeedback()) );
       //QAction *actionAbout = menuHelp->addAction( codec->toUnicode("关于"), this, SLOT(MenuAbout()) );
       menuHelp->addAction( codec->toUnicode("反馈建议"), this, SLOT(MenuFeedback()) );
       menuHelp->addAction( codec->toUnicode("关于"), this, SLOT(MenuAbout()) );
       menuHelp->addAction( codec->toUnicode("关于QT"), this, SLOT(MenuAboutQt() ) );

       layout->setMenuBar(menuBar);



       QWidget *widget = new QWidget(this);
       widget->setLayout(layout);
       setCentralWidget(widget);
    */
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}
