#ifndef ZBARLINK_H
#define ZBARLINK_H

#include <QFile>
#include <QProcess>
#include <QImage>
#include <QTemporaryFile>
#include <QDebug>
#include <QDir>
#include <QTextStream>

class ZBarLink
{
public:
    ZBarLink();
    static QString scanQRCode(const QImage& image);
};

#endif // ZBARLINK_H
