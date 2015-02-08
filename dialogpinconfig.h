#ifndef DIALOGPINCONFIG_H
#define DIALOGPINCONFIG_H

#include <QDialog>

namespace Ui {
class DialogPinConfig;
}

class PinInfo;

class DialogPinConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPinConfig(int n, const PinInfo &info, QWidget *parent = 0);
    ~DialogPinConfig();

    int selectedElement() const;
private:
    Ui::DialogPinConfig *ui;
};

#endif // DIALOGPINCONFIG_H
