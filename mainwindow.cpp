#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <qdebug.h>
#include <qfile.h>
#include <qdir.h>
#include <qprocess.h>
#include "inotifier.h"
MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "test";
    QStringList files;
    files << "./test";
    i.monitor(files);

    QObject::connect(&i, SIGNAL(fileChange(const QString&)), this, SLOT(fileChanged(const QString&)));
    i.start();
}

void
MainWindow::fileChanged(const QString& folder)
{
    qDebug() << folder;
    QFile f(folder);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&f);
    QString t = stream.readAll();
    qDebug() << "content" << t;
    qDebug() << f.remove();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::on_pushButton_2_clicked()
{
    //    QProcess
}
