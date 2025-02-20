#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->viewport()->installEventFilter(this);

    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveGraph);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadGraph);
    connect(ui->actionexit, &QAction::triggered, this, &QMainWindow::close);

    // обработчик создания связи между кружками
    connect(scene, &QGraphicsScene::selectionChanged, this, [this]() {
        qDebug() << "создается связь между нодами\n";
        auto selected = scene->selectedItems();
        if (selected.size() == 1) {
            if (!selected.isEmpty() &&
                selected.first()->type() == NodeItem::Type) {
                NodeItem *node = static_cast<NodeItem *>(selected.first());
                qDebug() << "текущая выбранная нода id = " << node->getId();
                if (selectedNode) {
                    qDebug() << "selectedNode уже выбран id = "
                             << selectedNode->getId() << "\n";

                    bool checkEdge =
                        false;  // существует ли уже ребро между этими узлами
                    for (EdgeItem *edg : selectedNode->getEdges()) {
                        if (edg->getDest() && edg->getSource()) {
                            if ((edg->getDest()->getId() == node->getId() &&
                                 edg->getSource()->getId() ==
                                     selectedNode->getId()) ||
                                (edg->getSource()->getId() == node->getId() &&
                                 edg->getDest()->getId() ==
                                     selectedNode->getId())) {
                                qDebug() << "ребро между нодами id_1 = "
                                         << selectedNode->getId()
                                         << " id_2 = " << node->getId()
                                         << " уже существует\n";
                                checkEdge = true;
                                break;
                            }
                        }
                    }
                    if (selectedNode->getId() == node->getId()) {
                        qDebug() << "создание петли не предумотрено\n";
                        checkEdge = true;
                    }

                    if (!checkEdge) {
                        qDebug() << "создаем ребро между нодами id_1 = "
                                 << selectedNode->getId()
                                 << " id_2 = " << node->getId() << "\n";
                        createEdge(selectedNode, node);
                    }

                    selectedNode = nullptr;
                    // scene->clearSelection();
                    scene->clearFocus();
                } else {
                    qDebug() << "записали ноду id = " << node->getId()
                             << " в селектед\n";
                    selectedNode = node;
                }
            } else {
                qDebug() << "вышли из обработчика связей\n";
                selectedNode = nullptr;
            }
        } else {
            if (selected.size() > 1)
                QMessageBox::information(
                    this, "количество элементов",
                    "колличество элементов для выбора привышено или выбранны "
                    "некорректные элементы");
            selectedNode = nullptr;
            scene->clearSelection();
        }
    });
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
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
    if (event->key() == Qt::Key_Delete) {
        if (scene->selectedItems().size() == 1) {
            for (auto item : scene->selectedItems()) {
                if (item->type() == NodeItem::Type) {
                    NodeItem *node = static_cast<NodeItem *>(item);
                    qDebug() << "удалем узел c id " << node->getId() << "\n";
                    for (auto edge : node->getEdges()) {
                        // scene.
                        scene->removeItem(edge);
                        delete edge;
                    }
                    scene->removeItem(node);
                    delete node;
                } else {
                    if (item->type() == EdgeItem::Type) {
                        EdgeItem *edg = static_cast<EdgeItem *>(item);
                        qDebug()
                            << "удалем ребро между нодами id_1 = "
                            << edg->getDest()->getId()
                            << " id_2 = " << edg->getSource()->getId() << "\n";
                        scene->removeItem(item);
                        delete item;
                    }
                }
            }
            qDebug() << "очистили селектед нод\n";
            selectedNode = nullptr;
        } else {
            if (scene->selectedItems().size() > 1)
                QMessageBox::information(
                    this, "количество элементов",
                    "колличество элементов для выбора привышено или выбранны "
                    "некорректные элементы");
            selectedNode = nullptr;
            scene->clearSelection();
        }
    }
}

void MainWindow::createEdge(NodeItem *source, NodeItem *dest) {
    auto selectedItems = scene->selectedItems();
    if (!selectedItems.isEmpty() &&
        selectedItems.first()->type() == NodeItem::Type) {
        EdgeItem *edge = new EdgeItem(source, dest);
        scene->addItem(edge);
    }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::saveGraph() {
    // выбираем имя файла в диалоговом окне
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Graph"), "",
                                                    tr("JSON Files (*.json)"));
    if (fileName.isEmpty()) return;

    // открываем файл для записи
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Could not open file for writing"));
        return;
    }

    // Создаем JSON-объект для хранения данных графа
    QJsonObject graphObject;

    // сохраняем узлы
    QJsonArray nodesArray;
    for (auto item : ui->graphicsView->scene()->items()) {
        if (item->type() == NodeItem::Type) {
            NodeItem *node = static_cast<NodeItem *>(item);
            QJsonObject nodeObject;
            nodeObject["id"] = node->getId();
            nodeObject["x"] = node->pos().x();
            nodeObject["y"] = node->pos().y();
            nodesArray.append(nodeObject);
        }
    }
    graphObject["nodes"] = nodesArray;

    // сохраняем связи
    QJsonArray edgesArray;
    for (auto item : ui->graphicsView->scene()->items()) {
        if (item->type() == EdgeItem::Type) {
            EdgeItem *edge = static_cast<EdgeItem *>(item);
            QJsonObject edgeObject;
            edgeObject["source"] = edge->getSource()->getId();
            edgeObject["destination"] = edge->getDest()->getId();
            edgesArray.append(edgeObject);
        }
    }
    graphObject["edges"] = edgesArray;

    QJsonDocument doc(graphObject);
    file.write(doc.toJson());
    file.close();
}

void MainWindow::loadGraph() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Graph"), "",
                                                    tr("JSON Files (*.json)"));
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Could not open file for reading."));
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid JSON file"));
        return;
    }

    QJsonObject graphObject = doc.object();

    ui->graphicsView->scene()->clear();

    QMap<int, NodeItem *> nodeMap;
    QJsonArray nodesArray = graphObject["nodes"].toArray();
    for (const QJsonValue &nodeValue : nodesArray) {
        QJsonObject nodesObject = nodeValue.toObject();
        int id = nodesObject["id"].toInt();
        qreal x = nodesObject["x"].toDouble();
        qreal y = nodesObject["y"].toDouble();

        NodeItem *node = new NodeItem(id);
        node->setPos(x, y);
        ui->graphicsView->scene()->addItem(node);
        nodeMap[id] = node;
    }

    QJsonArray edgesArray = graphObject["edges"].toArray();
    for (const QJsonValue &edgeValue : edgesArray) {
        QJsonObject edgeObject = edgeValue.toObject();
        int sourceId = edgeObject["source"].toInt();
        int destId = edgeObject["destination"].toInt();

        if (nodeMap.contains(sourceId) && nodeMap.contains(destId)) {
            EdgeItem *edge = new EdgeItem(nodeMap[sourceId], nodeMap[destId]);
            ui->graphicsView->scene()->addItem(edge);
        }
    }
}
