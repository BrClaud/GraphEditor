#include "edgeitem.h"

#include "nodeitem.h"
#include "qpen.h"

EdgeItem::EdgeItem(NodeItem *source, NodeItem *dest)
    : source(source), dest(dest) {
    QPen pen(Qt::blue);
    pen.setWidth(2);
    setPen(pen);
    setZValue(0);  // чтобы линии были под узлами
    updatePosition();
    source->addEdge(this);
    dest->addEdge(this);
}

void EdgeItem::updatePosition() {
    qDebug() << "перемещено ребро между узлами id_1 = " << source->getId()
             << " id_2 = " << dest->getId() << "\n";
    QLineF line(source->scenePos(), dest->scenePos());
    setLine(line);
}
