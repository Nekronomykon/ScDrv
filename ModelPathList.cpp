#include "ModelPathList.h"

#include <QFileInfo>

ModelPathList::ModelPathList()
    : QStringListModel()
{
}

QVariant ModelPathList::data(const QModelIndex &index, int role) const
{
  if (role != Qt::DecorationRole)
    return QStringListModel::data(index, role);
  QVariant res;
  QString str(data(index).toString());
  if (str.compare( keyNewFile() ) )
  {
    QFileInfo fi(str);
    res = (QVariant)fip_.icon(fi);
  }
  return res;
}