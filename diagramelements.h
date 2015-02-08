#ifndef DIAGRAMELEMENTS_H
#define DIAGRAMELEMENTS_H

#include <QGraphicsSvgItem>

#include "socinfo.h"

class SMDPinItem :  public QGraphicsSvgItem {
    Q_OBJECT
public:
    enum Side_t { SideLeft, SideBottom, SideRight, SideTop };

    SMDPinItem(const PinInfo &info, int n, Side_t side, QGraphicsItem *parent);

    const QString currentFunctionLabel() const;
protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    const Side_t pinSide;
    const PinInfo pinInfo;
    int currentFunction;
    QGraphicsTextItem *functionLabel;

    void refreshFunctionLabel();
};

class SocBodyItem : public QGraphicsSvgItem {
public:
    SocBodyItem(const SoCInfo& info, const QSizeF& z, QGraphicsItem *parentItem = 0l);
};

#endif // DIAGRAMELEMENTS_H

