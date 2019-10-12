#pragma once

#include <QList>
#include <QDebug>
#include <QLabel>
#include <QString>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QSpacerItem>
#include <QMainWindow>
#include <QActionGroup>
#include <QTableWidgetItem>

#include "ProjectReader.h"
#include "ProjectWriter.h"
#include "ProjectSymbol.h"
#include "ProjectStructure.h"

namespace Ui
{
    class MainWindow;
}

const int NO_SELECT = -1;
const int CELL_SIZE = 23;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

    int currentSelectedId;
    ProjectStructure project;

    bool isSelected[IMAGE_HEIGHT][IMAGE_WIDTH];

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void setupToolbar();
    void setupCombobox();
    void setupStatusBar();
    void setupInfoBlock();
    void setupElementsList();

    void setupTable();
    void paintTable();
    void paintCell( int x, int y );

    void clearWorkArea();

private slots:

    void newRecognition();
    void openRecognition();
    void saveRecognition();
    void startRecognition();

    void cleanPainter();

    void cellSelected( int x, int y );

    void addItem();
    void deleteItem();
    void selectItem( const QModelIndex &index );

private:

    QLabel *labelStatus;
    QWidget *mainToolbarSpacer;
    QActionGroup *mainToolbarStateGroup;

    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionRecognition;
    QAction *actionLearning;
    QAction *actionAdd;
    QAction *actionCleanPainter;
    QAction *actionStateLearn;
    QAction *actionStateRecog;

    QTableWidgetItem* tableItems[IMAGE_HEIGHT][IMAGE_WIDTH];

    Ui::MainWindow *ui;
};
