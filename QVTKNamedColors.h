#ifndef ChooseColor_byName__h
#define ChooseColor_byName__h

#include <QComboBox>

#include <QString>
#include <QStringList>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkStdString.h>
#include <vtkNamedColors.h>

class QVTKNamedColors
    : public QComboBox
{
  Q_OBJECT
public:
  explicit QVTKNamedColors(QWidget * /*parent*/ = nullptr);

  static QString getDefaultColorName();
  static void resetDefaultColorName(const QString&);

  void adjustColorName();

private:
  static QStringList ColorNames();
  static QString name_color_bg;
};

#endif // !ChooseColor_byName__h
