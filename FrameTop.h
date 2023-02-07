#ifndef Frame_Top_h__
#define Frame_Top_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

#include <QAction>
#include <QPointer>
#include <QLineEdit>
#include <QSettings>
#include <QValidator>
#include <QDoubleValidator>
#include <QMainWindow>
#include <QCloseEvent>
#include <QSessionManager>

#include "FrameDoc.h"

#include "ViewFileSystem.h"
#include "ViewWorkspace.h"

#include "QVTKNamedColors.h"

#include <vtkStdString.h>

#include "ui_FrameTop.h"

class FrameTop : public QMainWindow,
                 private Ui_FrameTop
{
  Q_OBJECT
public:
  explicit FrameTop(QWidget * /*parent*/ = nullptr);
  ~FrameTop() override = default;

  static FrameTop *createNew(const QWidget * /* parent */ = nullptr);

  FrameTop *tileAgainst(const QWidget * /* prev */ = nullptr);
  bool addPath(const fs::path & /* path */); // by reference or by value?

  FrameTop *addPathList(const QStringList & /* list_paths */);

protected:
  enum
  {
    MaxRecentFiles = 9
  };
  QAction *actionsRecent_[MaxRecentFiles] = {nullptr};

  void closeEvent(QCloseEvent *event) override;
  //
  FrameTop *setupActions();
  FrameTop *setupToolBars();
  FrameTop *setupDockViews();
  //
  FrameTop *readSettings(void);
  FrameTop *saveSettings(void);
  //
  bool queryDataSaved(void);
  //
  void updateBackgroundInfo(void);

private:
  QPointer<QLineEdit> editBgRed_;
  QPointer<QLineEdit> editBgGreen_;
  QPointer<QLineEdit> editBgBlue_;
  QPointer<QVTKNamedColors> colorsBg_;
  //
  QPointer<ViewWorkspace> workspace_;
  QPointer<ViewFileSystem> files_;

private slots:
  // void on_changeDocumentPath(QListWidgetItem *current, QListWidgetItem *previous);
  void commitData(QSessionManager & /*mgr */);
  void on_actionFileRecent(void);
  /* Background color for the scene */
  void on_activatedPathIndex(const QModelIndex &);
  // void on_changeInputPath(const QString &name);
  void on_nameBackgroundColor(const QString & /* name */);
  void on_changedBackgroundRed(void);
  void on_changedBackgroundGreen(void);
  void on_changedBackgroundBlue(void);
  //
  void on_changedClipboardData(void);

  /* File menu */
  void on_actionNew__triggered(void);
  void on_actionOpen__triggered(void);
  void on_actionSave__triggered(void);
  void on_actionSaveAs__triggered(void);
  void on_actionExport__triggered(void);
  void on_actionClose__triggered(void);
  void on_actionExit__triggered(void);
  void on_actionProperties__triggered(void);
  /* Help menu */
  void on_actionAbout__triggered(void);
  void on_actionAboutQt__triggered(void);
};

#endif //! Frame_Top_h__
