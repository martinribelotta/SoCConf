#ifndef SOCINFO_H
#define SOCINFO_H

#include <QScriptValue>
#include <QScriptEngine>

class SoCInfo {
    QString m_errorMsg;
    mutable QScriptEngine sc;

public:
    SoCInfo() { }

    QScriptValue obj() const { return sc.globalObject(); }

    QString errorMessage() const { return m_errorMsg; }

    QString name() const { return obj().property("name").toString(); }

    QScriptValue pins() const { return obj().property("pins"); }

    QScriptValue properties() const { return obj().property("properties"); }

    QString callFunction(const QString& name, QScriptValue pin) const {
        QScriptValue pinText = obj().property(name);
        QScriptValueList args;
        args << pin;
        QString res = pinText.call(QScriptValue(), args).toString();
        if (sc.hasUncaughtException())
            return QString("excepto on %1").arg(sc.uncaughtExceptionLineNumber());
        return res;
    }

    bool load(const QString& path);

private:
    bool error(const QString& message);
};


#endif // SOCINFO_H
