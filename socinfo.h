#ifndef SOCINFO_H
#define SOCINFO_H

#include <QScriptValue>
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
    QScriptValue generator;

    SoCInfo() { }

    bool isError() const { return !name.isEmpty() && pins.isEmpty(); }
    QString errorMessage() const { return name; }

    static SoCInfo fromFile(const QString& path);
    static SoCInfo error(const QString& message);
};


#endif // SOCINFO_H
