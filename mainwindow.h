#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QFileSystemWatcher"
#include "inotifier.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

public slots:
    void
    fileChanged(const QString&);


private slots:
    void
    on_pushButton_2_clicked();

private:
    Ui::MainWindow * ui;
    INotifier i;
};

#endif // MAINWINDOW_H
