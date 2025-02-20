#ifndef NODEITEM_H
#define NODEITEM_H

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QList>

class EdgeItem;

class NodeItem : public QGraphicsEllipseItem {
   public:
    NodeItem(int id, QGraphicsItem *parent = nullptr);
    int getId() const { return id; }
    void addEdge(EdgeItem *edge);
    void removeEdge(EdgeItem *edge);
    const QList<EdgeItem *> &getEdges() const { return edges; }
    int getId() { return id; }
    ~NodeItem();

   protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

   private:
    int id;
    QGraphicsTextItem *label;  // элемент для отображения номера
    QList<EdgeItem *> edges;
};

#endif  // NODEITEM_H
