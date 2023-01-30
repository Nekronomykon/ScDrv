#ifndef ChooseColor_byName__h
#define ChooseColor_byName__h

#include <QComboBox>

#include <QString>
#include <QStringList>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkColor.h>
#include <vtkStdString.h>
#include <vtkNamedColors.h>

class QVTKNamedColors
    : public QComboBox
{
  Q_OBJECT
public:
  explicit QVTKNamedColors(QWidget * /*parent*/ = nullptr);
  void setTextByColor(const vtkColor3d &color); 

private:
  static QStringList ColorNames();
};

#endif // !ChooseColor_byName__h
