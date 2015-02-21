#include "diagramelements.h"

#include "dialogpinconfig.h"

#include <QtDebug>

#include <QGraphicsProxyWidget>
#include <QSvgRenderer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsSceneEvent>
#include <QRegion>
#include <QPen>

SocBodyItem::SocBodyItem(SoCInfo *inf, const QSizeF &z, QGraphicsItem *parentItem) :
    QGraphicsSvgItem(":/images/qfp-body.svg", parentItem), info(inf)
{
    setTransform(QTransform().scale(z.width(), z.height()), false);

    QGraphicsRectItem *border = new QGraphicsRectItem(QRectF(QPointF(), QSizeF(1.5, 1.5)), this);
    border->setPen(Qt::NoPen);
    QRectF br = border->boundingRect();
    QRectF my = boundingRect();
    br.moveCenter(my.center());
    border->setPos(br.topLeft());

    QGraphicsTextItem *label = new QGraphicsTextItem(info->name(), this);
    label->setDefaultTextColor(Qt::lightGray);
    QRectF rl = label->boundingRegion(label->transform()).boundingRect();
    float unary_x = my.width()/rl.width();
    float unary_y = my.height()/rl.height();
    float unary = std::min(unary_x, unary_y) * 0.7;
    label->setScale(unary);
    label->setPos(my.width()/2 - rl.width()/2*unary, my.height()/2 - rl.height()/2*unary);

    QScriptValue pins = info->pins();
    const int pincount = pins.property("length").toInteger();
    const int sidecount = pincount/4;
    float scale = boundingRect().width()/(sidecount+1.8)/1.5;

    int pinNum = 1;

    static const int rotations[] = { 0, -90, -180, -270 };
    static const QPointF translations[] = {
        QPointF(0, 0),
        QPointF(0, boundingRect().height()),
        QPointF(boundingRect().width(), boundingRect().height()),
        QPointF(boundingRect().width(), 0)
    };
    for(int sideNum = 0; sideNum<4; sideNum++) {
        QGraphicsItemGroup *group = new QGraphicsItemGroup(this);
        group->setHandlesChildEvents(false);
        for(int n=1; n<=sidecount; n++) {
            //Pininfo->pinInfo(info->pins.value(pinNum));
            QScriptValue pinInfo = info->pins().property(QString("%1").arg(pinNum));
            pinInfo.setProperty("number", QScriptValue(pinNum));
            SMDPinItem *pin = new SMDPinItem(*info, pinInfo, SMDPinItem::Side_t(sideNum), this);
            pin->setAcceptedMouseButtons(Qt::LeftButton);
            pin->setScale(scale);
            float w = pin->boundingRect().width()*scale;
            float h = pin->boundingRect().height()*scale;
            pin->setPos(QPointF(-w, n * h * 1.5));
            group->addToGroup(pin);
            pinNum++;
        }
        group->setTransform(QTransform()
                            .translate(translations[sideNum].x(), translations[sideNum].y())
                            .rotate(rotations[sideNum]));
    }
}

SMDPinItem::SMDPinItem(const SoCInfo& sInfo, QScriptValue pInfo, Side_t side, QGraphicsItem *parent) :
    QGraphicsSvgItem(":/images/qfp-pin.svg", parent), pinSide(side), socInfo(sInfo), pinInfo(pInfo)
{
    setCursor(QCursor(Qt::ArrowCursor));
    setAcceptedMouseButtons(Qt::LeftButton);
    setGraphicsEffect(color = new QGraphicsColorizeEffect(this));
    color->setColor(QColor());

    int pinNum = pinInfo.property("number").toInteger();
    QGraphicsTextItem *label = new QGraphicsTextItem(QString("%1").arg(pinNum), this);
    QRectF rl = label->boundingRegion(label->transform()).boundingRect();
    QRectF my = boundingRect();
    float unary_x = my.width()/rl.width();
    float unary_y = my.height()/rl.height();
    float unary = std::min(unary_x, unary_y);
    QTransform t;
    t.scale(unary, unary);
    switch(side) {
    case SideLeft:
        break;
    case SideBottom:
        t.translate(rl.width()/2, rl.height()/2).rotate(90).translate(-rl.width()/2, -rl.height()/2);
        break;
    case SideRight:
        t.translate(rl.width()/2, rl.height()/2).rotate(180).translate(-rl.width()/2, -rl.height()/2);
        break;
    case SideTop:
        t.translate(rl.width()/2, rl.height()/2).rotate(270).translate(-rl.width()/2, -rl.height()/2);
        break;
    }
    label->setTransform(t);
    rl = label->boundingRegion(label->transform()).boundingRect();
    rl.moveCenter(my.center());
    label->setPos(rl.topLeft());

    functionLabel = 0l;

    refreshFunctionLabel();
}

const QString SMDPinItem::currentFunctionLabel() const
{
    return socInfo.callFunction("pinText", pinInfo);
}

void SMDPinItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    showPinMenu(event->scenePos());
}

void SMDPinItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    showPinMenu(event->scenePos());
}

void SMDPinItem::refreshFunctionLabel()
{
    if (functionLabel)
        delete functionLabel;
    QGraphicsTextItem *label = new QGraphicsTextItem(this);
    label->setPlainText(currentFunctionLabel());
    QRectF rl = label->boundingRegion(label->transform()).boundingRect();
    QRectF my = boundingRect();
    float unary_y = my.height()/rl.height();
    QTransform t;
    t.scale(unary_y, unary_y).translate(-rl.width(), 0);
    if (pinSide == SideRight)
        t.translate(rl.width()/2, rl.height()/2).rotate(180).translate(-rl.width()/2, -rl.height()/2);

    label->setTransform(t);
    functionLabel = label;

    color->setColor(QColor(socInfo.callFunction("pinColor", pinInfo)));
    setToolTip(socInfo.callFunction("pinTooltip", pinInfo));
}

void SMDPinItem::showPinMenu(const QPointF &pos)
{
    QGraphicsItem *body = parentItem()->parentItem();
    if (body->graphicsEffect())
        return;

    QGraphicsColorizeEffect *shadow = new QGraphicsColorizeEffect(this);
    shadow->setColor(Qt::darkGreen);
    scene()->setBackgroundBrush(QColor(Qt::darkGreen).darker());
    body->setGraphicsEffect(shadow);
    DialogPinConfig d(pinInfo, socInfo);
    if (d.isConfigurable()) {
        QGraphicsProxyWidget *w = scene()->addWidget(&d);
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(w);
        shadow->setBlurRadius(50);
        shadow->setOffset(QPointF(0, 0));
        shadow->setColor(Qt::black);
        w->setGraphicsEffect(shadow);
        w->setPos(pos);
        if (scene()->views().count())
            w->setTransform(scene()->views().at(0)->transform().inverted());
        if (d.exec() == QDialog::Accepted) {
            pinInfo.setProperty("idx", d.idx());
            refreshFunctionLabel();
        }
    }
    scene()->setBackgroundBrush(Qt::white);
    body->setGraphicsEffect(0l);
}
