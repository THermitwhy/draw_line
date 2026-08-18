#include "line_canvas_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LineCanvasWidget window;
    window.setWindowTitle("2D Line Anti-Aliasing Experiment");
    window.resize(800, 600);
    window.show();

    return app.exec();
}
