#include "edgeitem.h"

#include "nodeitem.h"
#include "qpen.h"

EdgeItem::EdgeItem(NodeItem *source, NodeItem *dest)
    : source(source), dest(dest) {
    QPen pen(Qt::blue);
    pen.setWidth(2);
    setPen(pen);
    setZValue(0);  // чтобы линии были под узлами
    setFlag(QGraphicsItem::ItemIsSelectable);
    updatePosition();
    source->addEdge(this);
    dest->addEdge(this);
}

void EdgeItem::updatePosition() {
    QLineF line(source->scenePos(), dest->scenePos());
    setLine(line);
}

EdgeItem::~EdgeItem() {
    source->removeEdge(this);
    dest->removeEdge(this);
    source = nullptr;
    dest = nullptr;
}
