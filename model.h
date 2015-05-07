#ifndef MODEL_H
#define MODEL_H

#include <QPainter>
#include <QString>
#include <QVector>

#include <math.h>

#include <QMatrix4x4>
#include <QVector3D>
#include <GL/glew.h>

class Model
{
public:
    Model() {}
    Model(const QString &filePath);

    void render(bool wireframe = false, bool normals = false) const;
    void render(QPainter *painter, const QMatrix4x4 &matrix, bool normals = false) const;

    QString fileName() const { return m_fileName; }
    int faces() const { return m_pointIndices.size() / 3; }
    int edges() const { return m_edgeIndices.size() / 2; }
    int points() const { return m_points.size(); }

    QVector3D size() const;

private:
    QString m_fileName;

    QVector<QVector3D> m_points;
    QVector<QVector3D> m_normals;

#ifdef QT_OPENGL_ES_2
    QVector<ushort> m_edgeIndices;
    QVector<ushort> m_pointIndices;
#else
    QVector<uint> m_edgeIndices;
    QVector<uint> m_pointIndices;
#endif

    QVector3D m_size;

    mutable QVector<QLineF> m_lines;
    mutable QVector<QVector3D> m_mapped;
};

#endif
