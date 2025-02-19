#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QGraphicsLineItem>

class NodeItem;

class EdgeItem : public QGraphicsLineItem {
   public:
    EdgeItem(NodeItem *source, NodeItem *dest);
    void updatePosition();

    NodeItem *getSource() const { return source; }
    NodeItem *getDest() const { return dest; }

   private:
    NodeItem *source, *dest;
};

#endif  // EDGEITEM_H
