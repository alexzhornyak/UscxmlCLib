#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QBitmap>

enum {
    testRolesSpecial = Qt::UserRole,
    testRolesState,
    testRolesPath,
    testRolesMainWindow
};

//---------------------------------------------------------------------------
QPixmap ConvertBmpWithAlpha(const QString &sPath) {
    QImage img(sPath);
    img.setAlphaChannel(img.createMaskFromColor(img.pixel(0,0), Qt::MaskOutColor));
    return QPixmap::fromImage(img);
}

//---------------------------------------------------------------------------
void __stdcall OnInterpreterLog(const int nSeverity, const char *chMessage, void *AUser) {
    MainWindow *wnd = static_cast<MainWindow*>(AUser);
    wnd->metaObject()->invokeMethod(wnd, "Log", Q_ARG(QString, chMessage), Q_ARG(int, nSeverity));
}

//---------------------------------------------------------------------------
void __stdcall OnInterpreterStopped(const UsclibInterpreter *AInterpreter, void *AUser) {
    bool bPass = false;
    usclib_IsInterpreterInState(AInterpreter, "pass", &bPass);

    QListWidgetItem *item = static_cast<QListWidgetItem*>(AUser);
    MainWindow *wnd = item->data(testRolesMainWindow).value<MainWindow *>();

    wnd->metaObject()->invokeMethod(wnd, "InterpreterStopped", Q_ARG(QListWidgetItem*, item),
                                    Q_ARG(bool, bPass));
}

//---------------------------------------------------------------------------
//------------------------------- MainWindow --------------------------------
//---------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i=0; i < MainWindow::tsMAXSIZE; i++) {
        _icon[i] = ConvertBmpWithAlpha(":/Res/img" + QString::number(i*2) + ".bmp");
    }

    try {

        // manual or restricted tests
        QStringList ASpecialTests{
            "test230.scxml", // manual
            "test250.scxml", // manual
            "test301.scxml", // rejected script src
            "test307.scxml", // manual
            "test415.scxml", // halt processing when final is initial
            "test530.scxml", // XML Nodes in script expressions not supported
            "test562.scxml" // content without evaluation not supported (only valid expressions)
        };

        const QDir dirW3C = QCoreApplication::applicationDirPath() +
                "\\..\\..\\..\\StateCharts\\W3C\\Lua";

        if (!dirW3C.exists())
            throw std::runtime_error("Directory [" + dirW3C.path().toStdString() + "] does not exist!");

        QFileInfo infoLog(QCoreApplication::applicationFilePath());
        const QString sLogFile = infoLog.path() + "\\" + infoLog.baseName() + ".scxml.log";
        if (USCLIB_SUCCESS != usclib_InitLogging(sLogFile.toStdString().c_str(), OnInterpreterLog, this))
            throw std::runtime_error(usclib_GetLastError());

        if (USCLIB_SUCCESS != usclib_InitHTTP(7080, 7443))
            throw std::runtime_error(usclib_GetLastError());

        for (const auto &it : dirW3C.entryInfoList(QStringList{"*.scxml"}, QDir::Files)) {
            const QString sTestFile = it.fileName();
            if (!sTestFile.contains("sub",Qt::CaseInsensitive)) {
                QListWidgetItem *item = new QListWidgetItem(
                            _icon[MainWindow::tsUnknown], sTestFile, ui->listTests);
                item->setData(testRolesState, MainWindow::tsUnknown);
                item->setData(testRolesPath, QDir::toNativeSeparators(it.absoluteFilePath()));
                item->setData(testRolesMainWindow, QVariant::fromValue(this));
                // mark warning or restricted tests
                if (ASpecialTests.contains(sTestFile, Qt::CaseInsensitive)) {
                    item->setData(testRolesSpecial, true);
                }
                ui->listTests->addItem(item);
            }
        }

        ui->progressBar->setMaximum(ui->listTests->count());

        ui->btnStart->setEnabled(true);
    }
    catch(std::exception &e) {
        Log(QString("ERROR> %1").arg(e.what()), USCLIB_LOG_ERROR);
    }
}

MainWindow::~MainWindow()
{
    if (_interpreter) {
        usclib_CloseInterpreter(_interpreter);
    }

    usclib_GlobalCleanup();
    delete ui;
}

void MainWindow::Log(const QString &sMsg, const int iSeverity)
{
    switch(iSeverity) {
    case USCLIB_LOG_INFO: {
        ui->editLog->setTextColor(QColor("blue"));
        ui->editLog->setFontWeight(QFont::Normal);

    }break;
    case USCLIB_LOG_WARN: {
        ui->editLog->setTextColor(QColor("navy"));
        ui->editLog->setFontWeight(QFont::Bold);

    }break;
    case USCLIB_LOG_ERROR: {
        ui->editLog->setTextColor(QColor("red"));
        ui->editLog->setFontWeight(QFont::Bold);

    }break;
    default: {
        ui->editLog->setTextColor(QColor("green"));
        ui->editLog->setFontWeight(QFont::Normal);
    }
    }
    ui->editLog->append(sMsg);
}

void MainWindow::setItemState(QListWidgetItem *item, const MainWindow::TestState state)
{
    item->setData(testRolesState, state);
    item->setIcon(_icon[state]);
}

void MainWindow::InterpreterStopped(QListWidgetItem *item, const bool pass) {

    const bool bUserCancelled = !ui->btnStop->isEnabled();

    item->listWidget()->setCurrentItem(item);

    if (!bUserCancelled) {
        if (item->data(testRolesSpecial).toBool()) {
            setItemState(item, MainWindow::tsManual);
        }
        else {
            setItemState(item, pass ? MainWindow::tsSuccess : MainWindow::tsError);
        }

        ui->progressBar->setValue(ui->progressBar->value() + 1);

        if (ui->progressBar->value() == ui->progressBar->maximum()) {

            on_btnStop_clicked();

            int iPassed = 0;
            int iManual = 0;
            int iNotPassed = 0;

            const int iTotalCount = ui->listTests->count();

            for (int i = 0; i < iTotalCount; i++) {
                switch(ui->listTests->item(i)->data(testRolesState).toInt()) {
                case MainWindow::tsSuccess:
                    iPassed++;
                    break;
                case MainWindow::tsManual:
                    iManual++;
                    break;
                default:
                    iNotPassed++;
                }
            }

            Log("Elapsed: "  +
                QTime::fromMSecsSinceStartOfDay(_elapsed.msecsTo(QTime::currentTime())).
                                                toString("hh:mm:ss.zzz"), USCLIB_LOG_INFO);
            Log("All " + QString::number(iTotalCount) + " tests were completed!", USCLIB_LOG_INFO);
            Log("Passed: " + QString::number(iPassed), USCLIB_LOG_INFO);
            Log("Manual or restricted: " + QString::number(iManual), USCLIB_LOG_INFO);
            Log("Failed: " + QString::number(iNotPassed), iNotPassed ? USCLIB_LOG_ERROR : USCLIB_LOG_INFO);
        }
        else {
            interpreterStartNext();
        }
    } else {
        Log("User cancelled!", USCLIB_LOG_WARN);
    }
}

void MainWindow::interpreterStartNext()
{
    try {
        for (int i = 0; i < ui->listTests->count(); i++) {
            if (ui->listTests->item(i)->data(testRolesState).toInt() == MainWindow::tsUnknown || //
                    ui->listTests->item(i)->data(testRolesState).toInt() == MainWindow::tsStarted) {

                setItemState(ui->listTests->item(i), MainWindow::tsStarted);

                usclib_RegisterInterpreterStoppedCallback(_interpreter, OnInterpreterStopped,
                                                          ui->listTests->item(i));

                Log("Starting " + ui->listTests->item(i)->text() + "...", USCLIB_LOG_INFO);

                if (ui->listTests->item(i)->data(testRolesSpecial).toBool()) {
                    Log("Test is manual or restricted!", USCLIB_LOG_WARN);
                }

                if (USCLIB_SUCCESS != usclib_StartInterpreter(_interpreter,
                    ui->listTests->item(i)->data(testRolesPath).toString().toStdString().c_str(),USCLIB_SCXML_AS_FILE)) {

                    setItemState(ui->listTests->item(i), MainWindow::tsError);

                    throw std::runtime_error(usclib_GetLastError());
                }

                break;
            }
        }
    }
    catch(std::exception &e) {
        Log(QString("ERROR> %1").arg(e.what()), USCLIB_LOG_ERROR);
    }
}

void MainWindow::on_btnStart_clicked()
{
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);

    try {
        if (!_interpreter) {
            // some test have planned errors, so skip interpreter issue checks,
            // which will terminate execution
            UsclibInterpreterOptions AInterpreterOptions;
            if (USCLIB_SUCCESS != usclib_GetDefaultInterpreterOptions(&AInterpreterOptions))
                throw std::runtime_error(usclib_GetLastError());
            AInterpreterOptions.Validate = false;
            AInterpreterOptions.AsyncStart = true;

            if (USCLIB_SUCCESS != usclib_OpenInterpreter(&_interpreter, 0, 0, &AInterpreterOptions))
                throw std::runtime_error(usclib_GetLastError());
        }

        _elapsed = QTime::currentTime();
        interpreterStartNext();
    }
    catch(std::exception &e) {
        Log(QString("ERROR> %1").arg(e.what()), USCLIB_LOG_ERROR);
    }
}

void MainWindow::on_btnStop_clicked()
{
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);

    try {
        if (_interpreter) {
            if (USCLIB_SUCCESS != usclib_StopInterpreter(_interpreter))
                throw std::runtime_error(usclib_GetLastError());
        }
    }
    catch(std::exception &e) {
        Log(QString("ERROR> %1").arg(e.what()), USCLIB_LOG_ERROR);
    }
}



void MainWindow::on_btnReset_clicked()
{
    for (int i = 0; i < ui->listTests->count(); i++) {
         setItemState(ui->listTests->item(i), MainWindow::tsUnknown);
    }
    ui->editLog->clear();
    ui->progressBar->setValue(0);
}
