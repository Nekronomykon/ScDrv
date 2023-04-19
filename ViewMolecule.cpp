#include "ViewMolecule.h"

#include <vtkProperty.h>

#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>
#include <vtkTextProperty.h>

#include <vtkTrivialProducer.h>

#include <vtkCamera.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleRubberBandPick.h>

ViewMolecule::ViewMolecule(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
  // init from registry?
  colorBg_.SetRed(0.9);    // debug colors
  colorBg_.SetGreen(0.85); // total up tp neither yellow
  colorBg_.SetBlue(0.95);  // nor light-yellow

  vtkRenderWindow *pRW = this->renderWindow();
  pRW->SetNumberOfLayers(2);

  // -----------------------------------------------------------
  // read these properties from registry?
  actorMol_->GetProperty()->SetDiffuse(0.75);
  actorMol_->GetProperty()->SetSpecular(0.5);
  actorMol_->GetProperty()->SetSpecularPower(20.0);
  // -----------------------------------------------------------
  actorMol_->SetMapper(this->getMoleculeMapper());
  renderMol_->AddActor(actorMol_);
  //
  vtkRenderWindowInteractor *pRWI = this->interactor();
  if (!pRWI)
  {
    ANewRenderWindowInteractor new_rwi;
    pRW->SetInteractor(new_rwi);
    pRWI = new_rwi.Get();
  }

  vtkSmartPointer<vtkInteractorStyleTrackballCamera>
      istyle_rb(vtkInteractorStyleRubberBandPick::New()); // see the song...
  // The InteractorStyle mechanism is the 3D GUI mechanism to be used in editing,
  // So we would try to override it by our own mechanism... yet not now.
  // In progress. Under the development.
  pRWI->SetInteractorStyle(istyle_rb);
  renderBg_->SetLayer(0);
  pRW->AddRenderer(renderBg_);
  renderMol_->SetLayer(1);
  // -----------------------------------------------------------
  // read these properties from registry?
  // initial atom labels in molecule
  vtkTextProperty *pAtomTextProp = mapLabelAtoms_->GetLabelTextProperty();
  pAtomTextProp->UseTightBoundingBoxOn();
  pAtomTextProp->SetFontSize(14);
  pAtomTextProp->SetFontFamilyToCourier();
  pAtomTextProp->BoldOn();
  // pAtomTextProp->SetColor(colors->GetColor3d("Yellow").GetData());
  if (bLabelAtoms_)
  {
    // mapLabelAtoms_->SetInputConnection(mapMol_->GetAtomGlyphPointOut()->GetOutputPort());
    // renderMol_->AddActor2D(actorLabelAtoms_);
  }

  pRW->AddRenderer(renderMol_);
  this->updateBackground();
}
//
///////////////////////////////////////////////////////////////////////////////
//
vtkColor3d ViewMolecule::getBackgroundColor(void) const
{
  return colorBg_;
}
//
vtkColor3d &ViewMolecule::backgroundColor(void)
{
  return colorBg_;
}
//
///////////////////////////////////////////////////////////////////////////////
//
vtkMoleculeMapper *ViewMolecule::getMoleculeMapper(void) const
{
  return mapMol_;
}
///////////////////////////////////////////////////////////////////////////////
//
vtkRenderer *ViewMolecule::getMoleculeRenderer(void) const
{
  return renderMol_;
}
//
///////////////////////////////////////////////////////////////////////////////
//
void ViewMolecule::updateBackground(vtkRenderWindow *pRW)
{
  if (!pRW)
    pRW = this->renderWindow();
  // [0] BACKGROUND
  renderBg_->SetBackground(colorBg_.GetData());
  pRW->Modified();
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool ViewMolecule::exportImageTo(vtkImageWriter *pIW, bool bAlpha)
{
  if (!pIW)
    return false;
  //
  vtkRenderWindow *pRW = this->renderWindow();
  if (!pRW)
    return false;
  pRW->Render();
  //
  vtkNew<vtkWindowToImageFilter> w2if;
  w2if->SetFixBoundary(true);
  // w2if->SetScale(4); --> how to scale label fonts???  to think on...
  //
  w2if->SetInput(pRW);
  if (bAlpha)
    w2if->SetInputBufferTypeToRGBA();
  else
    w2if->SetInputBufferTypeToRGB();
  w2if->ReadFrontBufferOff();
  w2if->Update();

  pIW->SetInputConnection(w2if->GetOutputPort());
  pIW->Write();
  //
  pRW->Modified();
  return true;
}
//
///////////////////////////////////////////////////////////////////////////////
//
ViewMolecule *ViewMolecule::setProjectParallel(bool bReset)
{
  vtkRenderWindow *pRW = this->renderWindow();
  vtkRenderer *pRen = this->getMoleculeRenderer();
  vtkCamera *pCam = pRen->GetActiveCamera();
  if (!pCam->GetParallelProjection())
    pCam->ParallelProjectionOn();
  else
  {
    if (bReset)
      pRen->ResetCamera();
  }
  pRW->Modified();
  return this;
}
//
///////////////////////////////////////////////////////////////////////////////
//
ViewMolecule *ViewMolecule::setProjectPerspective(bool bReset)
{
  vtkRenderWindow *pRW = this->renderWindow();
  vtkRenderer *pRen = this->getMoleculeRenderer();
  vtkCamera *pCam = pRen->GetActiveCamera();
  if (pCam->GetParallelProjection())
    pCam->ParallelProjectionOff();
  else
  {
    if (bReset)
      pRen->ResetCamera();
  }
  pRW->Modified();
  return this;
}
//
///////////////////////////////////////////////////////////////////////////////
//
ViewMolecule *ViewMolecule::resetMolecule(vtkMolecule *pMol)
{
  vtkRenderWindow *pRW = this->renderWindow();
  if (pMol)
    mapMol_->SetInputData(pMol);
  mapMol_->Update();
  pRW->Modified();
  return this;
}
//
///////////////////////////////////////////////////////////////////////////////
//
