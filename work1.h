#ifndef WORK1_H
#define WORK1_H

#include <QCommandLineParser>
#include <QFileInfo>
#include <QStringList>

struct UsbDriveModel{
    QString devicePath;
    QString usbPath;
    QStringList partLabels;
    quint64 size;

    QString toString() const;
    bool isValid();
    QString GetLastUsbTag() const;
};

struct RecModel{
    int units;
    int lastrec;
};

class Work1
{
public:
    enum Result : int{
        OK=0,
        ISEMPTY,
        NO_LASTREC,
        CANNOT_UNMOUNT,
        NO_UNITS,
        NO_INPUTFILE,
        FILENOTEXIST,
        NOT_CONFIRMED,
        COPY_ERROR,
        NO_CHECK0,
        NO_CHECK1,
        CHECKSUM_ERROR,
        NO_PASSWD,
        NO_USBDRIVE,
        DRIVE_SIZE_ERROR
    };
public:
    struct Params{
    public:
        QString ifile;
        QString usbPath;
        bool force;
        QString path;
        QString passwd;

        void Parse(QCommandLineParser *p)
        {
            ifile = p->value("i");
            path = p->value("p");
            usbPath = p->value("u");
            passwd = p->value("s");
            force = p->isSet("f");
        }
    };

    struct MultiSha{
        QString command;
        QString dev;
        QString shaFileName;
        QString sha;
    // private:
    //     QString _sha;
    // public:
    //     void setSha(const QString& v) {_sha = v;}
    //     QString sha(){return _sha;}
    };

    Work1();
    static int doWork();
    static Params _params;

    static QList<UsbDriveModel> GetUsbDrives();
    //static UsbDriveModel SelectUsbDrive(const QList<UsbDriveModel>& usbdrives);
    static QString GetUsbPath(const QString& dev);
    static QStringList GetPartLabels(const QString& dev);

    static int GetLastRecord(const QString &drive, int* units);    
    static QList<RecModel> GetLastRecords(const QList<UsbDriveModel> &usbdrives);
    static bool CheckRecords_Units(const QList<RecModel>& records);

    static QList<UsbDriveModel> GetSmallUsbDrive(const QList<UsbDriveModel>& usbdrives, quint64 size);
    static QStringList GetDevPaths(const QList<UsbDriveModel>& usbdrives);

    static int dd(const QString& src, const QString& dst, int bs, QString *mnt);
    static int dd2(const QString& src, const QStringList& dst, int bs, QString *mnt);
    static bool ConfirmYes();
    static QString GetFileName(const QString& msg);
    static QStringList MountedParts(const QString &src);
    static bool UmountParts(const QStringList &src);    
    static QFileInfo MostRecent(const QString&);
    static QString BytesToString(double b);
    static QList<MultiSha> sha256sumDevices(const QStringList &fns, int r, qint64 b, const QString& sha_fn);
    static QString LoadSha(const QString &fn);

    //static ProcessHelper::Output Execute2(const QString& cmd);
    //static com::helper::ProcessHelper::Output Execute2Pipe(const QString &cmd1, const QString &cmd2);
private:
    static QList<UsbDriveModel> SelectUsbDrives(const QList<UsbDriveModel>& drives, const QString& usbPath);
};

#endif // WORK1_H
