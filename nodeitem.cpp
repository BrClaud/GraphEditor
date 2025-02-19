#include "nodeitem.h"
#include "edgeitem.h"

NodeItem::NodeItem(int id, QGraphicsItem *parent) : QGraphicsEllipseItem(-20, -20, 40, 40, parent), id(id) {
    setBrush(Qt::lightGray);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    // добавляем номер
    label = new QGraphicsTextItem(QString::number(id),this);
    label->setPos(-5, -10); // отображение по центру

}

void NodeItem::addEdge(EdgeItem *edge){
    edges.append(edge);
}

void NodeItem::removeEdge(EdgeItem *edge){
    edges.removeOne(edge);
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value){
    qDebug()<<"изменился узел "<<id<<"\n";
    if(change == QGraphicsItem::ItemPositionHasChanged){
        for(EdgeItem *edge : edges){
            edge->updatePosition();
        }
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}
