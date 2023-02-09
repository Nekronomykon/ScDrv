#include "FrameTop.h"

#include <algorithm>

#include <QDir>
#include <QMimeData>
#include <QFileInfo>
#include <QSettings>
#include <QClipboard>
#include <QByteArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QDockWidget>
#include <QDoubleValidator>

#include "TableElements.h"
#include "DialogFileProperties.h"

#include <vtkVersion.h>
#include <vtkNamedColors.h>

namespace
{
  ///////////////////////////////////////////////////////////////////////
  //
  static inline QString keyRecentFiles()
  {
    return QStringLiteral("FileRecent");
  }
  //
  static inline QString keyBackColor()
  {
    return QStringLiteral("BackgroundColor");
  }
  //
  static inline QString keyGeometry()
  {
    return QStringLiteral("Geometry");
  }
  //
  static inline QString keyFrameState()
  {
    return QStringLiteral("FrameState");
  }
  //
  static inline QString keyFile()
  {
    return QStringLiteral("File");
  }
  //
  ///////////////////////////////////////////////////////////////////////
  //
  static bool hasRecentFiles()
  {
    QSettings settings;
    const int count = settings.beginReadArray(keyRecentFiles());
    settings.endArray();
    return count > 0;
  }
  //
  static QStringList readRecentFilesFrom(QSettings &settings)
  {
    QStringList result;
    const int count = settings.beginReadArray(keyRecentFiles());
    for (int i = 0; i < count; ++i)
    {
      settings.setArrayIndex(i);
      result.append(settings.value(keyFile()).toString());
    }
    settings.endArray();
    return result;
  }
  //
  // ...defaulted
  static QStringList readRecentFiles()
  {
    QSettings settings;
    // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QStringList result;
    const int count = settings.beginReadArray(keyRecentFiles());
    for (int i = 0; i < count; ++i)
    {
      settings.setArrayIndex(i);
      result.append(settings.value(keyFile()).toString());
    }
    settings.endArray();
    return result;
    // == return readRecentFilesFrom(settings);
  }
  //
  static void writeRecentFilesTo(const QStringList &files, QSettings &settings)
  {
    const int count = files.size();
    settings.beginWriteArray(keyRecentFiles());
    for (int i = 0; i < count; ++i)
    {
      settings.setArrayIndex(i);
      settings.setValue(keyFile(), files.at(i));
    }
    settings.endArray();
  }
  static void writeAsRecentFiles(const QStringList &files)
  {
    QSettings settings;
    // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = files.size();
    settings.beginWriteArray(keyRecentFiles());
    for (int i = 0; i < count; ++i)
    {
      settings.setArrayIndex(i);
      settings.setValue(keyFile(), files.at(i));
    }
    settings.endArray();
    // == writeRecentFilesTo(settings);
  }
  //
  ///////////////////////////////////////////////////////////////////////
}; // *anonymous*
//
FrameTop *FrameTop::readSettings(void)
{
  QSettings settings;
  // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
  const QByteArray geometry = settings.value(keyGeometry(), QByteArray()).toByteArray();
  if (geometry.isEmpty())
  {
    const QRect availableGeometry = screen()->availableGeometry();
    this->resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
    this->move((availableGeometry.width() - width()) / 2,
               (availableGeometry.height() - height()) / 2);
  }
  else
  {
    this->restoreGeometry(geometry);
  }
  this->restoreState(settings.value(keyFrameState(), QByteArray()).toByteArray());
  // now children:
  if (workspace_)
    workspace_->readSettings(settings);
  if (files_)
    files_->readSettings(settings);
  if (frameDoc_)
    frameDoc_->readSettings(settings);
  return this;
}

FrameTop *FrameTop::saveSettings(void)
{
  QSettings settings;
  // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
  settings.setValue(keyGeometry(), this->saveGeometry());
  settings.setValue(keyFrameState(), this->saveState());
  // now do the children:
  if (workspace_)
    workspace_->saveSettings(settings);
  if (files_)
    files_->saveSettings(settings);
  if (frameDoc_)
    frameDoc_->saveSettings(settings);
  return this;
}
//

///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::tileAgainst
///
FrameTop *FrameTop::tileAgainst(const QWidget *prev)
{
  if (prev)
  {
    int topFrameWidth = prev->geometry().top() - prev->pos().y();
    if (!topFrameWidth)
      topFrameWidth = 25;
    const QPoint pos = prev->pos() + 2 * QPoint(topFrameWidth, topFrameWidth);
    if (screen()->availableGeometry().contains(this->rect().bottomRight() + pos))
      this->move(pos);
  }
  return this;
}
///

FrameTop::FrameTop(QWidget *parent)
    : QMainWindow(parent),
      editBgRed_(new QLineEdit(this)), editBgGreen_(new QLineEdit(this)), editBgBlue_(new QLineEdit(this)),
      colorsBg_(new QVTKNamedColors(this)),
      workspace_(new ViewWorkspace(this)),
      files_(new ViewFileSystem(this))
{
  //
  this->setupUi(this);

  connect(qApp, &QGuiApplication::commitDataRequest,
          this, &FrameTop::commitData);
  // clipboard sniffer
  connect(QGuiApplication::clipboard(), &QClipboard::dataChanged,
          this, &FrameTop::on_changedClipboardData);

  editBgRed_->setPlaceholderText(tr("[red]"));
  editBgGreen_->setPlaceholderText(tr("[green]"));
  editBgBlue_->setPlaceholderText(tr("[blue]"));

  //
  this->setupActions()
      ->setupToolBars()
      ->setupDockViews();
  //
  this->readSettings();
  this->setUnifiedTitleAndToolBarOnMac(true); // Actually I don't know, what is it about
}
//
///
bool FrameTop::queryDataSaved()
{
  if (!frameDoc_->isModified())
    return true;
  QString query(tr("The document "));
  if (frameDoc_->hasPath())
  {
    query += tr("under the path:\n");
    query += frameDoc_->getPath().c_str();
    query += '\n';
  }
  query += tr("is modified\nDo you want to save the changes?");

  auto buttons = QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel;
  QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("ScDrv"), query, buttons);

  switch (ret)
  {
  case QMessageBox::Save:
    return frameDoc_->doSave();
  case QMessageBox::Cancel:
    return false;
  default:
    break;
  }
  return true;
}
//
void FrameTop::updateBackgroundInfo(void)
{
  WidgetMolecule *pEdit = frameDoc_->editMolecule();
  if (!pEdit)
    return; // no object
  auto *pView = pEdit->getView();
  if (!pView)
    return; // no object
  // const vtkColor3d& clrBg = pView->getBackgroundColor();
  QString txt;
  txt.setNum(pView->getBackgroundColor().GetRed(), 'f' /* , 8 */);
  editBgRed_->setText(txt);
  txt.setNum(pView->getBackgroundColor().GetGreen(), 'f' /* , 8 */);
  editBgGreen_->setText(txt);
  txt.setNum(pView->getBackgroundColor().GetBlue(), 'f' /* , 8 */);
  editBgBlue_->setText(txt);
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::addPath
///
bool FrameTop::addPath(const fs::path &the_path)
{
  workspace_->addPathString(tr(the_path.c_str()));
  return true;
}
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::addPathList
///
FrameTop *FrameTop::addPathList(const QStringList &list_paths)
{
  std::for_each(list_paths.begin(), list_paths.end(),
                [&](const QString &one_to_open) -> void
                {
                      fs::path a_path = fs::absolute(fs::path(one_to_open.toLocal8Bit().data()));
                      switch (fs::status(a_path).type())
                      {
                      case (fs::file_type::not_found):
                      {
                          this->addPath(a_path); // possibly a new file
                                                 // --> create a doc for it later
                          break;
                      }
                      case (fs::file_type::regular):
                      {
                          this->addPath(a_path); // a regular file
                                                 // --> open it and read the doc data from it
                          break;
                      }
                      case (fs::file_type::directory):
                      {
                          // a directory
                          // --> traverse and probably recurse it?
                          break;
                      }
                      case (fs::file_type::symlink):
                      {
                          // a symbolic link
                          // --> resolve it?
                          break;
                      }
                      default: // nothing else matters; probably fifo or pipe would be useful
                          break;
                      } });
  return this;
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop ::setupActions
///
FrameTop *FrameTop::setupActions(void)
{
  //! [implicit tr context]
  const QIcon iconNewWindow = QIcon::fromTheme("window-new", QIcon(":/images/New.png"));
  actionNewWindow_->setIcon(iconNewWindow);

  const QIcon iconNew = QIcon::fromTheme("document-new" /*, QIcon(":/images/NewDoc.png")*/);
  actionNew_->setIcon(iconNew);
  actionNew_->setShortcuts(QKeySequence::New);
  actionNew_->setStatusTip(tr("Create a new file"));

  const QIcon iconOpen = QIcon::fromTheme("document-open", QIcon(":/images/Open.png"));
  actionOpen_->setIcon(iconOpen);
  actionOpen_->setShortcuts(QKeySequence::Open);
  actionOpen_->setStatusTip(tr("Open an found file"));

  const QIcon iconReload = QIcon::fromTheme("document-revert", QIcon(":/images/Reload.png"));
  actionReload_->setIcon(iconReload);
  actionReload_->setShortcuts(QKeySequence::Refresh);
  actionReload_->setStatusTip(tr("Reload content from file"));

  menuRecent_->removeAction(actionRecentFiles_);
  for (int i = 0; i < MaxRecentFiles; ++i)
  {
    actionsRecent_[i] = menuRecent_->addAction(QString(), this, &FrameTop::on_actionFileRecent);
    actionsRecent_[i]->setVisible(false);
  }
  menuRecent_->addSeparator();
  menuRecent_->addAction(actionRecentFiles_);

  const QIcon iconSave = QIcon::fromTheme("document-save", QIcon(":/images/Save.png"));
  actionSave_->setIcon(iconSave);
  actionSave_->setShortcuts(QKeySequence::Save);
  actionSave_->setStatusTip(tr("Save the document to disk"));

  const QIcon iconSaveAs = QIcon::fromTheme("document-save-as", QIcon(":/images/SaveAs.png"));
  actionSaveAs_->setIcon(iconSaveAs);
  actionSaveAs_->setShortcuts(QKeySequence::SaveAs);
  actionSaveAs_->setStatusTip(tr("Save the document under a new name"));

  const QIcon iconClose = QIcon::fromTheme("window-close", QIcon(":/images/CloseFrame.png"));
  actionClose_->setIcon(iconClose);
  actionClose_->setShortcut(QKeySequence::Close);

  const QIcon iconExit = QIcon::fromTheme("application-exit", QIcon(":/images/Exit.png"));
  actionExit_->setIcon(iconExit);
  actionExit_->setShortcuts(QKeySequence::Quit);

  const QIcon iconUndo = QIcon::fromTheme("edit-undo", QIcon(":/images/Undo.png"));
  actionUndo_->setIcon(iconUndo);
  actionUndo_->setShortcut(QKeySequence::Undo);

  const QIcon iconRedo = QIcon::fromTheme("edit-redo", QIcon(":/images/Redo.png"));
  actionRedo_->setIcon(iconRedo);
  actionRedo_->setShortcut(QKeySequence::Redo);

  const QIcon iconCut = QIcon::fromTheme("edit-cut", QIcon(":/images/Cut.png"));
  actionCut_->setIcon(iconCut);
  actionCut_->setShortcuts(QKeySequence::Cut);
  actionCut_->setEnabled(false);
  // connect(frameDoc_->getTextSource()->editAtoms(),
  // &EditSource::copyAvailable, actionCut_, &QAction::setEnabled);

  const QIcon iconCopy = QIcon::fromTheme("edit-copy", QIcon(":/images/Copy.png"));
  actionCopy_->setIcon(iconCopy);
  actionCopy_->setShortcuts(QKeySequence::Copy);
  actionCopy_->setEnabled(false);
  // connect(frameDoc_->getTextSource()->editAtoms(),
  // &EditSource::copyAvailable, actionCopy_, &QAction::setEnabled);

  const QIcon iconPaste = QIcon::fromTheme("edit-paste", QIcon(":/images/Paste.png"));
  actionPaste_->setIcon(iconPaste);
  actionPaste_->setShortcuts(QKeySequence::Paste);

  const QIcon iconClear = QIcon::fromTheme("edit-clear", QIcon(":/images/Clear.png"));
  actionClear_->setIcon(iconClear);
  // actionClear_->setShortcut(QKeySequence::Clear);
  actionClear_->setEnabled(false);
  // connect(frameDoc_->getTextSource()->editAtoms(),
  // &EditSource::copyAvailable, actionClear_, &QAction::setEnabled);

  const QIcon iconClearAll = QIcon::fromTheme("edit-clear-all", QIcon(":/images/ClearAll.png"));
  actionClearAll_->setIcon(iconClearAll);

  const QIcon iconProps = QIcon::fromTheme("document-properties", QIcon(":/images/Options.png"));
  actionProperties_->setIcon(iconProps);

  const QIcon iconEditTxt(":/images/SrcEdit.png");
  actionTextSource_->setIcon(iconEditTxt);
  frameDoc_->setTabIcon(0, iconEditTxt);

  const QIcon iconFullScreen = QIcon::fromTheme("view-fullscreen", QIcon(":/images/FullScreen.png"));
  actionFullScreen_->setIcon(iconFullScreen);

  const QIcon iconPreview = QIcon::fromTheme("document-print-preview", QIcon(":/images/Preview.png"));
  actionPrintPreview_->setIcon(iconPreview);

  const QIcon iconCastSrc(":/images/SrcCast.png");
  actionViewMolecule_->setIcon(iconCastSrc);
  frameDoc_->setTabIcon(1, iconCastSrc);

  const QIcon iconMolFast(":/images/MolFast.png");
  actionViewMoleculeFast_->setIcon(iconMolFast);

  const QIcon iconMolBalls(":/images/MolBalls.png");
  actionViewMoleculeBalls_->setIcon(iconMolBalls);

  const QIcon iconMolStick(":/images/MolStick.png");
  actionViewMoleculeSticks_->setIcon(iconMolStick);

  const QIcon iconMolSphere(":/images/MolVDW.png");
  actionViewMoleculeSpheres_->setIcon(iconMolSphere);

  const QIcon iconProjOrtho(":/images/ProjOrtho.png");
  actionProjectOrthogonal_->setIcon(iconProjOrtho);
  const QIcon iconProjPersp(":/images/ProjPersp.png");
  actionProjectPerspective_->setIcon(iconProjPersp);
  const QIcon iconProjReset(":/images/ProjReset.png");
  actionProjectReset_->setIcon(iconProjReset);

  const QIcon iconShowBonds(":/images/ShowBond.png");
  actionShowBonds_->setIcon(iconShowBonds);
  const QIcon iconLabelAtoms(":/images/LblAtom.png");
  actionLabelAtoms_->setIcon(iconLabelAtoms);
  const QIcon iconLabelBonds(":/images/LblBond.png");
  actionLabelBonds_->setIcon(iconLabelBonds);

  const QIcon iconViewCoords(":/images/ViewCoord.png");
  actionViewCoordinates_->setIcon(iconViewCoords);
  frameDoc_->setTabIcon(3, iconViewCoords);

  const QIcon iconAbout = QIcon::fromTheme("help-about", QIcon(":/images/Help.png"));
  actionAbout_->setIcon(iconAbout);

  const QIcon iconAboutQt = QIcon::fromTheme("system-help", QIcon(":/images/AboutKit.png"));
  actionAboutQt_->setIcon(iconAboutQt);

  const QIcon iconSetFont = QIcon::fromTheme("preferences-desktop-font");
  // , QIcon(":/images/SetFont.png"));

  actionSetSourceFont_->setIcon(iconSetFont);

  const QIcon iconOptions = QIcon::fromTheme("preferences-system");
  // , QIcon(":/images/Options.png")

  actionOptions_->setIcon(iconOptions);

  //
  return this;
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop ::setupToolBars
///
FrameTop *FrameTop::setupToolBars(void)
{
  barTools_->addAction(actionNew_);
  barTools_->addAction(actionOpen_);
  barTools_->addAction(actionSave_);

  QToolBar *tbEdit = this->addToolBar(tr("Edit"));
  tbEdit->setObjectName("ToolbarEdit");
  tbEdit->addAction(actionUndo_);
  tbEdit->addAction(actionRedo_);
  tbEdit->addSeparator();
  tbEdit->addAction(actionCut_);
  tbEdit->addAction(actionCopy_);
  tbEdit->addAction(actionPaste_);
  tbEdit->addSeparator();
  tbEdit->addAction(actionClear_);
  tbEdit->addAction(actionClearAll_);

  QToolBar *tbText = this->addToolBar(tr("Text"));
  tbText->setObjectName("ToolbarText");
  tbText->addAction(actionTextSource_);
  tbText->addAction(actionSetSourceFont_);

  QToolBar *tbView = this->addToolBar(tr("View"));
  tbView->setObjectName("ToolbarView");
  tbView->addAction(actionViewMolecule_);

  tbView->addSeparator();
  tbView->addWidget(colorsBg_);
  connect(colorsBg_, &QVTKNamedColors::currentTextChanged, this, &FrameTop::on_nameBackgroundColor);
  //
  // --- [ ]
  tbView->addSeparator();
  tbView->addWidget(editBgRed_);
  editBgRed_->setMaximumWidth(75);
  connect(editBgRed_, &QLineEdit::editingFinished, this, &FrameTop::on_changedBackgroundRed);
  // --- [ ]
  tbView->addSeparator();
  tbView->addWidget(editBgGreen_);
  editBgGreen_->setMaximumWidth(75);
  connect(editBgGreen_, &QLineEdit::editingFinished, this, &FrameTop::on_changedBackgroundGreen);
  // --- [ ]
  tbView->addSeparator();
  tbView->addWidget(editBgBlue_);
  editBgBlue_->setMaximumWidth(75);
  connect(editBgBlue_, &QLineEdit::editingFinished, this, &FrameTop::on_changedBackgroundBlue);

  this->updateBackgroundInfo();
  //
  tbView->addSeparator();
  tbView->addAction(actionProjectOrthogonal_);
  tbView->addAction(actionProjectPerspective_);
  tbView->addAction(actionProjectReset_);
  tbView->addSeparator();
  tbView->addAction(actionViewGeometry_);
  tbView->addAction(actionViewCoordinates_);

  // ---- [ ] ----
  QToolBar *tbMol = this->addToolBar(tr("Molecule"));
  tbMol->setObjectName("ToolbarMolecule");
  tbMol->addAction(actionViewMoleculeFast_);
  tbMol->addSeparator();
  tbMol->addAction(actionViewMoleculeSticks_);
  tbMol->addAction(actionViewMoleculeBalls_);
  tbMol->addAction(actionViewMoleculeSpheres_);
  tbMol->addSeparator();
  tbMol->addSeparator();
  tbMol->addAction(actionShowBonds_);
  tbMol->addSeparator();
  tbMol->addAction(actionLabelAtoms_);
  tbMol->addAction(actionLabelBonds_);

  QToolBar *tbHelp = this->addToolBar(tr("Help"));
  tbHelp->setObjectName("ToolbarHelp");
  tbHelp->addAction(actionProperties_);
  tbHelp->addAction(actionOptions_);
  tbHelp->addAction(actionAbout_);
  tbHelp->addAction(actionAboutQt_);

  ///
  return this;
}
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop ::setupToolBars
///
FrameTop *FrameTop::setupDockViews(void)
{
  QDockWidget *pLeft = new QDockWidget(tr("Workspace"), this);
  pLeft->setWidget(workspace_);
  pLeft->setObjectName(tr("Workspace"));
  this->addDockWidget(Qt::LeftDockWidgetArea, pLeft);
  QObject::connect(workspace_->getPathList(), &ViewPathList::clicked,
                   this, &FrameTop::on_activatedPathIndex);
  // QObject::connect(workspace_->getPathList(), &ViewPathList::currentTextChanged, this, &FrameTop::on_changeInputPath);

  //
  QDockWidget *pNext = new QDockWidget(tr("Filesystem"), this);
  pNext->setWidget(files_);
  pNext->setObjectName(tr("Filesystem"));
  this->tabifyDockWidget(pLeft, pNext);
  //
  return this;
}
//
///////////////////////////////////////////////////////////////////////
// EVENTS:
//
void FrameTop::closeEvent(QCloseEvent *event)
{
  this->saveSettings(); // <-- here?
  if (this->queryDataSaved())
  {
    event->accept();
  }
  else
  {
    event->ignore();
  }
  // this->saveSettings();  <-- or here?
}
//
// SLOTS:
void FrameTop::commitData(QSessionManager &mgr)
{
  if (mgr.allowsInteraction())
  {
    if (!this->queryDataSaved())
      mgr.cancel();
  }
  else
  {
    // Lose unsaved???
    // Non-interactive: save without asking
    // if (frameDoc_->isModified())
    //     this->save();
  }
}
//
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::on_actionFileRecent
///
void FrameTop::on_actionFileRecent(void)
{
}
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::on_changeDocumentPath
///
void FrameTop::on_activatedPathIndex(const QModelIndex &idx)
{
  QString pathNew = workspace_->getPathList()->model()->data(idx).toString();
  if (pathNew.isEmpty() || !this->queryDataSaved())
    return;
  //
  frameDoc_->clearAll(false);
  frameDoc_->resetPath();
  //
  if (!pathNew.compare(ModelPathList::keyNewFile()))
    return;
  /* otherwise: try to load */
  QFileInfo fi(pathNew);
  // QMessageBox::information(this, tr("New path"), pathNew);
  FileFormat fmt = FrameDoc::FormatForSuffix(fi.suffix());
  Path path_to_read = fi.filesystemAbsoluteFilePath();
  if (fmt.isValidFormat() && fmt.hasRead() && this->queryDataSaved() && fmt.ReadTo(path_to_read, frameDoc_))
  {
    frameDoc_->resetPath(path_to_read);
    frameDoc_->resetFormat(fmt);
    frameDoc_->setModified(false);
  }
}
//
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::on_changeInputPath
///
// void FrameTop::on_changeInputPath(const QString &name)
//{
// }
//
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::on_nameBackgroundColor
///
void FrameTop::on_nameBackgroundColor(const QString &name)
{
  // QMessageBox::information(this, name, tr("Eh"), QMessageBox::Ok);
  WidgetMolecule *pEdit = frameDoc_->editMolecule();
  if (!pEdit) // assert(pEdit)
    return;
  auto *pView = pEdit->getView();
  if (!pView) // assert(pView)
    return;
  QByteArray ba = name.toLatin1();
  static vtkNew<vtkNamedColors> colors;
  colors->GetColor(ba.data(), pView->backgroundColor());
  pView->updateBackground();
  this->updateBackgroundInfo();
}
//
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::on_changedBackgroundRed
///
void FrameTop::on_changedBackgroundRed(void)
{
  WidgetMolecule *pEdit = frameDoc_->editMolecule();
  if (!pEdit) // assert(pEdit)
    return;
  auto *pView = pEdit->getView();
  if (!pView) // assert(pView)
    return;
  pView->backgroundColor().SetRed(editBgRed_->text().toDouble());
  pView->updateBackground();
  this->updateBackgroundInfo();
}
//
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::on_changedBackgroundGreen
///
void FrameTop::on_changedBackgroundGreen(void)
{
  WidgetMolecule *pEdit = frameDoc_->editMolecule();
  if (!pEdit) // assert(pEdit)
    return;
  auto *pView = pEdit->getView();
  if (!pView)
    return;
  pView->backgroundColor().SetGreen(editBgGreen_->text().toDouble());
  pView->updateBackground();
  this->updateBackgroundInfo();
}
//
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::on_changedBackgroundBlue
///
void FrameTop::on_changedBackgroundBlue(void)
{
  WidgetMolecule *pEdit = frameDoc_->editMolecule();
  if (!pEdit) // assert(pEdit)
    return;
  auto *pView = pEdit->getView();
  if (!pView)
    return;
  pView->backgroundColor().SetBlue(editBgBlue_->text().toDouble());
  pView->updateBackground();
  this->updateBackgroundInfo();
}
//
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::on_changedClipboardData
///
void FrameTop::on_changedClipboardData(void)
{
  QClipboard *pClipBoard = QGuiApplication::clipboard();
  if (!pClipBoard)
  {
    actionPaste_->setEnabled(false);
  }
  else
  {
    const QMimeData *pData = pClipBoard->mimeData(QClipboard::Clipboard);
    actionPaste_->setEnabled(!pData ? false : pData->hasText());
  }
}
//
///////////////////////////////////////////////////////////////////////
/// @brief FrameTop::createNew
/// @param parent --> widget, the
/// @return --> pointer to a new FrameTop instance
///
FrameTop *FrameTop::createNew(const QWidget *parent)
{
  FrameTop *pNew = new FrameTop();
  if (parent)
    pNew->tileAgainst(parent);
  return pNew;
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionExit__triggered
///
void FrameTop::on_actionExit__triggered(void)
{
  qApp->closeAllWindows();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionAboutQt__triggered
///
void FrameTop::on_actionAboutQt__triggered(void)
{
  qApp->aboutQt();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionAbout__triggered
///
void FrameTop::on_actionAbout__triggered(void)
{
  QString message(tr("<center><b> S c D r v </b></center><br>This example demonstrates how to write single document interface (<b>SDI</b>) applications using Qt and VTK<br><br>Qt version %1-%2 from https://wwww.qt.io<br>VTK version %3 from https://vtk.org/"));

  QMessageBox::about(this, tr("About the application"),
                     message.arg(QT_VERSION_STR).arg(QT_VERSION).arg(vtkVersion::GetVTKVersionFull()));
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionNewWindow__triggered
///
void FrameTop::on_actionNewWindow__triggered(void)
{
  FrameTop::createNew(this->saveSettings())->show();
}
//
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionNew__triggered
///
void FrameTop::on_actionNew__triggered(void)
{
  if (!this->queryDataSaved())
    return;
  frameDoc_->clearAll();
  frameDoc_->resetPath();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionOpen__triggered
///
void FrameTop::on_actionOpen__triggered(void)
{
  QFileDialog::Options opts_open = QFileDialog::QFileDialog::DontUseNativeDialog |
                                   QFileDialog::DontUseCustomDirectoryIcons;
  QString name_dir = QDir::currentPath();
  if (frameDoc_->hasPath())
  {
    QFileInfo info(tr(frameDoc_->getPath().c_str()));
    name_dir = info.path();
  }
  QString caption(tr("Open file(s)"));
  QString formats = FrameDoc::getReadFilter();
  QString fmt_use;
  QStringList to_open = QFileDialog::getOpenFileNames(this,
                                                      caption,
                                                      name_dir,
                                                      formats,
                                                      &fmt_use,
                                                      opts_open);
  if (!to_open.empty())
    this->addPathList(to_open);
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionSaveAs__triggered
///
void FrameTop::on_actionSave__triggered(void)
{
  assert(frameDoc_->hasPath());
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionSaveAs__triggered
///
void FrameTop::on_actionSaveAs__triggered(void)
{
  QFileDialog::Options opts_open = QFileDialog::QFileDialog::DontUseNativeDialog |
                                   QFileDialog::DontUseCustomDirectoryIcons;
  QString name_dir = QDir::currentPath();
  if (frameDoc_->hasPath())
  {
    QFileInfo finfo(tr(frameDoc_->getPath().c_str()));
    name_dir = finfo.path();
  }
  QString caption(tr("Save file as:"));
  // this is stub; it should deal with formats with two capabiblities:
  QString formats = FrameDoc::getExportFilter(); // both read and save
  QString fmt_use;
  QString to_save_as = QFileDialog::getSaveFileName(this,
                                                    caption,
                                                    name_dir,
                                                    formats,
                                                    &fmt_use,
                                                    opts_open);
  if (to_save_as.isEmpty())
    return;
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, caption, to_save_as);
#endif
  QFileInfo fi(to_save_as);
  Path path_to_save = fi.filesystemAbsoluteFilePath();
  FileFormat fmt = FrameDoc::FormatForSuffix(fi.suffix());
  if (fmt.isValidFormat() && fmt.hasSave() && fmt.hasRead() &&
      fmt.SaveTo(frameDoc_, path_to_save))
  {
    frameDoc_->resetPath(path_to_save);
    frameDoc_->resetFormat(fmt);
    frameDoc_->setModified(false);
  }
}
//
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionExport__triggered
///
void FrameTop::on_actionExport__triggered(void)
{
  QFileDialog::Options opts_open = QFileDialog::QFileDialog::DontUseNativeDialog |
                                   QFileDialog::DontUseCustomDirectoryIcons;
  QString name_dir = QDir::currentPath();
  if (frameDoc_->hasPath())
  {
    QFileInfo finfo(frameDoc_->getPath());
    name_dir = finfo.absoluteFilePath();
  }
  QString caption(tr("Export to file:"));
  QString formats = FrameDoc::getExportFilter();
  QString fmt_use;
  QString to_save_as = QFileDialog::getSaveFileName(this,
                                                    caption,
                                                    name_dir,
                                                    formats,
                                                    &fmt_use,
                                                    opts_open);
  if (to_save_as.isEmpty())
    return;
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, caption, to_save_as);
#endif
  QFileInfo fi(to_save_as);
  FileFormat fmt = FrameDoc::FormatForSuffix(fi.suffix());
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr(fmt.description()), tr(fmt.suffix()));
#endif
  if (fmt.isValidFormat() && fmt.hasSave())
    fmt.SaveTo(frameDoc_, fi.filesystemAbsoluteFilePath());
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionProperties__triggered
///
void FrameTop::on_actionProperties__triggered(void)
{
  if (!frameDoc_->hasPath() && frameDoc_->isModified())
  {
    return;
  } // no sense to characterize a nonexistant file..

  // ..but if the path is set...
  DialogFileProperties dlgProperties(frameDoc_->getPath(), this);

  int kRes = dlgProperties.exec();
  if (kRes == QDialog::Accepted)
  {
    // load newly changed data
    // this->updateUi();
  }
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionClearAll__triggered
///
void FrameTop::on_actionClearAll__triggered(void)
{
  if (!this->queryDataSaved())
    return;
  frameDoc_->clearAll(true); // set modefied ???
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionClone__triggered
///
void FrameTop::on_actionClone__triggered(void)
{
  workspace_->clearPathList();
  frameDoc_->setModified(true);
  frameDoc_->resetPath();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::on_actionClose__triggered
///
void FrameTop::on_actionClose__triggered(void)
{
  this->close();
}
//
///////////////////////////////////////////////////////////////////////
