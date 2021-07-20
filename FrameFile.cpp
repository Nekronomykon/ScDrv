#include "FrameFile.h"

#include <sstream>
#include <string>
#include <vector>

#include <vtkCMLMoleculeReader.h>
#include <vtkGaussianCubeReader.h>
#include <vtkGaussianCubeReader2.h>
#include <vtkPDBReader.h>
#include <vtkSimpleBondPerceiver.h>
#include <vtkXYZMolReader.h>
#include <vtkXYZMolReader2.h>

#include <QByteArray>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include "ReadMoleculeFileMGP.h"
#include "ReadMoleculeFileXYZ.h"

using namespace std;
using namespace vtk;
// namespace fs = std::filesystem;

typedef vtkNew<vtkSimpleBondPerceiver> NewSimpleBondMaker;
typedef vtkSmartPointer<vtkSimpleBondPerceiver> ASimpleBondMaker;

/* static */ const InputFileFormat FrameFile::allInputFormats[] = {
  { QString("AIMAll exteded output"), QString("extout"),
    &FrameFile::ParseEXTOUT },
  { QString("AIMAll molecular graph"), QString("mgp"), &FrameFile::ParseMGP },
  { QString("AIMAll summary"), QString("sum"), &FrameFile::ParseSUM },
  // What else from the AIMAll output formats?
  // --> http://aim.tkgristmill.com/readme.html
  { QString("XMol XYZ"), QString("xyz"), &FrameFile::ParseXYZ },
  { QString("Brookhaven Protein DB"), QString("pdb"), &FrameFile::ParsePDB },
  { QString("Gaussian formatted checkpoint"), QString("fchk") },
  { QString("Gaussian cube"), QString("cube"), &FrameFile::ParseCUBE },
  { QString("Chemical MarkUp CML"), QString("cml") }, /// ???
  { QString("Wavefinction"), QString("wfn") },
  { QString("Wavefinction extended"), QString("wfx") },
  // { QString(), QString() }
};

/* static */ InputFileFormat FrameFile::chooseFormatByExtension(
  const QString& exten)
{
  InputFileFormat fmt_res;
  for (const InputFileFormat& fmt : allInputFormats) {
    if (!fmt.hasExtension(exten))
      continue;
    fmt_res = fmt;
    break;
  }
  return fmt_res;
}

/* static */ QString FrameFile::filterInput()
{
  QString avlbl(tr("All available read formats ("));
  QString res;
  QString all = tr("All text files (*.*)");
  /*
  ::assumed that any format has the only file extension,
    associated with file with the format mentioned
    ::
    */
  set<QString> allEx;
  for (const InputFileFormat& fmt : allInputFormats) {
    // if(!fmt.hasAction()) --> only the text file:
    //  continue;
    if (!fmt.hasName() ||
        !fmt.hasFileExtension()) // --> nameless formats are skipped:
      continue;
    QString xtnsn =
      fmt.extension(); // <-- here to care whether there are moar extensions
    allEx.insert(xtnsn);
    QString fmt_desc(tr("%1 file(s) (*.%2);;").arg(fmt.name()).arg(xtnsn));
    res += fmt_desc;
  }

  // Make "All registered" section:
  QString reged;
  if (!allEx.empty()) {
    for (const QString& xnosch : allEx)
      if (!xnosch.isEmpty())
        avlbl += tr(" *.") += xnosch;
  }
  avlbl += ");;";
  avlbl += res;
  avlbl += all;
  return avlbl;
}

/* static */ QString FrameFile::filterOutput()
{
  QString avlbl(tr("All available read formats ("));
  QString res;
  QString all = tr("All text files (*.*)");
  /*
  ::assumed that any format has the only file extension,
    associated with file with the format mentioned
    ::
  set<QString> allEx;
  foreach(fmt : all.available.formats())
  {
    if(!fmt->writable())
        continue;
    QString xtnsn = fmt->usedFileExtension();
    allEx.add(xtsn);
    res += rdr->FormatName() += tr(" (*.") += xtnsn += tr(");;")
  }
  if(!allEx.isEmpty() )
  {
    res = avlbl;
    foreaach(xnosch : allEx)
      res += tr("*.") += xnosch;
      res += tr(")")
  }
  */
  res += all;
  return res;
}

/* static */ QString FrameFile::QuerySavePath(QWidget* parent, QString src,
                                              QString& context)
{
  QFileDialog::Options opts = QFileDialog::DontUseNativeDialog |
    QFileDialog::DontUseCustomDirectoryIcons;
  QString fmt = FrameFile::filterOutput();
  QString fmt_context;
  QString path = QFileDialog::getSaveFileName(parent, tr("Path to save file"),
                                              src, fmt, &fmt_context, opts);
  context = fmt_context;
  return path;
}

FrameFile::FrameFile(QWidget* parent)
  : QTabWidget(parent)
  , edit_src_(new EditSource(this))
  , view_mol_(new ViewMolecule(this))
  , view_val_(new ViewQuantities(this))
  , molecule_(AMolecule::New())
  , make_bonds_(ASimpleBondMaker::New())
// , source_(new FormStructureText(this))
// , view_(new FormStructureView(this))
// , split_(new QSplitter(this))
{
  // Features of the tab bar::
  this->setTabBarAutoHide(true);
  this->setTabShape(QTabWidget::Rounded);
  this->setTabPosition(QTabWidget::South);
  this->setDocumentMode(true);

  // this->addTab(view_, tr("Structure"));
  // this->addTab(source_, tr("Text parse"));
  id_edit_src_ = this->addTab(edit_src_, tr("Text source"));
  id_view_mol_ = this->addTab(view_mol_, tr("Molecule"));
  id_view_val_ = this->addTab(view_val_, tr("Structure"));

  connect(this, &FrameFile::currentChanged, this,
          &FrameFile::castChangedViews);
  // connect(this, &FrameFile::currentChanging, this,
  // &FrameFile::castChangedViews);
}

bool FrameFile::isUntitled() const
{
  return pathSource_.isEmpty();
}

bool FrameFile::hasSourcePath() const
{
  return !pathSource_.isEmpty();
}

const QString& FrameFile::getPathSource() const
{
  return pathSource_;
}

EditCode* FrameFile::editSource()
{
  if (edit_src_)
    this->setCurrentWidget(edit_src_);
  return edit_src_;
}

EditCode* FrameFile::getEditSource() const
{
  return edit_src_;
}

QTextDocument* FrameFile::getSourceDocument() const
{
  return edit_src_->document();
}

ViewMolecule* FrameFile::viewMolecule()
{
  if (view_mol_)
    this->setCurrentWidget(view_mol_);
  return view_mol_;
}

ViewMolecule* FrameFile::getViewMolecule() const
{
  return view_mol_;
}

QString FrameFile::describeInputFormats() const
{
  QString fmt = FrameFile::filterInput();
  return fmt;
}

bool FrameFile::changePathToBind(bool bSync)
{
  QString format;
  QString path = FrameFile::QuerySavePath(this, this->getPathSource(), format);
  if (path.isEmpty())
    return false; // cancelled
  // setup format:
  if (edit_src_->loadFromPath(path)) {
    pathSource_ = path;
    // apply format, if bSync
    return bSync ? this->castSource() : true; // TODO: rewrite
  } else                                      // something went wrong...
    return false;                             // stub
}

bool FrameFile::querySave()
{
  if (!this->isModified())
    return true;
  // bool bRes = true;

  QString query(tr("Current content of the document"));

  QString path(this->getPathSource());
  if (!path.isEmpty()) {
    query += "based upon the path\n";
    query += path;
  }
  query += "\nis modified. Do you want to save the changes?";
  int reply = QMessageBox::question(
    this, tr("Modified"), query,
    // QMessageBox::YesToAll | QMessageBox::NoToAll |
    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
    QMessageBox::Cancel);
  if (reply == QMessageBox::Cancel)
    return false;
  // ????????
  if (reply == QMessageBox::Yes || reply == QMessageBox::YesToAll) {
    if (!path.isEmpty()) {
      this->doSave(); // silent update
      return true;
    } else
      return !this->changePathToBind(); // ???????? TO RETHINK!
  }
  // ???? HERE TO CHECK CAREFULLY!!!!
  return true;
}
void FrameFile::doSave()
{
  edit_src_->saveToPath(this->getPathSource());
  this->getSourceDocument()->setModified(false);
}
bool FrameFile::isModified() const
{
  return this->getSourceDocument()->isModified() ? true
                                                 // : view_mol_->isModified();
                                                 : false;
}

bool FrameFile::hasPendingTasks() const
{
  return false; // stub;
  // actually there's no yet even a concept of a "side/parallel task"...
}

bool FrameFile::cannotBeClosed() const
{
  return this->isModified() ? true : this->hasPendingTasks();
}

bool FrameFile::openTextFile(const QString& path, bool bExistent)
{
  this->loadSource(path, bExistent);
  return this->castSource(); // NEVERTHELESS STILL A STUB!
}

bool FrameFile::loadSource(const QString& path, bool bExistent)
{
  this->pathSource_ = path;

  auto edit = this->editSource();
  if (!edit->loadFromPath(path))
    return false;
  edit->document()->setModified(false);
  edit->setReadOnly(!bExistent);

  return true;
}
bool FrameFile::ParseXYZ()
{
  return this->ReadMoleculeAs<vtk::ReadMoleculeFileXYZ>();
}
bool FrameFile::ParsePDB()
{
  return this->ReadMoleculeAs<vtkPDBReader>();
}
bool FrameFile::ParseCUBE()
{
  return this->ReadMoleculeAs<vtkGaussianCubeReader2>();
}
bool FrameFile::ParseEXTOUT()
{
  return
    // this->ReadMoleculeAs<vtk::ReadMoleculeFileEXTOUT>();
    false;
}
bool FrameFile::ParseSUM()
{
  return
    // this->ReadMoleculeAs<vtk::ReadMoleculeFileSUM>();
    false;
}
bool FrameFile::ParseMGP()
{
  return
    // this->ReadMoleculeAs<vtk::ReadMoleculeFileMGP>();
    false;
}

//////////////////////////////////////////////////////////////////////////
//
bool FrameFile::castSource()
{
  // NewMoleculeField mol_new;
  // this->setViewOnlySource();
  auto edit = this->getEditSource();
  edit->setReadOnly(true);

  QString path = this->getPathSource();
  QFileInfo fi(path);
  InputFileFormat fmt = FrameFile::chooseFormatByExtension(fi.suffix());
  if (!fmt)
    return true; // --> "text-only" mode is the only possible alternative:
  if (!fmt.applyFor(*this))
    return false; // --> unsuccesful attempt to apply known format...
  // this->setViewCastStructure();
  return true; // ->
}
bool FrameFile::wasCastSource()
{
  // NewMoleculeField mol_new;
  auto edit = this->getEditSource();
  edit->setReadOnly(true);

  QStringList allText =
    edit_src_->toPlainText().split('\n'); // work it out with "\r\n", etc

  int idAtom = 0;
  int idString = 0;

  do {
    QString strOne = allText.at(idString);
    int kComment = strOne.indexOf('#');
    if (kComment >= 0)
      strOne = strOne.left(kComment);
    strOne = strOne.trimmed();
    if (strOne.isEmpty())
      continue;

    QByteArray chars = strOne.toLatin1();
    std::istringstream inp(chars.data());
    if (!inp) // possibly a useless check...
    {
      QMessageBox::information(this, tr("Invalid stream from buffer"), strOne);
      break; // possibly never from here...
    }

    // cutting

    vector<string> parsed;
    string one_string;
    inp >> one_string;
    do {
      parsed.push_back(one_string);
      one_string.resize(0);
      inp >> one_string;
    } while (!one_string.empty());
    // if(parsed.size() <= 1)
    // vtkAtom newAtom = mol_new->AppendAtom(); // newly appended...

  } while (++idString < allText.size());

  // if (!mol_new->GetNumberOfAtoms())
  // return false;

  // molecule_->DeepCopy(mol_new);
  // wipe or rebuild all the structure stuff...

  return true; //
}
void FrameFile::ShowTheStructure()
{
  // view_str_->ShowMolecule(molecule_, styleMapMol_);
}

void FrameFile::castChangedViews(int id)
{
  if (edit_src_ == ptrActiveWidget_) // --> deactivating edit_src_
  {
    if (this->castSource())
      this->ShowTheStructure();
  }
  if ((id_edit_src_ >= 0) && (id == id_edit_src_)) // --> activating edit_src_
  {
    ;
  }
  // [*]: update the active widget info to contain actual data
  ptrActiveWidget_ = this->currentWidget();
}

template <class Reader>
bool FrameFile::ReadMoleculeAs()
{
  QString path = this->getPathSource();
  // --- what we really need here
  // --- to avoid the real pathSource dependence:
  // QString path = edit_src_->writeTemporaryContent();
  QFileInfo fi(path);
  QByteArray bytes = fi.canonicalFilePath().toLatin1();

  NewMolecule new_mol;
  vtkSmartPointer<Reader> reader(vtkSmartPointer<Reader>::New());
  // vtkNew<Reader> reader;

  reader->SetOutput(new_mol);
  reader->SetFileName(bytes.data());
  reader->Update();
  bool bResult(new_mol->GetNumberOfAtoms() > 0);

#ifndef QT_MESSAGE_BOX_INFO_DEBUG
QString message( tr("Reading molecule from:\nPath: %1\nNumber of atoms read: %2\n") );
  QMessageBox::information( this, QString("Reading a molecule :1"),
  message.arg(path).arg(new_mol->GetNumberOfAtoms()) );
#endif //! QT_MESSAGE_BOX_INFO_DEBUG

  // make_bonds_->SetInputData(new_mol);
  // make_bonds_->SetOutput(molecule_);
  // make_bonds_->E??

  // if (bResult) molecule_->DeepCopy(new_mol);
  return bResult;
}
