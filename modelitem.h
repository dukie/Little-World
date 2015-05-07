#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include <QVector3D>

#include <QCheckBox>
#include <QWidget>
#include <QTime>

#ifndef QT_NO_CONCURRENT
#include <QFutureWatcher>
#endif

#include "mazescene.h"

QT_BEGIN_NAMESPACE
class QGLShaderProgram;
QT_END_NAMESPACE

class Model;

class ModelItem : public QWidget, public ProjectedItem
{
    Q_OBJECT

public:
    ModelItem();

    void updateTransform(const Camera &camera);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void enableWireframe(bool enabled);
    void enableNormals(bool enabled);
    void setModelColor();
    void loadModel();
    void loadModel(const QString &filePath);
    void modelLoaded();
    void updateItem();

private:
    void setModel(Model *model);

    bool m_wireframeEnabled;
    bool m_normalsEnabled;
    bool m_useQPainter;

    QColor m_modelColor;

    Model *m_model;

    QTime m_time;
    int m_lastTime;
    int m_mouseEventTime;

    float m_distance;
    QVector3D m_rotation;
    QVector3D m_angularMomentum;
    QVector3D m_accumulatedMomentum;

    QWidget *m_modelButton;
    QCheckBox *m_wireframe;

#ifndef QT_NO_CONCURRENT
    QFutureWatcher<Model *> m_modelLoader;
#endif
    QMatrix4x4 m_matrix;

#ifndef QT_NO_OPENGL
    mutable QGLShaderProgram *m_program;
#endif
};

#endif
