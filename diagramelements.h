#ifndef DIAGRAMELEMENTS_H
#define DIAGRAMELEMENTS_H

#include <QGraphicsSvgItem>

#include "socinfo.h"

class QGraphicsColorizeEffect;

class SMDPinItem :  public QGraphicsSvgItem {
    Q_OBJECT
public:
    enum Side_t { SideLeft, SideBottom, SideRight, SideTop };

    SMDPinItem(const SoCInfo& sInfo, QScriptValue pInfo, Side_t side, QGraphicsItem *parent);

    const QString currentFunctionLabel() const;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    const Side_t pinSide;
    const SoCInfo& socInfo;
    QScriptValue pinInfo;
    QGraphicsTextItem *functionLabel;
    QGraphicsColorizeEffect *color;

    void refreshFunctionLabel();
    void showPinMenu(const QPointF &pos);
};

class SocBodyItem : public QGraphicsSvgItem {
public:
    SocBodyItem(SoCInfo *inf, const QSizeF& z, QGraphicsItem *parentItem = 0l);
    virtual ~SocBodyItem() { delete info; }

private:
    const SoCInfo *info;
};

#endif // DIAGRAMELEMENTS_H

