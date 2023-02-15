#ifndef View_Molecule_h__
#define View_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QVTKOpenGLNativeWidget.h>

#include <vtkColor.h>
#include <vtkRenderer.h>
#include <vtkImageWriter.h>
#include <vtkMoleculeMapper.h>

#include <vtkRenderWindowInteractor.h>

#include <vtkActor.h>
#include <vtkLODActor.h>

#include "Molecule.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<vtkActor> ANewActor;
typedef vtkSmartPointer<vtkActor> AnActor;

typedef vtkNew<vtkRenderer> ANewRenderer;
typedef vtkSmartPointer<vtkRenderer> ARenderer;

typedef vtkNew<vtkMoleculeMapper> ANewMolMapper;
typedef vtkSmartPointer<vtkMoleculeMapper> AMolMapper;

typedef vtkNew<vtkRenderWindowInteractor> ANewRenderWindowInteractor;

class ViewMolecule
    : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit ViewMolecule(QWidget * /*parent*/ = nullptr);
    ~ViewMolecule() override = default;

    vtkColor3d getBackgroundColor() const;
    vtkColor3d &backgroundColor();

    vtkMoleculeMapper *getMoleculeMapper(void) const;
    vtkRenderer *getMoleculeRenderer(void) const;

    void updateBackground(vtkRenderWindow * /*pRW*/ = nullptr);

    bool exportImageTo(vtkImageWriter * /*pIW*/, bool /*bAlpha*/ = true);

    ViewMolecule* setProjectParallel(bool /* bResetCameraIfAlreadySetSo */ = false);
    ViewMolecule* setProjectPerspective(bool /* bResetCameraIfAlreadySetSo */ = false);

    ViewMolecule* resetMolecule(vtk::Molecule * /*pMol*/ = nullptr);

private:
    bool isImageBackTransparent_ = true;
    ANewRenderer renderBg_;
    vtkColor3d colorBg_;

    ANewActor actorMol_;
    ANewRenderer renderMol_;
    ANewMolMapper mapMol_;
};

#endif //! View_Molecule_h__
