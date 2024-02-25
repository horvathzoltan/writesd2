#include <QCoreApplication>
#include "helpers/logger.h"
#include "helpers/signalhelper.h"
//#include "helpers/commandlineparserhelper.h"
#include "helpers/coreappworker.h"

#include "work1.h"
#include "helpers/stringify.h"

auto main(int argc, char *argv[]) -> int
{
#if defined (STRING) && defined (TARGI)
    auto target = STRING(TARGI);
#else
    auto target=QStringLiteral("ApplicationNameString");
#endif
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(target);
    QCoreApplication::setApplicationVersion("1");
    QCoreApplication::setOrganizationName("LogControl Kft.");
    QCoreApplication::setOrganizationDomain("https://www.logcontrol.hu/");

    SignalHelper::setShutDownSignal(SignalHelper::SIGINT_); // shut down on ctrl-c
    SignalHelper::setShutDownSignal(SignalHelper::SIGTERM_); // shut down on killall
    Logger::Init(Logger::ErrLevel::INFO, Logger::DbgLevel::TRACE, true, true);

    QString user = qgetenv("USER");
    zInfo(QStringLiteral("started ")+target+" as "+user);

    QCommandLineParser parser;
    QList<QCommandLineOption> options{
      {{"i",QStringLiteral("input")},QStringLiteral("image file name"),"input"},
      {{"u",QStringLiteral("usbpath")},QStringLiteral("path of usb"),"usbpath"},
      {{"p",QStringLiteral("path")},QStringLiteral("working path"),"path"},
      {{"s",QStringLiteral("passwd")},QStringLiteral("secret"),"passwd"},
      {{"f",QStringLiteral("force")},QStringLiteral("no ask")},
      };
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions(options);
    parser.process(app);

    Work1::_params.Parse(&parser);

    CoreAppWorker c(Work1::doWork, &app);//, &parser);
    volatile auto errcode = c.run();

    switch(errcode)
    {
        case Work1::OK: zInfo("ok"); break;
        case Work1::ISEMPTY: zInfo("no block device to write"); break;
        case Work1::NO_INPUTFILE: zInfo("no input file to read"); break;
        case Work1::NO_LASTREC: zInfo("cannot find last record"); break;
        case Work1::NO_UNITS: zInfo("unknown block size"); break;
        case Work1::FILENOTEXIST: zInfo("file not exist"); break;

        case Work1::CANNOT_UNMOUNT: zInfo("cannot unmount device"); break;
        case Work1::NO_CHECK0: zInfo("no checksum0"); break;
        case Work1::NO_CHECK1: zInfo("no checksum1"); break;
        case Work1::CHECKSUM_ERROR: zInfo("checksum error"); break;
        case Work1::NO_PASSWD: zInfo("cannot sudo"); break;

        case Work1::NO_USBDRIVE: zInfo("no usb device to use"); break;
        case Work1::DRIVE_SIZE_ERROR: zInfo("drive size error"); break;

        case Work1::NO_DCFLDD: zInfo("no dcfldd"); break;

        default: zInfo("an error occured: "+QString::number(errcode)); break;
    }

    auto e = QCoreApplication::exec();
    return e;
}
