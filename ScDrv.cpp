#include <QVTKOpenGLNativeWidget.h>
#include <QVTKApplication.h>

#include <QSurfaceFormat>
#include <QCoreApplication>

#include <QCommandLineParser>
#include <QStringList>
#include <QString>

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

#include "FrameTop.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(ViewMolecule::defaultFormat());

    QVTKApplication app(argc, argv);
    // VTK-speccific application part ends...
    //
    QApplication::setQuitOnLastWindowClosed(true);
    // set false if one would make use of
    // the tray activation system. Who would dare?
    // --------------------------------------------
    //
    QCoreApplication::setApplicationName("ScDrv Browser");
    QCoreApplication::setOrganizationName("Nekronomykon");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    //
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file(s) to open.");
    parser.process(app);
    //
    FrameTop *pFrame = FrameTop::createNew();
    const QStringList all_paths_to_open = parser.positionalArguments();
    if (!all_paths_to_open.empty())
        pFrame->addPathList(all_paths_to_open);
    pFrame->tileAgainst()->show();

    return app.exec();
}
