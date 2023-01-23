#include "ViewWorkspace.h"

ViewWorkspace::ViewWorkspace(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), files_(new ViewFileList(this)), content_(new ViewFileContent(this))
{
    this->addWidget(files_);
    this->addWidget(content_);
}
