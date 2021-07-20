#ifndef View_Workpace_h__
#define View_Workpace_h__

#ifdef _MSC_VER
#  pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QListWidget>

class ViewWorkspace : public QListWidget
{
  Q_OBJECT
public:
  ViewWorkspace(QWidget* /*parent*/ = nullptr);
  ~ViewWorkspace() override = default;
};

#endif // !View_Workpace_h__
