#include "diagramelements.h"

#include "dialogpinconfig.h"

#include <QtDebug>

#include <QSvgRenderer>
#include <QGraphicsEffect>
#include <QGraphicsSceneEvent>
#include <QRegion>
#include <QPen>

static const QString createTooltop(int pin, const QStringList& funcList) {
    QString s;
    s = QString("Pin %1\n").arg(pin);
    if (funcList.count() == 1)
        return QString("Pin %1: %2").arg(pin).arg(funcList.front());
    int i = 0;
    foreach(QString f, funcList) {
        if (f != "None")
            s+= QString("     %2: %1\n").arg(f).arg(i);
        i++;
    }
    return s;
}

SocBodyItem::SocBodyItem(const SoCInfo &info, const QSizeF &z, QGraphicsItem *parentItem) :
    QGraphicsSvgItem(":/images/qfp-body.svg", parentItem)
{
    setTransform(QTransform().scale(z.width(), z.height()), false);

    QGraphicsRectItem *border = new QGraphicsRectItem(QRectF(QPointF(), QSizeF(1.2, 1.2)), this);
    border->setPen(Qt::NoPen);
    QRectF br = border->boundingRect();
    QRectF my = boundingRect();
    br.moveCenter(my.center());
    border->setPos(br.topLeft());

    QGraphicsTextItem *label = new QGraphicsTextItem(info.name, this);
    label->setDefaultTextColor(Qt::lightGray);
    QRectF rl = label->boundingRegion(label->transform()).boundingRect();
    float unary_x = my.width()/rl.width();
    float unary_y = my.height()/rl.height();
    float unary = std::min(unary_x, unary_y) * 0.7;
    label->setScale(unary);
    label->setPos(my.width()/2 - rl.width()/2*unary, my.height()/2 - rl.height()/2*unary);

    const int pincount = info.pins.count();
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
            PinInfo pinInfo(info.pins.value(pinNum));
            SMDPinItem *pin = new SMDPinItem(pinInfo, pinNum, SMDPinItem::Side_t(sideNum), this);
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

SMDPinItem::SMDPinItem(const PinInfo& info, int n, Side_t side, QGraphicsItem *parent) :
    QGraphicsSvgItem(":/images/qfp-pin.svg", parent), pinSide(side), pinInfo(info), currentFunction(0)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setGraphicsEffect(color = new QGraphicsColorizeEffect(this));
    color->setColor(QColor());

    setToolTip(createTooltop(n, info.functions));
    QGraphicsTextItem *label = new QGraphicsTextItem(QString("%1").arg(n), this);
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
    if (pinInfo.functions.count() > 1)
        color->setEnabled(false);
}

const QString SMDPinItem::currentFunctionLabel() const
{
    return pinInfo.functions.at(currentFunction);
}

void SMDPinItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    showPinMenu(event->buttonDownScreenPos(Qt::LeftButton));
}

void SMDPinItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    showPinMenu(event->screenPos());
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
    switch(pinSide) {
    case SideRight:
        t.translate(rl.width()/2, rl.height()/2).rotate(180).translate(-rl.width()/2, -rl.height()/2);
        break;
    default:
        break;
    }

    label->setTransform(t);
    functionLabel = label;

    if (pinInfo.functions.count() == 1)
        color->setColor(Qt::darkYellow);
    else
        color->setColor(Qt::darkGreen);
    color->setEnabled(true);
}

void SMDPinItem::showPinMenu(const QPoint &screenPos)
{
    if (pinInfo.functions.count() > 1) {
        DialogPinConfig d(currentFunction, pinInfo);
        d.move(screenPos);
        if (d.exec() == QDialog::Accepted) {
            currentFunction = d.selectedElement();
            refreshFunctionLabel();
        }
    }
}
