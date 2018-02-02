#include "editor.h"

#include "newmapdialog.h"
#include "meshviewcontainer.h"
#include "tilemapbrushtool.h"

#include <QDockWidget>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>

Editor::Editor(QObject *parent)
    : QObject(parent)
    , mMainWindow(new QMainWindow())
    , mTileMap(nullptr)
    , mMapView(new MapView(mTileMapSelectedRegion, mMainWindow))
    , mTileMapToolManager(new TileMapToolManager(this))
    , mToolBar(new QToolBar(mMainWindow))
{
    //Initiallize mMainWindow
    mMainWindow->setCentralWidget(mMapView);
    setUpMenuBar();
    mMainWindow->addToolBar(mToolBar);

    mToolBar->addAction(mTileMapToolManager->registerTool(QSharedPointer<AbstractTileMapTool>(new TileMapBrushTool(mTileMap)), "Brush Tool"));

    //Set up and add all dock widgets
    QDockWidget *dw = new QDockWidget("Mesh View", mMainWindow);
    MeshViewContainer *mvc = new MeshViewContainer(dw);
    dw->setWidget(mvc);

    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, dw);

    //Create widget connections
    connect(mMapView, &MapView::cellActivated,
            mTileMapToolManager, &TileMapToolManager::cellActivated);
    connect(mMapView, &MapView::cellClicked,
            mTileMapToolManager, &TileMapToolManager::cellClicked);
    connect(mMapView, &MapView::cellReleased,
            mTileMapToolManager, &TileMapToolManager::cellReleased);

    mMainWindow->showMaximized();
}

Editor::~Editor()
{
    delete mMainWindow;
}

void Editor::newMap()
{
    NewMapDialog nmd;
    nmd.exec();

    if (nmd.result.width != -1) {
        if (mTileMap)
            delete mTileMap;
        mTileMap = new TileMap(QSize(nmd.result.width, nmd.result.height), this);
        mTileMapToolManager->setTileMap(mTileMap);

        mMapView->createMap(mTileMap);
    }
}

void Editor::saveMap()
{
    mTileMap->setDepend(SharedTileTemplateSet::create());
    //mTileMap->updateDepend();
    if(mTileMap->savePath().isEmpty()){
        mTileMap->setSavePath(QFileDialog::getSaveFileName(mMainWindow,
            tr("Save Map"), "/home", tr("Save Files (*.xml)")));
    }
    for(SharedTileTemplateSet set: mTileMap->dependencies()){
        qDebug()<<"save dependencies...";
        if(set->savePath().isEmpty()){
            set->setSavePath(QFileDialog::getSaveFileName(mMainWindow,
                tr("Save Templates"), "/home", tr("Save Files (*.xml)")));
        }
    }
    XMLTool::saveTileMap(QSharedPointer<TileMap>(mTileMap));
}

void Editor::loadMap()
{
    QString fileName = QFileDialog::getOpenFileName(mMainWindow,
        tr("Open Map"), "/home", tr("Open Files (*.xml)"));

    SharedTileMap tilemap = XMLTool::openTileMap(fileName);
    mTileMap = tilemap.data();
    mTileMapToolManager->setTileMap(mTileMap);
    mMapView->createMap(mTileMap);
}

void Editor::exportMapMesh()
{
    MeshViewContainer *meshViewContainer = mMainWindow->findChild<MeshViewContainer *>();

    if (meshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(mMainWindow,
        tr("Export OBJ"), "/home", tr("Export Files (*.obj)"));

    if(!fileName.isEmpty())
        meshViewContainer->saveMesh(fileName);


    /*MeshViewContainer *meshViewContainer = findChild<MeshViewContainer *>();

    if (meshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("import OBJ"), "/home", tr("import Files (*.obj)"));

    if(!fileName.isEmpty())
        meshViewContainer->loadMesh(fileName);
    */
}

void Editor::setUpMenuBar()
{
    QMenuBar *menuBar = mMainWindow->menuBar();
    menuBar->clear();

    QMenu *fileMenu = menuBar->addMenu(tr("File"));
    fileMenu->addAction(tr("New Map"), this, &Editor::newMap);
    fileMenu->addAction(tr("Save Map"), this, &Editor::saveMap);
    fileMenu->addAction(tr("Load Map"), this, &Editor::loadMap);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Export Map Mesh"), this, &Editor::exportMapMesh);
}
