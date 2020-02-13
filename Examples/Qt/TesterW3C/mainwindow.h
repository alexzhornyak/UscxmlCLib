#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTime>

#include "UscxmlCLib.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void Log(const QString &sMsg, const int iSeverity);

    void InterpreterStopped(QListWidgetItem *item, const bool pass);

private slots:
    void on_btnStop_clicked();

    void on_btnStart_clicked();

    void on_btnReset_clicked();

private:
    Ui::MainWindow *ui;

    UsclibInterpreter *_interpreter = nullptr;

    enum TestState {
        tsSuccess,
        tsError,
        tsStarted,
        tsUnknown,
        tsManual,
        tsMAXSIZE
    };

    QIcon _icon[tsMAXSIZE];

    QTime _elapsed;

    void interpreterStartNext();

    void setItemState(QListWidgetItem *item, const TestState state);
};

Q_DECLARE_METATYPE(QListWidgetItem*)
Q_DECLARE_METATYPE(MainWindow*)

#endif // MAINWINDOW_H
