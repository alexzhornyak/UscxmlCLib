#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>

//---------------------------------------------------------------------------
void __stdcall OnInterpreterGlobalDataChange(const UsclibInterpreter *AInterpreter,
    const char *chName, const char *chPath,
    const char *chAtomOrJsonData, const bool bIsAtomOrJson, const int iType, void *AUser) {

    // Type = 0 - global data changed from App
    // Type = 1 - global data changed inside Scxml

    if (iType == 1) {
        MainWindow *wnd = static_cast<MainWindow*>(AUser);

        // synchronize callback with GUI thread
        wnd->metaObject()->invokeMethod(wnd, "SetOutput",
             Q_ARG(QString, chName), Q_ARG(QString, chAtomOrJsonData));
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _tray.setIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
    _tray.show();

    // bind SCXML inputs
    for (auto it : this->findChildren<QPushButton*>()) {
        if (!it->isCheckable()) {
            connect(it, &QPushButton::pressed, this, &MainWindow::on_pushButton_pressed);
            connect(it, &QPushButton::released, this, &MainWindow::on_pushButton_released);
        }
    }

    // make all outputs hidden by default
    for (auto it : ui->frameOutputs->findChildren<QLabel*>()) {
        it->setVisible(false);
    }

    for (auto it : ui->frameOutputs->findChildren<QLCDNumber*>()) {
        it->display("");
    }

    // load SCXML
    try {
        const QString sScxmlFile = QCoreApplication::applicationDirPath() +
                "\\..\\..\\..\\StateCharts\\KT76C.scxml";

        QFileInfo info(QCoreApplication::applicationFilePath());
        const QString sLogFile = info.path() + "\\" + info.baseName() + ".scxml.log";

        if (USCLIB_SUCCESS != usclib_InitLogging(sLogFile.toStdString().c_str(), NULL, NULL))
            throw std::runtime_error(usclib_GetLastError());

        // enable RemoteMonitor
        UsclibInterpreterOptions AInterpreterOptions;
        usclib_GetDefaultInterpreterOptions(&AInterpreterOptions);
        AInterpreterOptions.RemoteMonitorPort = USCLIB_DEFAULT_SCXMLEDITOR_MONITOR;
        AInterpreterOptions.Validate = false;

        if (USCLIB_SUCCESS != usclib_OpenInterpreter(&_interpreter, 0, 0, &AInterpreterOptions))
            throw std::runtime_error(usclib_GetLastError());

        usclib_RegisterInterpreterGlobalDataChangeCallback(_interpreter, OnInterpreterGlobalDataChange, USCLIB_DATATYPE_ATOM, this);

        if (USCLIB_SUCCESS != usclib_StartInterpreter(_interpreter,
            sScxmlFile.toStdString().c_str(), USCLIB_SCXML_AS_FILE))
            throw std::runtime_error(usclib_GetLastError());
    }
    catch(std::exception &e) {
        _tray.showMessage("ERROR", e.what(), QSystemTrayIcon::Critical);
    }

}

MainWindow::~MainWindow()
{
    if (_interpreter) {
        // we generate 'Quit' event to give State Chart an option to save settings
        usclib_TriggerEvent(_interpreter, "Quit");
        usclib_WaitForInterpreterStopped(_interpreter);

        usclib_CloseInterpreter(_interpreter);
    }

    usclib_GlobalCleanup();

    delete ui;
}

void MainWindow::SetOutput(const QString &sName, const QString &sData)
{
    for (auto it : ui->frameOutputs->findChildren<QLCDNumber*>()) {
        if (it->toolTip() == sName) {
            it->display(sData);
        }
    }

    for (auto it : ui->frameOutputs->findChildren<QLabel*>()) {
        if (it->toolTip() == sName) {
            it->setVisible(sData.toInt());
        }
    }
}

void MainWindow::on_dial_valueChanged(int value)
{
    if (_interpreter) {
        if (auto dial = qobject_cast<QDial*>(sender())) {
            usclib_TriggerIntEvent(_interpreter, dial->toolTip().toStdString().c_str(), value);
        }
    }
}

void MainWindow::on_pushBtnSupply_clicked(bool checked)
{
    if (_interpreter) {
        if (auto btn = qobject_cast<QPushButton*>(sender())) {
            usclib_TriggerIntEvent(_interpreter, btn->toolTip().toStdString().c_str(), checked ? 1 : 0);
        }
    }
}

void MainWindow::on_pushButton_pressed()
{
    /* we are giving an option to press a couple of buttons */

    /* when pressed, set a flag, later we will use it for handle CTRL case */
    auto btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        if (!btn->property("Pressed").toBool()) {
            btn->setProperty("Pressed", true);

            btn->setStyleSheet("color: red;");

            if (_interpreter) {
                usclib_TriggerIntEvent(_interpreter, btn->toolTip().toStdString().c_str(), 1);
            }
        }
    }
}

void MainWindow::on_pushButton_released()
{
    /* when released with CTRL button, then do not send message to State Machine */
    auto btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        const bool bCtrlPressed = QApplication::keyboardModifiers() & Qt::ControlModifier;
        if (btn->property("Pressed").toBool() && !bCtrlPressed) {
            btn->setProperty("Pressed", false);

            btn->setStyleSheet("color: black;");

            if (_interpreter) {
                usclib_TriggerIntEvent(_interpreter, btn->toolTip().toStdString().c_str(), 0);
            }
        }
    }
}
