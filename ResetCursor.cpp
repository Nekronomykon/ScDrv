#include "ResetCursor.h"

#include <QApplication>

ResetCursor::ResetCursor(const QCursor &crs)
{
  QApplication::setOverrideCursor(crs);
}

ResetCursor::ResetCursor(Qt::CursorShape shape)
{
  QApplication::setOverrideCursor(QCursor(shape));
}

// ----------------------------------------------------------------- //
ResetCursor::~ResetCursor()
{
  QApplication::restoreOverrideCursor();
}
