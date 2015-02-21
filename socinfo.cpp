#include "socinfo.h"

#include <QFile>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QObject>

#include <QtDebug>

#define _(s) QObject::tr(s)

bool SoCInfo::load(const QString &path)
{
    QFile f(path);
    m_errorMsg = QString();

    if (!f.open(QFile::ReadOnly))
        return SoCInfo::error(_("File error: %1").arg(f.errorString()));

    sc.evaluate(f.readAll());

    if (sc.hasUncaughtException())
        return SoCInfo::error(QString("Exception at %2: %1")
                              .arg(sc.uncaughtException().toString())
                              .arg(sc.uncaughtExceptionLineNumber()));

    QScriptValueIterator it(pins());
    int max = 0;
    while(it.hasNext()) {
        it.next();
        bool ok = false;
        int v = it.name().toInt(&ok);
        if (ok && (max < v))
            max = v;
    }
    if (max == 0)
        return SoCInfo::error(_("No pins"));
    pins().setProperty("length", max);

    return true;
}

bool SoCInfo::error(const QString &message)
{
    m_errorMsg = message;
    return false;
}
