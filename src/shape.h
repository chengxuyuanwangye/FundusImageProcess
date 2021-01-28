#ifndef SHAPE_H
#define SHAPE_H

#include<QOpenGLBuffer>
#include<QOpenGLVertexArrayObject>
#include<QOpenGLFunctions>
class QOpenGLShaderProgram;
class Camera;
class QImage;
class QOpenGLTexture;
class Shape : public QObject,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Shape(int width,int height);
    ~Shape();
    void Render();
    void Resize(int width, int height);
    void SetTranslateVec(QVector3D vec);
    void SetTexture(QImage * srcimg);
    Camera* ShapeCamera;
protected:
    bool CreateShaderProgram(QString vertexpath,QString fragpath);
    QOpenGLBuffer* m_vbo;//顶点缓冲对象
    QOpenGLBuffer* m_ebo;//索引缓冲对象
    QOpenGLVertexArrayObject* m_vao;//顶点数组对象
    QOpenGLShaderProgram *m_program;
    QOpenGLTexture *m_curTexture;
    bool m_visible;
    int m_width;
    int m_height;
private:
    bool m_imageInitialized;
    float imgrect_width;
    float imgrect_height;
};



#endif // SHAPE_H
