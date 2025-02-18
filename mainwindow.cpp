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
        qDebug() << "создается связь между нодами\n";
        auto selected = scene->selectedItems();
        qDebug()<<"получили выбранную ноду\n";
        if(!selected.isEmpty() && selected.first()->type() == NodeItem::Type){
            qDebug()<<"зашли в первый if\n";
            NodeItem *node = static_cast<NodeItem*>(selected.first());
            // qDebug()<<"выбранная нода id = " <<selectedNode->getId()<<" новая нода id = "<<node->getId()<<"\n";
            if(selectedNode){
                qDebug()<<"создаем ребро между нодами id_1 = " << selectedNode->getId() << " id_2 = "<<node->getId()<<"\n";
                createEdge(selectedNode, node);
                selectedNode = nullptr;
            }else{
                qDebug()<< "записали ноду в селектед\n";
                selectedNode = node;
            }
        }else{
            qDebug()<<"вышли из обработчика связей\n";
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

void MainWindow::createEdge(NodeItem* source, NodeItem* dest){
    qDebug()<<"создано ребро\n";
    auto selectedItems = scene->selectedItems();
    if(!selectedItems.isEmpty() && selectedItems.first()->type() == NodeItem::Type){

        EdgeItem *edge = new EdgeItem(source, dest);
        scene->addItem(edge);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
