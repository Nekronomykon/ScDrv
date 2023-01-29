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
#include <QLineEdit>
#include <QPointer>
#include <QSettings>
#include <QMainWindow>
#include <QCloseEvent>
#include <QSessionManager>

#include "FrameDoc.h"

#include "ViewFileSystem.h"
#include "ViewWorkspace.h"

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

  void addPathList(const QStringList & /* list_paths */);

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
  bool queryDataSaved();

private:
  QPointer<QLineEdit> editBgRed_;
  QPointer<QLineEdit> editBgGreen_;
  QPointer<QLineEdit> editBgBlue_;

  QPointer<ViewWorkspace> workspace_;
  QPointer<ViewFileSystem> files_;

private slots:
  void commitData(QSessionManager & /*mgr */);
  void on_actionFileRecent(void);
  /* File menu */
  void on_actionNew__triggered(void);
  void on_actionOpen__triggered(void);
  void on_actionClose__triggered(void);
  void on_actionExit__triggered(void);

  /* Help menu */
  void on_actionAbout__triggered(void);
  void on_actionAboutQt__triggered(void);
};

#endif //! Frame_Top_h__
