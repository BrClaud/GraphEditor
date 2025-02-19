#include "nodeitem.h"
#include "edgeitem.h"

NodeItem::NodeItem(int id, QGraphicsItem *parent) : QGraphicsEllipseItem(-20, -20, 40, 40, parent), id(id) {
    setBrush(QColorConstants::Svg::aqua);
    setFlag(QGraphicsItem::ItemIsMovable); // узел двигается
    setFlag(QGraphicsItem::ItemIsSelectable); // узел можно выбрать
    setFlag(QGraphicsItem::ItemSendsGeometryChanges); // узел отправляет сообщение об изменении позиции
    setZValue(1); // чтобы линии были под узлами

    // добавляем номер
    label = new QGraphicsTextItem(this);
    label->setHtml(QString("<center><span style='color: black;'>%1</span></center>").arg(id));
    QRectF textRect = label->boundingRect();
    qreal textWidth = textRect.width();
    qreal textHeight = textRect.height();
    label->setPos(-textWidth / 2, - textHeight / 2); // отображение по центру

}

void NodeItem::addEdge(EdgeItem *edge){
    edges.append(edge);
}

void NodeItem::removeEdge(EdgeItem *edge){
    edges.removeOne(edge);
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value){
    // qDebug()<<"изменен узел "<<id<<"\n";
    if(change == QGraphicsItem::ItemPositionHasChanged){
        qDebug()<<"перемещен узел "<<id<<"\n";
        for(EdgeItem *edge : edges){
            edge->updatePosition();
        }
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}
