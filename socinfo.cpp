#include "socinfo.h"

#include <QFile>
#include <QScriptEngine>
#include <QScriptValueIterator>

static QString readFile(const QString& path) {
    QFile f(path);
    return f.open(QFile::ReadOnly)? f.readAll() : QString();
}

SoCInfo SoCInfo::fromFile(const QString &path)
{
    QScriptEngine sc;
    QFile f(path);
    if (!f.open(QFile::ReadOnly))
        return SoCInfo();
    QScriptValue v = sc.evaluate(f.readAll());
    if (!v.isObject())
        return SoCInfo();
    QScriptValue functions = v.property("functions");
    if (!functions.isObject())
        return SoCInfo();

    SoCInfo info;
    info.name = v.property("name").toString();
    if (info.name.isEmpty())
        return SoCInfo();
    QScriptValueIterator it(functions);
    while(it.hasNext()) {
        it.next();
        bool ok = false;
        int pinNum = it.name().toInt(&ok);
        if (ok && it.value().isArray()) {
            info.pins.insert(pinNum, PinInfo(it.value().toVariant().toStringList()));
        } else
            return SoCInfo();
    }
    return info;
}
