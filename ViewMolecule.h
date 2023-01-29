#ifndef View_Molecule_h__
#define View_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QVTKOpenGLNativeWidget.h>

#include <vtkColor.h>
#include <vtkRenderer.h>
#include <vtkMoleculeMapper.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<vtkRenderer> ANewRenderer;
typedef vtkSmartPointer<vtkRenderer> ARenderer;

class ViewMolecule
    : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit ViewMolecule(QWidget * /*parent*/ = nullptr);
    ~ViewMolecule() override = default;

    vtkColor3d getBackgroundColor() const;
    vtkColor3d& backgroundColor();

    void updateBackgroundColor();

  private:
    vtkColor3d colorBg_;
    ANewRenderer renderBg_;
};

#endif //! View_Molecule_h__
