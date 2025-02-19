#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "edgeitem.h"
#include "nodeitem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QGraphicsScene *scene;
    int nodeCounter = 0; // счетчик узлов
    NodeItem *selectedNode = nullptr;
    Ui::MainWindow *ui;

private slots:
    void createEdge(NodeItem *source, NodeItem* dest);
    void saveGraph();
    void loadGraph();
};
#endif // MAINWINDOW_H
