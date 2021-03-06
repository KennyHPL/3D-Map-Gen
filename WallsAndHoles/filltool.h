#ifndef FILLTOOL_H
#define FILLTOOL_H

#include <QSet>
#include <QPoint>
#include <QUndoStack>

#include "abstracttilemaptool.h"
#include "mapview.h"

class FillTool : public AbstractTileMapTool {
    Q_OBJECT

public:
    FillTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack);

    /**
     * @brief This will immediately fill an area.
     * @param x  The X position of the clicked tile.
     * @param y  The Y position of the clicked tile.
     */
    void cellClicked(int x, int y, QMouseEvent *) override;


    /**
     * @brief This will show a preview for the selected area.
     */
    void cellHovered(int x, int y, QMouseEvent *) override;

    void toolTileMapChanged(TileMap *prev) override;


    void mouseExitedMap(QMouseEvent *) override;
    void deactivate() override;

public slots:
    void invalidateSelection();

protected:
    /// Updates mSelection to match the points that will be filled if
    /// the given point is selected.
    void updateSelection(int x, int y);

    /// The set of points that will be filled in.
    QSet<QPoint> mSelection;

    /// Draws an overlay previewing the area that will be filled.
    void drawOverlay(int endX, int endY);

    /// Clears the overlay.
    void clearOverlay();


    QUndoStack *mUndoStack;
};

#endif // FILLTOOL_H
