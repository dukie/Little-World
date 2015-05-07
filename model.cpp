#include "model.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QVarLengthArray>



Model::Model(const QString &filePath)
    : m_fileName(QFileInfo(filePath).fileName())
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QVector3D boundsMin( 1e9, 1e9, 1e9);
    QVector3D boundsMax(-1e9,-1e9,-1e9);

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString input = in.readLine();
        if (input.isEmpty() || input[0] == '#')
            continue;

        QTextStream ts(&input);
        QString id;
        ts >> id;
        if (id == "v") {
            QVector3D p;
            for (int i = 0; i < 3; ++i) {
                ts >> ((float *)&p)[i];
                ((float *)&boundsMin)[i] = qMin(((float *)&boundsMin)[i], ((float *)&p)[i]);
                ((float *)&boundsMax)[i] = qMax(((float *)&boundsMax)[i], ((float *)&p)[i]);
            }
            m_points << p;
        } else if (id == "f" || id == "fo") {
            QVarLengthArray<int, 4> p;

            while (!ts.atEnd()) {
                QString vertex;
                ts >> vertex;
                const int vertexIndex = vertex.split('/').value(0).toInt();
                if (vertexIndex)
                    p.append(vertexIndex > 0 ? vertexIndex - 1 : m_points.size() + vertexIndex);
            }

            for (int i = 0; i < p.size(); ++i) {
                const int edgeA = p[i];
                const int edgeB = p[(i + 1) % p.size()];

                if (edgeA < edgeB)
                    m_edgeIndices << edgeA << edgeB;
            }

            for (int i = 0; i < 3; ++i)
                m_pointIndices << p[i];

            if (p.size() == 4)
                for (int i = 0; i < 3; ++i)
                    m_pointIndices << p[(i + 2) % 4];
        }
    }

    const QVector3D bounds = boundsMax - boundsMin;
    const qreal scale = 1 / qMax(bounds.x() / 1, qMax(bounds.y(), bounds.z() / 1));
    for (int i = 0; i < m_points.size(); ++i)
        m_points[i] = (m_points[i] - (boundsMin + bounds * 0.5)) * scale;

    m_size = bounds * scale;

    m_normals.resize(m_points.size());
    for (int i = 0; i < m_pointIndices.size(); i += 3) {
        const QVector3D a = m_points.at(m_pointIndices.at(i));
        const QVector3D b = m_points.at(m_pointIndices.at(i+1));
        const QVector3D c = m_points.at(m_pointIndices.at(i+2));

        const QVector3D normal = QVector3D::crossProduct(b - a, c - a).normalized();

        for (int j = 0; j < 3; ++j)
            m_normals[m_pointIndices.at(i + j)] += normal;
    }

    for (int i = 0; i < m_normals.size(); ++i)
        m_normals[i] = m_normals[i].normalized();
}

QVector3D Model::size() const
{
    return m_size;
}

void Model::render(bool wireframe, bool normals) const
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GEQUAL);
    glDepthMask(true);


    GLenum elementType = GL_UNSIGNED_INT;


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (float *)m_points.data());
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (float *)m_normals.data());

    if (wireframe)
        glDrawElements(GL_LINES, m_edgeIndices.size(), elementType, m_edgeIndices.data());
    else
        glDrawElements(GL_TRIANGLES, m_pointIndices.size(), elementType, m_pointIndices.data());

    if (normals) {
        QVector<QVector3D> points;
        QVector<QVector3D> normals;
        for (int i = 0; i < m_normals.size(); ++i) {
            points << m_points.at(i) << (m_points.at(i) + m_normals.at(i) * 0.02f);
            normals << m_normals.at(i) << m_normals.at(i);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (float *)points.data());
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (float *)normals.data());
        glDrawArrays(GL_LINES, 0, points.size());
    }

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glDisable(GL_DEPTH_TEST);
}
