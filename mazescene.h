#ifndef MAZESCENE_H
#define MAZESCENE_H

#include <GL/glew.h>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPointF>
#include <QPushButton>
#include <QTime>
#include <QTimeLine>
#include <QGLWidget>
#include <QMatrix4x4>
#include <QWebView>
#include <QGLShaderProgram>

class MazeScene;
class MediaPlayer;
class Entity;
class WalkingItem;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View();
    void resizeEvent(QResizeEvent *event);
    void setScene(MazeScene *scene);

private:
    MazeScene *m_scene;
};

class Camera
{
public:
    Camera()
        : m_yaw(0)
        , m_pitch(0)
        , m_fov(70)
        , m_time(0)
        , m_matrixDirty(true)
    {
    }

    qreal yaw() const { return m_yaw; }
    qreal pitch() const { return m_pitch; }
    qreal fov() const { return m_fov; }
    QPointF pos() const { return m_pos; }

    void setYaw(qreal yaw);
    void setPitch(qreal pitch);
    void setPos(const QPointF &pos);
    void setFov(qreal fov);
    void setTime(qreal time);

    const QMatrix4x4 &viewProjectionMatrix() const;
    const QMatrix4x4 &viewMatrix() const;

private:
    void updateMatrix() const;

    qreal m_yaw;
    qreal m_pitch;
    qreal m_fov;
    qreal m_time;

    QPointF m_pos;

    mutable bool m_matrixDirty;
    mutable QMatrix4x4 m_viewMatrix;
    mutable QMatrix4x4 m_viewProjectionMatrix;
};

class Light
{
public:
    Light() {}
    Light(const QPointF &pos, qreal intensity);

    qreal intensityAt(const QPointF &pos) const;

    QPointF pos() const { return m_pos; }
    qreal intensity() const { return m_intensity; }

private:
    QPointF m_pos;
    qreal m_intensity;
};

class ProjectedItem : public QGraphicsItem
{
public:
    ProjectedItem(const QRectF &bounds, bool shadow = true, bool opaque = true);

    QPointF a() const { return m_a; }
    QPointF b() const { return m_b; }

    virtual void updateTransform(const Camera &camera);

    void setOpaque(bool opaque);
    bool isOpaque() const;

    QRectF boundingRect() const;

    void setPosition(const QPointF &a, const QPointF &b);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setAnimationTime(qreal time);
    void setImage(const QImage &image);
    void updateLighting(const QVector<Light> &lights, bool useConstantLight);

    void setLightingEnabled(bool enabled);

    void setObscured(bool obscured);
    bool isObscured() const;

private:
    QPointF m_a;
    QPointF m_b;
    QRectF m_bounds;
    QRectF m_targetRect;
    QImage m_image;
    QGraphicsRectItem *m_shadowItem;

    bool m_opaque;
    bool m_obscured;
};

class WallItem : public ProjectedItem
{
public:
    WallItem(MazeScene *scene, const QPointF &a, const QPointF &b, int type);

    QGraphicsProxyWidget *childItem() const
    {
        return m_childItem;
    }

    int type() const { return m_type; }

    void childResized();

private:
    QGraphicsProxyWidget *m_childItem;
    int m_type;
    qreal m_scale;
};

class MazeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MazeScene(const QVector<Light> &lights, const char *map, int width, int height);

    void addProjectedItem(ProjectedItem *item);
    void addEntity(Entity *entity);
    void addWall(const QPointF &a, const QPointF &b, int type);
    void drawBackground(QPainter *painter, const QRectF &rect);

    bool tryMove(QPointF &pos, const QPointF &delta, Entity *entity = 0) const;

    Camera camera() const { return m_camera; }

    void viewResized(QGraphicsView *view);
    QWebView *view;


protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    bool eventFilter(QObject *target, QEvent *event);

    bool handleKey(int key, bool pressed);

public slots:
    void move();
  //  void toggleRenderer();
    void toggleDoors();
    void loadFinished();
    void updateRenderer();
    void changeurl();

private slots:
    void moveDoors(qreal value);

private:
    bool blocked(const QPointF &pos, Entity *entity) const;
    void updateTransforms();


    QVector<WallItem *> m_walls;
    QVector<WallItem *> m_doors;
    QVector<QGraphicsItem *> m_floorTiles;
    QVector<QPushButton *> m_buttons;
    QVector<Entity *> m_entities;
    QVector<Light> m_lights;
    int m_width;
    int m_height;
    QVector<ProjectedItem *> m_projectedItems;


    Camera m_camera;

    qreal m_walkingVelocity;
    qreal m_strafingVelocity;
    qreal m_turningSpeed;
    qreal m_pitchSpeed;

    qreal m_deltaYaw;
    qreal m_deltaPitch;
    QTime m_time;
    QTimeLine *m_doorAnimation;
    long m_simulationTime;
    long m_walkTime;

    MediaPlayer *m_player;
    QPointF m_playerPos;

   // WalkingItem *m_walkingItem;
};

#endif
