#include "utils.h"
#include "screencapturewgt.h"

#ifdef Q_OS_WIN
#include <windows.h>
#elif defined(Q_OS_UNIX)
#include <unistd.h>
#endif

Utils::Utils() {}

void Utils::loadStyle(const QString &styleFile)
{
    QFile file(styleFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(file.readAll());
        file.close();
        qApp->setStyleSheet(styleSheet);
        qApp->processEvents();
    }
}

QVariantList Utils::readJson(const QString &path)
{
    QVariantList datas;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        return datas;
    }
    QByteArray data = file.readAll();
    file.close();
    if(data.isEmpty()){
        return QVariantList();
    }

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));
    if(jsonError.error != QJsonParseError::NoError){
        return datas;
    }

    QVariant var = jsonDoc.toVariant();
    switch (var.type()) {
    case QVariant::Map:
    {
        QVariantMap data = var.toMap();
        datas.append(data);
    }
    break;
    case QVariant::List:
    {
        datas = var.toList();
    }
    default:
        break;
    }

    return datas;
}

void Utils::writeJson(const QVariantList &datas, const QString &path)
{
    QJsonDocument jsonDoc = QJsonDocument::fromVariant((QVariant)datas);
    QByteArray byteData = jsonDoc.toJson();
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    file.write(byteData);
    file.close();
}

QString Utils::currentDateTimeString(const QString format)
{
    return QDateTime::currentDateTime().toString(format);
}

void Utils::writeConfig(const QString &cfg, const QString &key, const QVariant &value)
{
    QSettings set(cfg,QSettings::IniFormat);
    set.setValue(key,value);
}

QVariant Utils::readConfig(const QString &cfg, const QString &key)
{
    QSettings set(cfg,QSettings::IniFormat);
    return set.value(key);
}

QSize Utils::primaryScreenSize()
{
    return QGuiApplication::primaryScreen()->geometry().size();
}

QString Utils::getMotherboardSerialNumber()
{
    QString serialNumber = QSysInfo::machineUniqueId();
    return serialNumber;
}

QString Utils::createUuid()
{
    QString uuidString = QUuid::createUuid().toString();
    QRegularExpression regex("[{}\\-]");
    return uuidString.remove(regex);
}

bool Utils::fileExsit(const QString &path)
{
    QFile file(path);
    return file.exists();
}

bool Utils::dirExsit(const QString &path)
{
    QDir dir(path);
    return dir.exists();
}

bool Utils::removeDir(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return true;
    }
    // 删除目录中的所有文件和子目录
    foreach (QString file, dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString filePath = dir.filePath(file);
        if (QFileInfo(filePath).isDir()) {
            removeDir(filePath); // 递归删除子目录
        } else {
            QFile::remove(filePath); // 删除文件
        }
    }
    // 删除空目录
    return dir.rmdir(path);
}

bool Utils::removeFile(const QString &filePath)
{
    if(!fileExsit(filePath)){
        return true;
    }
    QFile file(filePath);
    return file.remove();
}

bool Utils::isRunningAsAdmin()
{
#ifdef Q_OS_WIN
    BOOL isAdmin = FALSE;
    PSID adminGroup;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&NtAuthority, 2,
                                  SECURITY_BUILTIN_DOMAIN_RID,
                                  DOMAIN_ALIAS_RID_ADMINS,
                                  0, 0, 0, 0, 0, 0,
                                  &adminGroup))
    {
        return false;
    }

    if (!CheckTokenMembership(NULL, adminGroup, &isAdmin))
    {
        isAdmin = FALSE;
    }

    FreeSid(adminGroup);
    return isAdmin == TRUE;
#elif defined(Q_OS_UNIX)
    return geteuid() == 0;
#else
    return false; // 未知平台默认返回false
#endif
}

QPixmap Utils::screenCapture()
{
    QPixmap pix;
    ScreenCaptureWgt w;
    if(w.exec() == QDialog::Accepted)
    {
        pix = w.pixmap();
    }
    return pix;
}

