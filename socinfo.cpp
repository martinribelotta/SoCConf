#include "socinfo.h"

#include <QFile>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QObject>

#define _(s) QObject::tr(s)

SoCInfo SoCInfo::fromFile(const QString &path)
{
    QScriptEngine sc;
    QFile f(path);
    if (!f.open(QFile::ReadOnly))
        return SoCInfo::error(_("File error: %1").arg(f.errorString()));
    QScriptValue v = sc.evaluate(f.readAll());
    if (!v.isObject())
        return SoCInfo::error(_("Format error: Base value is not an object."));
    QScriptValue functions = v.property("functions");
    if (!functions.isObject())
        return SoCInfo::error(_("Format error: File not contain function list or is not an object."));

    SoCInfo info;
    info.name = v.property("name").toString();
    if (info.name.isEmpty())
        return SoCInfo::error(_("Format error: Object name missing."));
    info.generator = v.property("generator");
    if (!info.generator.isFunction())
        return SoCInfo::error(_("Format error: File not contain generator or generator is not a function."));
    QScriptValueIterator it(functions);
    int entry = 0;
    while(it.hasNext()) {
        it.next();
        bool ok = false;
        int pinNum = it.name().toInt(&ok);
        if (!ok)
            return SoCInfo::error(_("Format error: Entry %1 malformed, pin is not a number.").arg(entry));
        if (!it.value().isArray())
            return SoCInfo::error(_("Format error: Entry %1 malformed, missing pin array.").arg(entry));
        info.pins.insert(pinNum, PinInfo(it.value().toVariant().toStringList()));
        entry++;
    }
    return info;
}

SoCInfo SoCInfo::error(const QString &message)
{
    SoCInfo e;
    e.name = message;
    return e;
}
