#include "FrameTop.h"

#include <QDockWidget>
#include <QSettings>
#include <QByteArray>
#include <QMessageBox>

#include <vtkVersion.h>

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
    // now children:
    workspace_->readSettings(settings);
    files_->readSettings(settings);
    frameDoc_->readSettings(settings);
    return this;
}

FrameTop *FrameTop::saveSettings(void)
{
    QSettings settings;
    // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(keyGeometry(), this->saveGeometry());
    // now children:
    workspace_->saveSettings(settings);
    files_->saveSettings(settings);
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
    : QMainWindow(parent), workspace_(new ViewWorkspace(this)), files_(new ViewFileSystem(this))
{
    //
    this->setupUi(this);

    this->setUnifiedTitleAndToolBarOnMac(true); // Actually I don't know, what is it about

    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &FrameTop::commitData);
    //
    this->setupActions()
        ->setupToolBars()
        ->setupDockViews();
    //
    this->readSettings();
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
        query += frameDoc_->getPath();
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
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop::addPath
///
bool FrameTop::addPath(const fs::path &the_path)
{
    workspace_->addPathString(tr(the_path.c_str()));
    return true;
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameTop ::setupActions
///
FrameTop *FrameTop::setupActions(void)
{
    //! [implicit tr context]
    const QIcon iconNew = QIcon::fromTheme("window-new", QIcon(":/images/New.png"));
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
    tbText->addAction(actionTextSource_);
    tbText->addAction(actionSetSourceFont_);

    QToolBar *tbView = this->addToolBar(tr("View"));
    tbView->addAction(actionViewMolecule_);
    /*
        tbView->addSeparator();
        tbView->addWidget(nameBgColor_);
        connect(nameBgColor_, &ChooseNamedColor::currentTextChanged, this,
                &FrameBrowser::setBgColorByName);

        // editBgRed_->setValidator(vldColorComp_);
        // editBgRed_->setClearButtonEnabled(true);
        // editBgGreen_->setValidator(vldColorComp_);
        // editBgGreen_->setClearButtonEnabled(true);
        // editBgBlue_->setValidator(vldColorComp_);
        // editBgBlue_->setClearButtonEnabled(true);
        tbView->addSeparator();
        // --- [ ]
        tbView->addWidget(editBgRed_);
        connect(editBgRed_, &QLineEdit::editingFinished, this, &FrameBrowser::updateBackgroundRed);
        editBgRed_->setMaximumWidth(100);
        // --- [ ]
        tbView->addWidget(editBgGreen_);
        connect(editBgGreen_, &QLineEdit::editingFinished, this, &FrameBrowser::updateBackgroundGreen);
        editBgGreen_->setMaximumWidth(100);
        // --- [ ]
        tbView->addWidget(editBgBlue_);
        connect(editBgBlue_, &QLineEdit::editingFinished, this, &FrameBrowser::updateBackgroundBlue);
        editBgBlue_->setMaximumWidth(100);
        tbView->addSeparator();
    */
    tbView->addAction(actionProjectOrthogonal_);
    tbView->addAction(actionProjectPerspective_);
    tbView->addAction(actionProjectReset_);
    tbView->addSeparator();
    tbView->addAction(actionViewGeometry_);
    tbView->addAction(actionViewCoordinates_);

    // ---- [ ] ----
    QToolBar *tbMol = this->addToolBar(tr("Molecule"));
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
    this->addDockWidget(Qt::LeftDockWidgetArea, pLeft);
    QDockWidget *pNext = new QDockWidget(tr("Filesystem"), this);
    pNext->setWidget(files_);
    this->tabifyDockWidget(pLeft, pNext);
    ///
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
/// \brief FrameTop::on_actionClose__triggered
///
void FrameTop::on_actionNew__triggered(void)
{
    FrameTop::createNew(this->saveSettings())->show();
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
