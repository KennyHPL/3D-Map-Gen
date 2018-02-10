#ifndef TILETEMPLATESETSVIEW_H
#define TILETEMPLATESETSVIEW_H

#include "tiletemplateset.h"

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
    TileTemplateSetsView(QWidget *parent = nullptr);

    void addTileTemplateSet(SharedTileTemplateSet tileTemplateSet);
    void removeCurrentTileTemplateSet();

    const QList<SharedTileTemplateSet> &tileTemplateSets() const { return mTileTemplateSets; }

signals:
    void tileTemplateChanged(SharedTileTemplate tileTemplate);

    void tileTemplateAboutToBeRemoved(const SharedTileTemplate tileTemplate);
    void tileTemplateSetAboutToBeRemoved(const SharedTileTemplateSet tileTemplateSet);

private slots:
    void selectedTileTemplateChanged();

    void addTemplate();
    void removeTemplate();

    void addTemplateSet();
    void removeTemplateSet();
    void saveTemplateSet();
    void loadTemplateSet();

private:
    void tileTemplateSetSaveStatusChanged(SharedTileTemplateSet tileTemplateSet, bool status);

    QList<SharedTileTemplateSet> mTileTemplateSets;
    QList<QListView *> mListViews;

    QTabWidget *mTabs;
};

#endif // TILETEMPLATESETSVIEW_H
