#ifndef TILETEMPLATESETSVIEW_H
#define TILETEMPLATESETSVIEW_H

#include "savabletiletemplateset.h"
#include "tiletemplatesetsmanager.h"

#include <QWidget>
#include <QTabWidget>
#include <QListView>

/**
 * @brief The TileTemplateSetsView class
 * Stores and displays all open TileTemplateSets.
 * Has a tool bar for saving/loading/creating sets,
 * and modifying existing sets
 */
class TileTemplateSetsView : public QWidget
{
    Q_OBJECT

public:
    explicit TileTemplateSetsView(TileTemplateSetsManager *tileTemplateSetsManage,
                                  QWidget *parent = nullptr);

signals:
    void tileTemplateChanged(TileTemplate *tileTemplate);

private slots:
    void tileTemplateSetAdded(SavableTileTemplateSet *tileTemplateSet);
    void tileTemplateSetAboutToBeRemoved(SavableTileTemplateSet *tileTemplateSet);

    void selectedTileTemplateChanged();

    void addTemplate();
    void removeTemplate();

    void addTemplateSet();
    void removeTemplateSet();
    void saveTemplateSet();
    void loadTemplateSet();

private:
    void tileTemplateSetSaveStatusChanged(SavableTileTemplateSet *tileTemplateSet, bool status);

    TileTemplateSetsManager *mTileTemplateSetsManager;
    QList<QListView *> mListViews;

    QTabWidget *mTabs;
};

#endif // TILETEMPLATESETSVIEW_H
