#ifndef View_Molecule_h__
#define View_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QVTKOpenGLNativeWidget.h>

class ViewMolecule
    : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit ViewMolecule(QWidget * /*parent*/ = nullptr);
    ~ViewMolecule() override = default;
};

#endif //! View_Molecule_h__
