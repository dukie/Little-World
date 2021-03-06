#include "modelitem.h"
#include "model.h"

#include <QtGui>
#include "mazescene.h"


#define GL_MULTISAMPLE  0x809D

#include <QVector2D>

void ModelItem::updateTransform(const Camera &camera)
{
    QPointF pos(3, 7);

    QVector2D toCamera = QVector2D(camera.pos() - pos).normalized();
    QPointF delta(toCamera.y(), -toCamera.x());

    setPosition(pos - delta, pos + delta);

    ProjectedItem::updateTransform(camera);

    setTransform(QTransform());

    m_matrix = camera.viewMatrix();
    m_matrix.translate(3, 0, 7);
}

QRectF ModelItem::boundingRect() const
{
    if (!scene()->views().isEmpty()) {
        QGraphicsView *view = scene()->views().at(0);
        return view->mapToScene(view->rect()).boundingRect();
    }

    return scene()->sceneRect();
}

void ModelItem::updateItem()
{
    ProjectedItem::update();
}

const char *vertexProgram =
    "attribute highp    vec4    vertexCoordsArray;"
    "attribute highp    vec4    normalCoordsArray;"
    "varying   highp    vec4    normal;"
    "uniform   highp    mat4    pmvMatrix;"
    "uniform   highp    mat4    modelMatrix;"
    "void main(void)"
    "{"
    "        normal = modelMatrix * vec4(normalCoordsArray.xyz, 0);"
    "        gl_Position = pmvMatrix * vertexCoordsArray;"
    "}";

const char *fragmentProgram =
    "uniform   lowp     vec4    color;"
    "varying   highp    vec4    normal;"
    "void main() {"
    "   lowp vec3 toLight = normalize(vec3(-0.9, -1, 0.6));"
    "   gl_FragColor = color * (0.4 + 0.6 * max(dot(normalize(normal).xyz, toLight), 0.0));"
    "}";

QMatrix4x4 fromProjection(float fov);
QMatrix4x4 fromRotation(float angle, Qt::Axis axis);

void ModelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_model || isObscured())
        return;

    QMatrix4x4 projectionMatrix = QMatrix4x4(painter->transform()) * fromProjection(70);

    const int delta = m_time.elapsed() - m_lastTime;
    m_rotation += m_angularMomentum * (delta / 1000.0);
    m_lastTime += delta;

    QVector3D size = m_model->size();
    float extent = qSqrt(2.0);
    float scale = 1 / qMax(size.y(), qMax(size.x() / extent, size.z() / extent));
    QMatrix4x4 modelMatrix;
    modelMatrix.scale(scale, -scale, scale);

    modelMatrix = fromRotation(m_rotation.z(), Qt::ZAxis) * modelMatrix;
    modelMatrix = fromRotation(m_rotation.y(), Qt::YAxis) * modelMatrix;
    modelMatrix = fromRotation(m_rotation.x(), Qt::XAxis) * modelMatrix;

    QTimer::singleShot(10, this, SLOT(updateItem()));


    m_wireframe->setEnabled(true);

    painter->beginNativePainting();



    glClearDepth(0);

    glClear(GL_DEPTH_BUFFER_BIT);

    if (!m_program) {

        glewInit();

        m_program = new QGLShaderProgram;
        m_program->addShaderFromSourceCode(QGLShader::Fragment, fragmentProgram);
        m_program->addShaderFromSourceCode(QGLShader::Vertex, vertexProgram);
        m_program->bindAttributeLocation("vertexCoordsArray", 0);
        m_program->bindAttributeLocation("normalCoordsArray", 1);
        m_program->link();

        if (!m_program->isLinked())
            qDebug() << m_program->log();
    }

    qreal ortho[] = {
        2.0f / painter->device()->width(), 0, 0, -1,
        0, -2.0f / painter->device()->height(), 0, 1,
        0, 0, -1, 0,
        0, 0, 0, 1
    };

    m_program->bind();
    m_program->setUniformValue("color", m_modelColor);
    m_program->setUniformValue("pmvMatrix", QMatrix4x4(ortho) * projectionMatrix * m_matrix * modelMatrix);
    m_program->setUniformValue("modelMatrix", modelMatrix);
    m_model->render(m_wireframeEnabled, m_normalsEnabled);
    m_program->release();

    painter->endNativePainting();

}


ModelItem::ModelItem()
    : ProjectedItem(QRectF(), false, false)
    , m_wireframeEnabled(false)
    , m_normalsEnabled(false)
    , m_modelColor(153, 255, 0)
    , m_model(0)
    , m_lastTime(0)
    , m_distance(1.4f)
    , m_angularMomentum(0, 40, 0)
    , m_program(0)
{
    setLayout(new QVBoxLayout);

    m_modelButton = new QPushButton(tr("Load model"));
    connect(m_modelButton, SIGNAL(clicked()), this, SLOT(loadModel()));
#ifndef QT_NO_CONCURRENT
    connect(&m_modelLoader, SIGNAL(finished()), this, SLOT(modelLoaded()));
#endif
    layout()->addWidget(m_modelButton);

    m_wireframe = new QCheckBox(tr("Render as wireframe"));
    connect(m_wireframe, SIGNAL(toggled(bool)), this, SLOT(enableWireframe(bool)));
    layout()->addWidget(m_wireframe);

    QCheckBox *normals = new QCheckBox(tr("Display normals vectors"));
    connect(normals, SIGNAL(toggled(bool)), this, SLOT(enableNormals(bool)));
    layout()->addWidget(normals);

    QPushButton *colorButton = new QPushButton(tr("Choose model color"));
    connect(colorButton, SIGNAL(clicked()), this, SLOT(setModelColor()));
    layout()->addWidget(colorButton);

    loadModel(QLatin1String("wal.obj"));
    m_time.start();
}

static Model *loadModel(const QString &filePath)
{
    return new Model(filePath);
}

void ModelItem::loadModel()
{
    loadModel(QFileDialog::getOpenFileName(0, tr("Choose model"), QString(), QLatin1String("*.obj")));
}

void ModelItem::loadModel(const QString &filePath)
{
    if (filePath.isEmpty())
        return;

    m_modelButton->setEnabled(false);
    QApplication::setOverrideCursor(Qt::BusyCursor);
#ifndef QT_NO_CONCURRENT
    m_modelLoader.setFuture(QtConcurrent::run(::loadModel, filePath));
#else
    setModel(::loadModel(filePath));
    modelLoaded();
#endif
}

void ModelItem::modelLoaded()
{
#ifndef QT_NO_CONCURRENT
    setModel(m_modelLoader.result());
#endif
    m_modelButton->setEnabled(true);
    QApplication::restoreOverrideCursor();
}

void ModelItem::setModel(Model *model)
{
    delete m_model;
    m_model = model;

    ProjectedItem::update();
}

void ModelItem::enableWireframe(bool enabled)
{
    m_wireframeEnabled = enabled;
    ProjectedItem::update();
}

void ModelItem::enableNormals(bool enabled)
{
    m_normalsEnabled = enabled;
    ProjectedItem::update();
}

void ModelItem::setModelColor()
{
    const QColor color = QColorDialog::getColor(m_modelColor);
    if (color.isValid()) {
        m_modelColor = color;
        ProjectedItem::update();
    }
}
