#ifndef Frame_File__H_
#define Frame_File__H_

#ifdef _MSC_VER
#  pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkMolecule.h>
#include <vtkMoleculeAlgorithm.h>
#include <vtkMoleculeReaderBase.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkVectorFieldTopology.h>

#include <QPointer>
#include <QSplitter>
#include <QTabWidget>

#include "EditSource.h"
#include "FormatFor.h"
#include "ReadMoleculeFileBase.h"
#include "StyleMapMolecule.h"
#include "ViewMolecule.h"
#include "ViewQuantities.h"

typedef vtkNew<vtkMolecule> NewMolecule;
typedef vtkSmartPointer<vtkMolecule> AMolecule;

typedef vtkNew<vtkMoleculeAlgorithm> NewMolAlgorithm;
typedef vtkSmartPointer<vtkMoleculeAlgorithm> AMolAlgorithm;

typedef vtkNew<vtkVectorFieldTopology> NewFieldTopology;
typedef vtkSmartPointer<vtkVectorFieldTopology> AFieldTopology;

using namespace std;
using namespace vtk;

class FrameFile;

typedef FileFormatFor<FrameFile,bool> InputFileFormat;

class FrameFile : public QTabWidget
{
  Q_OBJECT

private:
  static const InputFileFormat allInputFormats[];

public:
  static InputFileFormat chooseFormatByExtension(const QString& exten);
  static QString filterInput();
  static QString filterOutput();

  static QString QuerySavePath(QWidget* parent, QString src,
                               QString& /* context */);

public:
  explicit FrameFile(QWidget* /*parent*/ = nullptr);

  bool isUntitled() const;
  bool hasSourcePath() const;
  const QString& getPathSource() const;

  EditCode* editSource();
  EditCode* getEditSource() const;
  QTextDocument* getSourceDocument() const;

  ViewMolecule* viewMolecule();
  ViewMolecule* getViewMolecule() const;

  QString describeInputFormats() const;

  bool changePathToBind(bool bSync = false);

  bool querySave();
  void doSave();
  bool isModified() const;
  bool hasPendingTasks() const;
  bool cannotBeClosed() const;

  bool openTextFile(const QString& /* path */, bool /* bExistent */ = true);

  bool ParseXYZ();
  bool ParsePDB();
  bool ParseCUBE();
  bool ParseEXTOUT();
  bool ParseMGP();
  bool ParseSUM();

  // signals:

protected:
  bool loadSource(const QString& /* path */, bool /* bExistent */ = true);
  bool castSource();

  void ShowTheStructure();

  template <class Reader>
  bool ReadMoleculeAs();

private:
  bool wasCastSource(); // storage stub; unknown mind writings...
  // std::filesystem::path path_bound_;
  QString pathSource_ = QString();
  //
  // AMoleculeField molecule_;
  //
  // QPointer<FormStructureText> source_;
  // QPointer<FormStructureView> view_;

  // StyleMapMolecule styleMapMol_;

  // QByteArray state_;
  // QPointer<QSplitter> split_;
  // QPointer<ViewStructure> view_str_;
  // int id_view_str_;
  //
  QPointer<EditSource> edit_src_;
  int id_edit_src_ = -1;
  QPointer<ViewMolecule> view_mol_;
  int id_view_mol_ = -1;
  QPointer<ViewQuantities> view_val_;
  int id_view_val_ = -1;

  AMolecule molecule_;
  AFieldTopology field_;

  AMolAlgorithm make_bonds_;

  // vtkSmartPointer<vtk::ReadMoleculeFileBase> ptrReader_;
  InputFileFormat fmt_input_;

  QWidget* ptrActiveWidget_ = nullptr;

private slots:
  void castChangedViews(int id);
};

#endif // Frame_File__H_
