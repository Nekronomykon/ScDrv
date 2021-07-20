#ifndef View_Messages_h__
#define View_Messages_h__

#ifdef _MSC_VER
#  pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QListWidget>

class ViewMessages : public QListWidget
{
  Q_OBJECT
public:
  ViewMessages(QWidget* /*parent*/ = nullptr);
  ~ViewMessages() override = default;
};

#endif // !View_Messages_h__
