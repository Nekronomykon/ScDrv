#include "ViewWorkspace.h"

ViewWorkspace::ViewWorkspace(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), files_(new ViewPathList(this)), content_(new ViewFileContent(this))
{
    this->setOpaqueResize(false);
    //
    this->addWidget(files_);
    this->addWidget(content_);
}
