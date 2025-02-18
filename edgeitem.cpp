#include "edgeitem.h"
#include <nodeitem.h>

EdgeItem::EdgeItem(NodeItem *source, NodeItem *dest) : source(source), dest(dest) {
    updatePosition();
    source->addEdge(this);
    dest->addEdge(this);
}

void EdgeItem::updatePosition(){
    QLineF line(source->scenePos(), dest->scenePos());
    setLine(line);
}
