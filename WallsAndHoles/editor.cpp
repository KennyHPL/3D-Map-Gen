#include "editor.h"

#include "newmapdialog.h"
#include "meshviewcontainer.h"
#include "tilemapbrushtool.h"

#include <QDockWidget>
#include <QApplication>
#include <QDebug>

Editor::Editor(QObject *parent)
    : QObject(parent)
    , mMainWindow(new MainWindow())
    , mMap2Mesh(nullptr)
    , mTileMap(nullptr)
    , mMapView(new MapView(mTileMapSelectedRegion, mMainWindow))
    , mTileMapToolManager(new TileMapToolManager(this))
    , mToolBar(new QToolBar(mMainWindow))
{
    //Initialize mMainWindow
    mMainWindow->show();
    mMainWindow->setCentralWidget(mMapView);

    //Create and add Main Tools
    QAction *newMap = mToolBar->addAction("New Map");
    connect(newMap, &QAction::triggered, this, &Editor::createNewMap);
    mMainWindow->addToolBar(mToolBar);

    mToolBar->addAction(mTileMapToolManager->registerTool(QSharedPointer<AbstractTileMapTool>(new TileMapBrushTool(mTileMap)), "Brush Tool"));

    //Set up and add all dock widgets
    QDockWidget *dw = new QDockWidget("Mesh View", mMainWindow);
    mMeshViewContainer = new MeshViewContainer(dw);
    dw->setWidget(mMeshViewContainer);

    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, dw);

    //Create widget connections
    connect(mMapView, &MapView::cellActivated,
            mTileMapToolManager, &TileMapToolManager::cellActivated);
    connect(mMapView, &MapView::cellClicked,
            mTileMapToolManager, &TileMapToolManager::cellClicked);
    connect(mMapView, &MapView::cellReleased,
            mTileMapToolManager, &TileMapToolManager::cellReleased);
}

Editor::~Editor()
{
    delete mMainWindow;
}

void Editor::createNewMap()
{
    NewMapDialog nmd;
    nmd.exec();

    if (nmd.result.width != -1) {
        // TODO: delete mTileMap
        mTileMap = new TileMap(QSize(nmd.result.width, nmd.result.height), this);
        mTileMapToolManager->setTileMap(mTileMap);

        mMapView->createMap(mTileMap);

        if (mMap2Mesh != nullptr)
            delete mMap2Mesh;

        mMap2Mesh = new Map2Mesh(mTileMap, this);
        connect(mMap2Mesh, &Map2Mesh::mapUpdated, this, &Editor::updateScene);
    }
}

void Editor::updateScene()
{
    QSharedPointer<Scene> scene = QSharedPointer<Scene>::create();

    for (auto&& obj : mMap2Mesh->getMeshes())
        scene->addObject(obj);

    mMeshViewContainer->setScene(scene);
}
