#pragma once

#include <QOpenGLWidget>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include <QList>
#include <QPushButton>

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class MainWindow;

class GLWidget : public QOpenGLWidget, QOpenGLFunctions
{
    Q_OBJECT
public:
    GLWidget(MainWindow* parent);
    ~GLWidget();

    void reset();

public slots:
    void setScaling(int scale);
    void setTransparent(bool transparent);

protected:
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initializeGL() override;

private:

    MainWindow* m_mainWindow;

    qreal m_fAngle = 0;
    qreal m_fScale = 1;
    bool m_showBubbles = true;
    QList<QVector3D> m_vertices;
    QList<QVector3D> m_normals;
    int m_frames = 0;
    QElapsedTimer m_time;
    QOpenGLShader *m_vshader1 = nullptr;
    QOpenGLShader *m_fshader1 = nullptr;

    QOpenGLShaderProgram *m_program1 = nullptr;

    QOpenGLTexture *m_texture = nullptr;
    QOpenGLBuffer m_vbo1;

    int m_vertexAttr1 = 0;
    int m_normalAttr1 = 0;
    int m_matrixUniform1 = 0;
    bool m_transparent = false;
    QColor m_background;
    QMetaObject::Connection m_contextWatchConnection;
};
