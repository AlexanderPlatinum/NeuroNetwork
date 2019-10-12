#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setupTable();
    this->setupToolbar();
    this->setupCombobox();
    this->setupInfoBlock();
    this->setupStatusBar();
    this->setupElementsList();

    this->setWindowTitle( "Neuro Network: Распознавание букв" );
    this->setWindowIcon( QIcon( ":/images/word.png" ) );

    this->currentSelectedId = NO_SELECT;

    network = new Network(IMAGE_WIDTH * IMAGE_HEIGHT);
}

MainWindow::~MainWindow()
{  
    delete network;

    delete labelStatus;
    delete mainToolbarSpacer;
    delete mainToolbarStateGroup;

    delete actionNew;
    delete actionOpen;
    delete actionSave;
    delete actionRecognition;
    delete actionLearning;
    delete actionAdd;
    delete actionCleanPainter;
    delete actionStateLearn;
    delete actionStateRecog;

    for ( int row = 0; row < ui->tableWidget->rowCount(); row++ )
    {
        for ( int collumn = 0; collumn < ui->tableWidget->columnCount(); collumn++ )
        {
            delete tableItems[row][collumn];
        }
    }

    delete ui;
}

void MainWindow::setupToolbar()
{
    actionNew = new QAction( QIcon(":/images/new.png"), "Создать", this );
    ui->mainToolBar->addAction( actionNew );

    actionOpen = new QAction( QIcon(":/images/open.png"), "Открыть", this );
    ui->mainToolBar->addAction( actionOpen );

    actionSave = new QAction( QIcon(":/images/save.png"), "Сохранить", this );
    ui->mainToolBar->addAction( actionSave );

    actionRecognition = new QAction( QIcon(":/images/recognition.png"), "Распознать", this );
    ui->mainToolBar->addAction( actionRecognition );

    actionLearning = new QAction( QIcon(":/images/play.png"), "Старт обучения", this );
    ui->mainToolBar->addAction( actionLearning );

    actionAdd = new QAction( QIcon(":/images/add.png"), "Добавить символ", this );
    ui->mainToolBar->addAction( actionAdd );

    actionCleanPainter = new QAction( QIcon(":/images/clear.png"), "Очистить полотно", this );
    ui->mainToolBar->addAction( actionCleanPainter );

    mainToolbarSpacer = new QWidget( this );
    mainToolbarSpacer->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );
    ui->mainToolBar->addWidget( mainToolbarSpacer );

    mainToolbarStateGroup = new QActionGroup( this );

    actionStateLearn = new QAction( "Обучение", this );
    actionStateLearn->setCheckable( true );

    actionStateRecog = new QAction( "Распознавание", this );
    actionStateRecog->setCheckable( true );

    mainToolbarStateGroup->addAction( actionStateLearn );
    mainToolbarStateGroup->addAction( actionStateRecog );

    ui->mainToolBar->addAction( actionStateLearn );
    ui->mainToolBar->addAction( actionStateRecog );

    connect( actionNew,  SIGNAL(triggered()), this, SLOT(newRecognition()));
    connect( actionOpen, SIGNAL(triggered()), this, SLOT(openRecognition()));
    connect( actionSave, SIGNAL(triggered()), this, SLOT(saveRecognition()));

    connect( actionAdd, SIGNAL(triggered()), this, SLOT(addItem()));

    connect( actionCleanPainter, SIGNAL(triggered()), this, SLOT(cleanPainter()));
    connect( actionRecognition,  SIGNAL(triggered()), this, SLOT(startRecognition()));

    connect(actionLearning, SIGNAL(triggered()), this, SLOT(startLearning()));
}

void MainWindow::setupStatusBar()
{
    labelStatus = new QLabel();
    ui->statusBar->addWidget( labelStatus );
}

void MainWindow::setupInfoBlock()
{
    ui->deleteItemBtn->setIcon( QIcon(":/images/trash.png") );
    ui->deleteItemBtn->setIconSize( QSize( 24, 24 ) );

    connect(ui->deleteItemBtn, SIGNAL(clicked()), this, SLOT(deleteItem()));
}

void MainWindow::setupCombobox()
{
    ui->itemTypeComboBox->addItem( "A", QVariant(0) );
    ui->itemTypeComboBox->addItem( "B", QVariant(1) );
    ui->itemTypeComboBox->addItem( "C", QVariant(2) );
    ui->itemTypeComboBox->addItem( "D", QVariant(3) );
}

void MainWindow::setupElementsList()
{
    static bool isInitialized = false;

    if ( !isInitialized )
    {
        connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectItem(QModelIndex)));
        isInitialized = true;
    }

    ui->listWidget->clear();

    QList<ProjectSymbol>::iterator it;
    for ( it = project.Symbols.begin(); it != project.Symbols.end(); ++it )
    {
        QString description = QString( "Буква %1 %2" )
                                .arg( ProjectSymbol::GetNameFromType( it->Type ) )
                                .arg( it->Name );

        ui->listWidget->addItem( description );
    }
}

void MainWindow::setupTable()
{
    ui->tableWidget->setRowCount( IMAGE_HEIGHT );
    ui->tableWidget->setColumnCount( IMAGE_WIDTH );

    ui->tableWidget->verticalHeader()->setVisible( false );
    ui->tableWidget->horizontalHeader()->setVisible( false );

    for ( int i = 0; i < IMAGE_HEIGHT; i++ )
    {
        ui->tableWidget->setRowHeight( i, CELL_SIZE );
    }

    for ( int i = 0; i < IMAGE_WIDTH; i++ )
    {
        ui->tableWidget->setColumnWidth( i, CELL_SIZE );
    }

    for ( int row = 0; row < ui->tableWidget->rowCount(); row++ )
    {
        for ( int collumn = 0; collumn < ui->tableWidget->columnCount(); collumn++ )
        {
            isSelected[row][collumn] = false;
            tableItems[row][collumn] = new QTableWidgetItem();
            ui->tableWidget->setItem( row, collumn, tableItems[row][collumn] );
        }
    }

    ui->tableWidget->setFocusPolicy( Qt::NoFocus );
    ui->tableWidget->setEditTriggers( QAbstractItemView::NoEditTriggers );

    ui->tableWidget->setStyleSheet( "QTableWidget::item:selected{ background-color: black; }" );

    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(cellSelected(int,int)));
}

void MainWindow::newRecognition()
{
    int status = QMessageBox::warning( this, "Создание нового проекта",
                                       "Вы точно хотите создать новый проект? <br> <b>Это приведёт к удалению введенных данных!</b>",
                                       QMessageBox::Yes,
                                       QMessageBox::No );
    if ( status == QMessageBox::No )
    {
        return;
    }

    labelStatus->setText( "Начало нового проекта" );

    project = ProjectStructure();

    this->clearWorkArea();
    this->setupElementsList();
}

void MainWindow::openRecognition()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Открыть файл проекта:",
                                                    "",
                                                     "Neuro Network Project (*.nnpro)");
    if ( fileName.size() == 0 )
    {
        return;
    }

    ProjectStructure newProject;

    ProjectReader reader( fileName );
    reader.Read( newProject );

    project = newProject;

    this->setupElementsList();

    labelStatus->setText( "Проект успешно открыт" );
}

void MainWindow::saveRecognition()
{
    labelStatus->setText( "Проект сохранен" );

    QString fileName =
            QFileDialog::getSaveFileName( this,
                                          "Сохранить как:",
                                          "",
                                          "Neuro Network Project (*.nnpro)");

    if ( fileName.size() == 0 )
    {
        return;
    }

    ProjectWriter writer ( fileName );
    writer.Save( project );
}

void MainWindow::startRecognition()
{
    labelStatus->setText( "Идет распознание буквы..." );

    float data[IMAGE_WIDTH * IMAGE_HEIGHT];

    convertTable(data);

    for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; i++) {
        std::cout << data[i] << " ";
    }

    for (int i = 0; i < 4; i++) {
        float result = network->Recognize(data, i);

        std::cout << result << std::endl;

        if (result > 0.5f) {

            if(i == 0) ui->checkBoxA->setChecked(true);
            if(i == 1) ui->checkBoxB->setChecked(true);
            if(i == 2) ui->checkBoxC->setChecked(true);
            if(i == 3) ui->checkBoxD->setChecked(true);

            break;
        }
    }


    labelStatus->setText( "Распознание буквы закончено!" );
}

void MainWindow::cleanPainter()
{
    for ( int i = 0; i < IMAGE_HEIGHT; i++ )
    {
        for ( int j = 0; j < IMAGE_WIDTH; j++ )
        {
            isSelected[i][j] = false;
        }
    }

    paintTable();
}

void MainWindow::cellSelected( int x, int y )
{
    isSelected[x][y] = !isSelected[x][y];
    this->paintCell( x, y );
}

void MainWindow::addItem()
{
    ProjectSymbol symbol;

    symbol.Name = ui->itemNameEdit->text();
    symbol.Type = ui->itemTypeComboBox->currentIndex();
    symbol.Description = ui->itemDescriptionEdit->toPlainText();

    for ( int i = 0; i < IMAGE_HEIGHT; i++ )
    {
        for ( int j = 0; j < IMAGE_WIDTH; j++ )
        {
            symbol.Data[i][j] = this->isSelected[i][j];
        }
    }

    if ( symbol.Name.size() == 0 )
    {
        QPalette palette = ui->itemNameEdit->palette();

        palette.setColor( QPalette::Base, Qt::red );
        palette.setColor( QPalette::Text, Qt::white );

        ui->itemNameEdit->setPalette( palette );

        return;
    }
    else
    {
        QPalette palette = ui->itemNameEdit->palette();

        palette.setColor( QPalette::Base, Qt::white );
        palette.setColor( QPalette::Text, Qt::black );

        ui->itemNameEdit->setPalette( palette );
    }

    this->clearWorkArea();

    project.Symbols.push_back( symbol );

    this->setupElementsList();
}

void MainWindow::deleteItem()
{
    if ( currentSelectedId == NO_SELECT ) { return; }

    if ( currentSelectedId < 0 ) { return; }
    if ( currentSelectedId > ( project.Symbols.size() - 1 ) ) { return; };

    int status = QMessageBox::warning( this, "Удаление элемента",
                                       "Вы точно хотите удалить элемент?",
                                       QMessageBox::Yes,
                                       QMessageBox::No );

    if ( status == QMessageBox::No ) { return; }

    project.Symbols.removeAt( currentSelectedId );

    this->clearWorkArea();
    this->setupElementsList();

    labelStatus->setText( "Элемент удален" );
}

void MainWindow::selectItem( const QModelIndex &index )
{
    int id = index.row();

    if ( id < 0 ) { return; }
    if ( id > ( project.Symbols.size() - 1 ) ) { return; };

    currentSelectedId = id;

    ProjectSymbol symbol = project.Symbols.at( id );

    ui->itemNameEdit->setText( symbol.Name );
    ui->itemTypeComboBox->setCurrentIndex( symbol.Type );
    ui->itemDescriptionEdit->setText( symbol.Description );

    for ( int i = 0; i < IMAGE_HEIGHT; i++ )
    {
        for ( int j = 0; j < IMAGE_WIDTH; j++ )
        {
            isSelected[i][j] = symbol.Data[i][j];
        }
    }

    paintTable();
}

void MainWindow::paintTable()
{
    for ( int row = 0; row < ui->tableWidget->rowCount(); row++ )
    {
        for ( int collumn = 0; collumn < ui->tableWidget->columnCount(); collumn++ )
        {
            this->paintCell( row, collumn );
        }
    }
}

void MainWindow::paintCell( int x, int y )
{
    QTableWidgetItem *item = ui->tableWidget->item( x, y );

    QColor color;
    QString style = "QTableWidget::item:selected{ background-color: %1; }";

    if ( isSelected[x][y] )
    {
        ui->tableWidget->setStyleSheet( style.arg("black") );
        color = Qt::black;
    }
    else
    {
        ui->tableWidget->setStyleSheet( style.arg("white") );
        color = Qt::white;
    }

    item->setBackgroundColor( color );
}

void MainWindow::clearWorkArea()
{
    this->cleanPainter();

    ui->itemNameEdit->setText("");
    ui->itemDescriptionEdit->setText("");
    ui->itemTypeComboBox->setCurrentIndex(0);
}

void MainWindow::convertTable(float *data) {
    int counter = 0;
    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        for (int j = 0; j < IMAGE_WIDTH; j++) {
            data[counter] = static_cast<float>(isSelected[i][j]);
            counter++;
        }
    }
}

void MainWindow::startLearning() {
    labelStatus->setText( "Идет обучение нейронной сети ..." );

    for (int i = 0; i < 4; i++) {

        QList<ProjectSymbol>::iterator it;
        for ( it = project.Symbols.begin(); it != project.Symbols.end(); ++it )
        {
            float resultNeed = (i == it->Type) ? 1.0f : 0.0f;

            float data[IMAGE_WIDTH * IMAGE_HEIGHT];

            int counter = 0;
            for (int t = 0; t < IMAGE_HEIGHT; t++) {
                for (int k = 0; k < IMAGE_WIDTH; k++) {
                    data[counter] = static_cast<float>(it->Data[t][k]);
                    counter++;
                }
            }


            network->Teaching(data, i, resultNeed);
        }

    }

    labelStatus->setText( "Обучение нейронной сети закончено ..." );
}
