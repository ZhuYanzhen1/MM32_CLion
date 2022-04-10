#include "mainwindow.h"
#include <QApplication>
#include <QMutex>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type) {
        case QtCriticalMsg:
            text = QString("Critical ");
            break;
        case QtFatalMsg:
            text = QString("Fatal ");
            break;
        default:
        case QtDebugMsg:
            text = QString("Debug ");
            break;
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("MM-dd hh:mm:ss");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2: %3").arg(text).arg(current_date).arg(msg);

    QFile file(QCoreApplication::applicationDirPath() + "/log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion("1.3");
    qInstallMessageHandler(outputMessage);
    MainWindow w;
    w.show();
    return a.exec();
}
