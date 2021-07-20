#ifndef FRAMEBROWSER_H
#define FRAMEBROWSER_H

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ui_FrameBrowser.h"

#include "ChooseColor.h"

#include "ViewFilesystem.h"
#include "ViewWorkspace.h"
#include "ViewMessages.h"

#include <QCloseEvent>
#include <QMainWindow>
#include <QWidget>

class FrameBrowser
    : public QMainWindow,
      private Ui_FrameBrowser
{
  Q_OBJECT
public:
  explicit FrameBrowser(QWidget * /*parent*/ = nullptr);

  static FrameBrowser *createNewBrowser() { return new FrameBrowser; }
  static FrameBrowser *provideBrowserFor(const QString & /*what*/);

  void tile(const QMainWindow *previous);
  void updateUi();

  void navigateTo(const QString &);

protected:
  void closeEvent(QCloseEvent *event) override;

signals:

private:
  QString path_bound_;
  QPointer<ViewFilesystem> files_;
  QPointer<ViewWorkspace> wkspace_;
  QPointer<ViewMessages> messages_;

  QPointer<ChooseColor> choose_color_;

  void setupActions();
  void setupToolBars();
  void setupDockViews();

  void readSettings();
  void writeSettings();
private slots:
  void setSceneBgColor();
  void loadIndexedFile(const QModelIndex &);

  void on_actionClose__triggered();
  void on_actionExit__triggered();

  void on_actionAbout__triggered();
  void on_actionAboutQt__triggered();

  void on_actionNew__triggered();
  void on_actionClone__triggered();

  void on_actionSave__triggered();
  void on_actionSaveAs__triggered();
  void on_actionOpen__triggered();

  void on_actionSetFont__triggered();
  void on_actionElements__triggered();

  void on_actionMoleculeFast__triggered();
  void on_actionMoleculeAsSticks__triggered();
  void on_actionMoleculeAsBallsSticks__triggered();
  void on_actionMoleculeAsCPK__triggered();

  void on_actionProperties__triggered();
  void on_actionTextSource__triggered();
  void on_actionViewMolecule__triggered();
};

#endif // FRAMEBROWSER_H
