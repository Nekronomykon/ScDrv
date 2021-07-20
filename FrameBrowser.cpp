#include "FrameBrowser.h"

#include <vtkVersion.h>

#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDialog>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QString>
#include <QVariant>

#include "DialogFileProperties.h"
#include "TableElements.h"

static inline QString keyRecentFiles()
{
  return QStringLiteral("FileRecent");
}
static inline QString keyBackColor()
{
  return QStringLiteral("BackgroundColor");
}
static inline QString keyGeometry()
{
  return QStringLiteral("Geometry");
}
static inline QString keyFile()
{
  return QStringLiteral("File");
}

FrameBrowser* FrameBrowser::provideBrowserFor(const QString& what)
{
  FrameBrowser* pOpen = nullptr;
  /*
   * pOpen = FrameBrowser::findBrowserFor(what);
   * if(pOpen)
   * {
   *   pOpen->show/activate/browseTo(what);
   * }
   * else
   */
  {
    pOpen = FrameBrowser::createNewBrowser();
    /*
     * pOpen->openFile(what);
     */
  }
  return pOpen;
}

FrameBrowser::FrameBrowser(QWidget* parent)
  : QMainWindow(parent)
  , choose_color_(new ChooseColor(this))
  , files_(new ViewFilesystem(this))
  , wkspace_(new ViewWorkspace(this))
  , messages_(new ViewMessages(this))
{
  this->setupUi(this);

  this->setupActions();
  this->setupToolBars();
  this->setupDockViews();

  this->readSettings();

  connect(files_->getFileSelector(), &QListView::activated, this,
          &FrameBrowser::loadIndexedFile);
  this->updateUi();
}
//
void FrameBrowser::tile(const QMainWindow* previous)
{
  if (!previous)
    return;
  int topFrameWidth = previous->geometry().top() - previous->pos().y();
  if (!topFrameWidth)
    topFrameWidth = 40;
  const QPoint pos =
    previous->pos() + 2 * QPoint(topFrameWidth, topFrameWidth);
  if (screen()->availableGeometry().contains(rect().bottomRight() + pos))
    move(pos);
}
//
void FrameBrowser::readSettings()
{
  QSettings settings; // (QCoreApplication::organizationName(),
  // QCoreApplication::applicationName());
  QString nameBgColor = settings.value(keyBackColor(), QString()).toString();
  if (nameBgColor.isEmpty())
    nameBgColor = tr("black"); // hard-coded
  ChooseColor::resetDefaultColorName(nameBgColor);

  const QByteArray geometry =
    settings.value(keyGeometry(), QByteArray()).toByteArray();
  if (geometry.isEmpty()) {
    const QRect availableGeometry = this->screen()->availableGeometry();
    resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
    move((availableGeometry.width() - width()) / 2,
         (availableGeometry.height() - height()) / 2);
  } else {
    this->restoreGeometry(geometry);
  }
}
//
void FrameBrowser::writeSettings()

{
  QSettings settings; // (QCoreApplication::organizationName(),
  // QCoreApplication::applicationName());
  settings.setValue(keyBackColor(), ChooseColor::getDefaultColorName());
  settings.setValue(keyGeometry(), this->saveGeometry());
}
//
void FrameBrowser::setupActions()
{
  //! [implicit tr context]
  const QIcon iconNew =
    QIcon::fromTheme("window-new", QIcon(":/images/New.png"));
  actionNew_->setIcon(iconNew);
  actionNew_->setShortcuts(QKeySequence::New);
  actionNew_->setStatusTip(tr("Create a new file"));

  const QIcon iconOpen =
    QIcon::fromTheme("document-open", QIcon(":/images/Open.png"));
  actionOpen_->setIcon(iconOpen);
  actionOpen_->setShortcuts(QKeySequence::Open);
  actionOpen_->setStatusTip(tr("Open an found file"));

  const QIcon iconReload =
    QIcon::fromTheme("document-revert", QIcon(":/images/Reload.png"));
  actionReload_->setIcon(iconReload);
  actionReload_->setShortcuts(QKeySequence::Refresh);
  actionReload_->setStatusTip(tr("Reload content from file"));

  const QIcon iconSave =
    QIcon::fromTheme("document-save", QIcon(":/images/Save.png"));
  actionSave_->setIcon(iconSave);
  actionSave_->setShortcuts(QKeySequence::Save);
  actionSave_->setStatusTip(tr("Save the document to disk"));

  const QIcon iconSaveAs =
    QIcon::fromTheme("document-save-as", QIcon(":/images/SaveAs.png"));
  actionSaveAs_->setIcon(iconSaveAs);
  actionSaveAs_->setShortcuts(QKeySequence::SaveAs);
  actionSaveAs_->setStatusTip(tr("Save the document under a new name"));

  const QIcon iconClose =
    QIcon::fromTheme("window-close", QIcon(":/images/CloseFrame.png"));
  actionClose_->setIcon(iconClose);
  actionClose_->setShortcut(QKeySequence::Close);

  const QIcon iconExit =
    QIcon::fromTheme("application-exit", QIcon(":/images/Exit.png"));
  actionExit_->setIcon(iconExit);
  actionExit_->setShortcuts(QKeySequence::Quit);

  const QIcon iconUndo =
    QIcon::fromTheme("edit-undo", QIcon(":/images/Undo.png"));
  actionUndo_->setIcon(iconUndo);
  actionUndo_->setShortcut(QKeySequence::Undo);

  const QIcon iconRedo =
    QIcon::fromTheme("edit-redo", QIcon(":/images/Redo.png"));
  actionRedo_->setIcon(iconRedo);
  actionRedo_->setShortcut(QKeySequence::Redo);

  const QIcon iconCut =
    QIcon::fromTheme("edit-cut", QIcon(":/images/Cut.png"));
  actionCut_->setIcon(iconCut);
  actionCut_->setShortcuts(QKeySequence::Cut);

  const QIcon iconCopy =
    QIcon::fromTheme("edit-copy", QIcon(":/images/Copy.png"));
  actionCopy_->setIcon(iconCopy);
  actionCopy_->setShortcuts(QKeySequence::Copy);

  const QIcon iconPaste =
    QIcon::fromTheme("edit-paste", QIcon(":/images/Paste.png"));
  actionPaste_->setIcon(iconPaste);
  actionPaste_->setShortcuts(QKeySequence::Paste);

  const QIcon iconClear =
    QIcon::fromTheme("edit-clear", QIcon(":/images/Clear.png"));
  actionClear_->setIcon(iconClear);
  // actionClear_->setShortcut(QKeySequence::Clear);

  const QIcon iconClearAll =
    QIcon::fromTheme("edit-clear-all", QIcon(":/images/ClearAll.png"));
  actionClearAll_->setIcon(iconClearAll);

  const QIcon iconProps =
    QIcon::fromTheme("document-properties", QIcon(":/images/Options.png"));
  actionProperties_->setIcon(iconProps);

  const QIcon iconEditTxt(":/images/SrcEdit.png");
  actionTextSource_->setIcon(iconEditTxt);
  frameDoc_->setTabIcon(0, iconEditTxt);

  const QIcon iconSetFont = QIcon::fromTheme("preferences-desktop-font"
                                             // , QIcon(":/images/SetFont.png")
  );
  actionSetFont_->setIcon(iconSetFont);

  const QIcon iconOptions = QIcon::fromTheme("preferences-system"
                                             // , QIcon(":/images/Options.png")
  );
  actionOptions_->setIcon(iconOptions);

  const QIcon iconCastSrc(":/images/SrcCast.png");
  actionViewMolecule_->setIcon(iconCastSrc);
  frameDoc_->setTabIcon(1, iconCastSrc);

  const QIcon iconFast(":/images/MolFast.png");
  actionMoleculeFast_->setIcon(iconFast);

  const QIcon iconBalls(":/images/MolBalls.png");
  actionMoleculeAsBallsSticks_->setIcon(iconBalls);

  const QIcon iconStick(":/images/MolStick.png");
  actionMoleculeAsSticks_->setIcon(iconStick);

  const QIcon iconVDW(":/images/MolVDW.png");
  actionMoleculeAsCPK_->setIcon(iconVDW);

  const QIcon iconAbout =
    QIcon::fromTheme("help-about", QIcon(":/images/Help.png"));
  actionAbout_->setIcon(iconAbout);

  const QIcon iconAboutQt =
    QIcon::fromTheme("system-help", QIcon(":/images/AboutKit.png"));
  actionAboutQt_->setIcon(iconAboutQt);
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameBrowser::setupToolBars()
{
  barTools_->addAction(actionNew_);
  barTools_->addAction(actionOpen_);
  barTools_->addAction(actionSave_);

  QToolBar* tbEdit = this->addToolBar(tr("Edit"));
  tbEdit->addAction(actionUndo_);
  tbEdit->addAction(actionRedo_);
  tbEdit->addSeparator();
  tbEdit->addAction(actionCut_);
  tbEdit->addAction(actionCopy_);
  tbEdit->addAction(actionPaste_);
  tbEdit->addSeparator();
  tbEdit->addAction(actionClear_);
  tbEdit->addAction(actionClearAll_);

  QToolBar* tbText = this->addToolBar(tr("Text"));
  tbText->addAction(actionTextSource_);
  tbText->addAction(actionSetFont_);

  QToolBar* tbView = this->addToolBar(tr("View"));
  tbView->addAction(actionViewMolecule_);
  tbView->addSeparator();
  tbView->addWidget(choose_color_);
  connect(choose_color_, &ChooseColor::currentTextChanged, this,
          &FrameBrowser::setSceneBgColor);
  tbView->addAction(actionMoleculeFast_);
  tbView->addSeparator();
  tbView->addAction(actionMoleculeAsSticks_);
  tbView->addAction(actionMoleculeAsBallsSticks_);
  tbView->addAction(actionMoleculeAsCPK_);

  QToolBar* tbHelp = this->addToolBar(tr("Help"));
  tbHelp->addAction(actionProperties_);
  tbHelp->addAction(actionOptions_);
  tbHelp->addAction(actionAbout_);
  tbHelp->addAction(actionAboutQt_);
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameBrowser::setupDockViews()
{
  QDockWidget* pDock(new QDockWidget(tr("File system"), this));
  pDock->setWidget(files_);
  this->addDockWidget(Qt::LeftDockWidgetArea, pDock);
  QDockWidget* pNext(new QDockWidget(tr("Workspace"), this));
  pNext->setWidget(wkspace_);
  this->tabifyDockWidget(pDock,pNext);
  QDockWidget* pDown(new QDockWidget(tr("Log"), this));
  pDown->setWidget(messages_);
  this->addDockWidget(Qt::BottomDockWidgetArea,pDown);
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameBrowser::updateUi()
{
  bool bHavePath = !frameDoc_->isUntitled();
  actionSave_->setEnabled(bHavePath);
  actionClone_->setEnabled(bHavePath);
  actionReload_->setEnabled(bHavePath);

  QWidget* pActive = frameDoc_->currentWidget();

  EditCode* pEdit = frameDoc_->getEditSource();
  actionTextSource_->setChecked(bool(pActive == (QWidget*)pEdit));

  ViewMolecule* pView = frameDoc_->getViewMolecule();
  actionViewMolecule_->setChecked(bool(pActive == (QWidget*)pView));
}

void FrameBrowser::navigateTo(const QString& path)
{
  QFileInfo fi(path);
  frameDoc_->openTextFile(fi.canonicalFilePath(), fi.exists());
  files_->showPath(path);

  QString ex_type(fi.suffix());
#ifdef QT_DEBUG_MESSAGE_BOX
  QMessageBox::information(this, tr("Extension"), ex_type, QMessageBox::Ok);
#endif
  // the simplest form of the format/by-extension casting ;)
  if (!ex_type.compare(tr("xyz")))
    frameDoc_->ParseXYZ();
  if (!ex_type.compare(tr("pdb")))
    frameDoc_->ParsePDB();
  if (!ex_type.compare(tr("cube")))
    frameDoc_->ParseCUBE();
}

void FrameBrowser::closeEvent(QCloseEvent* event)
{
  this->writeSettings();
  if (frameDoc_->querySave())
    event->accept();
  else
    event->ignore();
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameBrowser::setSceneBgColor()
{
  QString new_name_bg = choose_color_->currentText();
  ChooseColor::resetDefaultColorName(new_name_bg);
  ViewMolecule* pView = frameDoc_->viewMolecule();
  pView->adjustBgColor();
  // ??? choose_color_->adjustColorName();
}

void FrameBrowser::loadIndexedFile(const QModelIndex& idx)
{
  // QString path(idx.data());
  QFileInfo fi(files_->fileInfo(idx));
#ifdef QMESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Selected path:"), fi.canonicalFilePath());
#endif
  if (fi.isFile() && fi.isReadable())
    this->navigateTo(fi.canonicalFilePath());
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::on_actionClose__triggered
///
void FrameBrowser::on_actionClose__triggered()
{
  this->close();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::on_actionExit__triggered
///
void FrameBrowser::on_actionExit__triggered()
{
  qApp->closeAllWindows();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::on_actionAboutQt__triggered
///
void FrameBrowser::on_actionAboutQt__triggered()
{
  qApp->aboutQt();
  // finally
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionNew__triggered
///
void FrameBrowser::on_actionNew__triggered()
{
  FrameBrowser* pNew = FrameBrowser::createNewBrowser();
  pNew->tile(this);
  pNew->show();
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionClone__triggered
///
void FrameBrowser::on_actionClone__triggered()
{
  // frameDoc_->resetSourcePath();
  // frameDoc_->setModified(true);
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionOpen__triggered
///
void FrameBrowser::on_actionOpen__triggered()
{
  QString formats = frameDoc_->describeInputFormats();
  QString fmt_select;
  QFileDialog::Options options_open =
    QFileDialog::DontUseCustomDirectoryIcons |
    QFileDialog::DontUseNativeDialog;
  // QStringList paths_to_open =
  QString path_to_open = QFileDialog::getOpenFileName(
    this, tr("Open files"), frameDoc_->getPathSource(), formats, &fmt_select,
    options_open);

  if (path_to_open.isEmpty())
    return;

  this->navigateTo(path_to_open);
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionElements__triggered
///
void FrameBrowser::on_actionElements__triggered()
{
  QPointer<TableElements> table(new TableElements);
  table->show();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionMoleculeFast__triggered
///
void FrameBrowser::on_actionMoleculeFast__triggered()
{
  if (frameDoc_->viewMolecule()->setMoleculeStyle(StyleMapMolecule::styleFast))
    ;
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionMoleculeAsSticks__triggered
///
void FrameBrowser::on_actionMoleculeAsSticks__triggered()
{
  if (frameDoc_->viewMolecule()->setMoleculeStyle(StyleMapMolecule::styleStyx))
    ;
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionMoleculeAsBallsSticks__triggered
///
void FrameBrowser::on_actionMoleculeAsBallsSticks__triggered()
{
  if (frameDoc_->viewMolecule()->setMoleculeStyle(StyleMapMolecule::styleBnS))
    ;
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionMoleculeAsCPK__triggered
///
void FrameBrowser::on_actionMoleculeAsCPK__triggered()
{
  if (frameDoc_->viewMolecule()->setMoleculeStyle(StyleMapMolecule::styleCPK))
    ;
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionSave__triggered
///
void FrameBrowser::on_actionSave__triggered()
{
  assert(!frameDoc_->isUntitled());
  frameDoc_->doSave();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionSaveAs__triggered
///
void FrameBrowser::on_actionSaveAs__triggered()
{
  QString name_format;
  QString path_to_set =
    FrameFile::QuerySavePath(this, frameDoc_->getPathSource(), name_format);
  if (!path_to_set.isEmpty()) {
  }
  this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionSetFont__triggered
///
void FrameBrowser::on_actionSetFont__triggered()
{
  auto pEdit = frameDoc_->editSource();
  pEdit->setReadOnly(true);

  bool bOk;
  QFont font_edit = QFontDialog::getFont(&bOk, pEdit->font(), this, QString(),
                                         QFontDialog::MonospacedFonts);
  if (bOk)
    pEdit->setFont(font_edit);
  this->updateUi();
}
//
//////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::on_actionProperties__triggered
///
void FrameBrowser::on_actionProperties__triggered()
{
  assert(!frameDoc_->isUntitled());
  DialogFileProperties dlgProperties(frameDoc_->getPathSource(), this);
  // dlgProperties.setModal(true);
  int kRes = dlgProperties.exec();

  // finally
  this->updateUi();
}
//
//////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::on_actionTextSource__triggered
///
void FrameBrowser::on_actionTextSource__triggered()
{
  EditCode* pEdit = frameDoc_->editSource();

  // finally
  this->updateUi();
}
//
//////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::on_actionViewMolecule__triggered
///
void FrameBrowser::on_actionViewMolecule__triggered()
{
  ViewMolecule* pView = frameDoc_->viewMolecule();

  // finally
  this->updateUi();
}
//
//////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionAbout__triggered
///
void FrameBrowser::on_actionAbout__triggered()
{
  QString message(
    tr("<center><b> S c D r v </b></center><br>This example demonstrates how "
       "to write single document interface (<b>SDI</b>) applications using Qt "
       "and VTK<br><br>Qt version %1-%2 from https://wwww.qt.io<br>VTK "
       "version %3 from https://vtk.org/"));

  QMessageBox::about(this, tr("About"),
                     message.arg(QT_VERSION_STR)
                       .arg(QT_VERSION)
                       .arg(vtkVersion::GetVTKVersionFull()));
  // finally
  this->updateUi();
}
