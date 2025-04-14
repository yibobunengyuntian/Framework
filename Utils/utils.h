#ifndef UTILS_H
#define UTILS_H

#include <QVariant>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSettings>
#include <QApplication>
#include <QScreen>
#include <QDir>
#include <QFileInfo>

class Utils
{
public:
    Utils();

    //读取json
    static QVariantList readJson(const QString &path);
    //写入json
    static void writeJson(const QVariantList &datas, const QString &path);

    //获取当前时间
    static QString currentDateTimeString(const QString format = "yyyy-MM-dd hh:mm:ss");

    //写入配置文件
    static void writeConfig(const QString &cfg, const QString &key, const QVariant &value);
    //读取配置文件
    static QVariant readConfig(const QString &cfg, const QString &key);

    //获取当前主屏幕大小
    static QSize primaryScreenSize();
    //获取主板唯一码
    static QString getMotherboardSerialNumber();
    //创建一个唯一码
    static QString createUuid();

    //文件是否存在
    static bool fileExsit(const QString &path);
    //文件夹是否存在
    static bool dirExsit(const QString &path);
    //删除文件夹及其中内容
    static bool removeDir(const QString &path);
    //删除文件
    static bool removeFile(const QString &filePath);

    //程序是否以管理员权限运行
    static bool isRunningAsAdmin();

    //截屏
    static QPixmap screenCapture();
};

#endif // UTILS_H
