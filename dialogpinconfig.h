#ifndef DIALOGPINCONFIG_H
#define DIALOGPINCONFIG_H

#include <QDialog>
#include <QScriptValue>
#include <QVariant>

class SoCInfo;
class EntryView;

class DialogPinConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPinConfig(QScriptValue pinInfo, const SoCInfo &socInfo, QWidget *parent = 0);
    ~DialogPinConfig();

    bool isConfigurable() const { return property("configurable").toBool(); }

    QScriptValue idx() const;

private:
    QScriptValue pIdx;
    QList<EntryView*> views;
};

#endif // DIALOGPINCONFIG_H
