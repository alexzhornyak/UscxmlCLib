#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "UscxmlCLib.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public slots:

    void SetOutput(const QString &sName, const QString &sData);

private slots:

    void on_dial_valueChanged(int value);

    void on_pushBtnSupply_clicked(bool checked);

    void on_pushButton_pressed();

    void on_pushButton_released();

private:
    Ui::MainWindow *ui;

    UsclibInterpreter *_interpreter = nullptr;
    QSystemTrayIcon _tray;
};

#endif // MAINWINDOW_H
