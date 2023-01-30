#include "ViewMolecule.h"

#include <vtkRenderWindow.h>

ViewMolecule::ViewMolecule(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
    // init from registry?
    colorBg_.SetRed(0.9);    // debug colors
    colorBg_.SetGreen(0.85); // total up to yellow
    colorBg_.SetBlue(0.95);  // light-yellow

    vtkRenderWindow *pRW = this->renderWindow();

    // [0] BACKGROUND
    pRW->AddRenderer(renderBg_);
    this->updateBackground();
}

vtkColor3d ViewMolecule::getBackgroundColor() const
{
    return colorBg_;
}

vtkColor3d &ViewMolecule::backgroundColor()
{
    return colorBg_;
}

void ViewMolecule::updateBackground()
{
    renderBg_->SetLayer(0);
    renderBg_->SetBackground(colorBg_.GetData());
    this->renderWindow()->Render();
}