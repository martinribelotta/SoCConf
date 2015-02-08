#ifndef SOCINFO_H
#define SOCINFO_H

#include <QStringList>
#include <QHash>
#include <QList>
#include <QSet>

class PinInfo {
public:
    QStringList functions;

    PinInfo() { }
    PinInfo(const QStringList& func) : functions(func) {}

    bool isValid() const { return !functions.isEmpty(); }
};


class SoCInfo {
public:
    QString name;
    QHash<int, PinInfo> pins;

    SoCInfo() { }

    static SoCInfo fromFile(const QString& path);
};


#endif // SOCINFO_H
