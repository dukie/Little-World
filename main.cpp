#include <QtGui>
#include "mazescene.h"

int main(int argc, char **argv)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);
    app.setApplicationName("LittleWorld");
    QPixmapCache::setCacheLimit(100 * 1024); // 100 MB

    const char *map =
            "###&?##/#&##&##=########"
            "#                      #"
            "#                  #   &"
            "#                      #"
            "##/4/&%-#              #"
            "&       #    #  #      #"
            "* @@    #              &"
            "# @@    #       #      #"
            "#       #              #"
            "#############&#/#&#/####";

    QVector<Light> lights;
    lights << Light(QPointF(3.5, 2.5), 1)
           << Light(QPointF(3.5, 6.5), 1)
           << Light(QPointF(1.5, 10.5), 0.3)
           << Light(QPointF(20.5, 10.5), 0.3);

    MazeScene *scene = new MazeScene(lights, map, 24, 10);

    View view;
    view.resize(1024, 768);
    view.setScene(scene);
    view.show();
    QGraphicsView *tmpview = scene->views().at(0);
    tmpview->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    scene->updateRenderer();

    return app.exec();
}
