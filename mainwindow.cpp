#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->viewport()->installEventFilter(this);

    // обработчик создания связи между кружками
    connect(scene, &QGraphicsScene::selectionChanged,this, [this](){
        auto selected = scene->selectedItems();
        if(!selected.isEmpty() && selected.first()->type() == NodeItem::Type){
            NodeItem *node = static_cast<NodeItem*>(selected.first());
            if(selectedNode){
                createEdge(selectedNode);
                selectedNode = nullptr;
            }else{
                selectedNode = node;
            }
        }
    });
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if(event->type() == QEvent::MouseButtonDblClick){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPointF pos = ui->graphicsView->mapToScene(mouseEvent->pos());

        // создание узла
        NodeItem *node = new NodeItem(++nodeCounter);
        node->setPos(pos);
        scene->addItem(node);
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete){
        for(auto item : scene->selectedItems()){
            if(item->type() == NodeItem::Type){
                NodeItem *node = static_cast<NodeItem*>(item);
                for(auto edge: node->getEdges()){
                    scene->removeItem(edge);
                    delete edge;
                }
                scene->removeItem(node);
                delete node;
            }else
            {
                if(item->type() == EdgeItem::Type){
                    scene->removeItem(item);
                    delete item;
                }
            }
        }
    }
}

void MainWindow::createEdge(NodeItem* source){
    auto selectedItems = scene->selectedItems();
    if(!selectedItems.isEmpty() && selectedItems.first()->type() == NodeItem::Type){
        NodeItem *dest = static_cast<NodeItem*>(selectedItems.first());

        EdgeItem *edge = new EdgeItem(source, dest);
        scene->addItem(edge);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
