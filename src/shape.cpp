#include "shape.h"
#include<QDebug>
#include<QOpenGLShaderProgram>
#include<QImage>
#include<QOpenGLTexture>
#include<QOpenGLVertexArrayObject>
#include<QOpenGLFunctions>
Shape::Shape(int width,int height)
: QObject(nullptr),
  m_visible(false),
  m_width(width),
  m_height(height),
  m_imageInitialized(false)
{
    m_curTexture=nullptr;
    m_vbo=nullptr;//顶点缓冲对象
    m_ebo=nullptr;//索引缓冲对象
    m_vao=nullptr;//顶点数组对象
    m_program=nullptr;

}

Shape::~Shape()
{

    if(m_vbo!=nullptr)
    {
        m_vbo->destroy();
        delete m_vbo;
    }
    if(m_ebo!=nullptr){
        m_ebo->destroy();
        delete m_ebo;
    }
    if(m_vao!=nullptr)
    {
        m_vao->destroy();
        delete m_vao;
    }
    if(m_program!=nullptr)
        delete m_program;

    if(m_curTexture!=nullptr)
        delete m_curTexture;
}

///
/// \brief Shape::Render
///渲染函数，只有在设置图像以后才进行渲染操作
void Shape::Render()
{
    if(m_imageInitialized)
    {
        m_program->bind();
        {
            glActiveTexture(GL_TEXTURE0);
            m_curTexture->bind();
            QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
           // GLenum result= glGetError();
            m_vao->release();
            m_curTexture->release();

        }
        m_program->release();
    }

}

///
/// \brief Shape::SetTexture
///创建纹理
/// 创建VAO和VBO，
/// 创建着色器函数
void Shape::SetTexture(QImage* srcimg)
{

    int baselen= srcimg->width()>srcimg->height()?srcimg->width():srcimg->height();
    float imgwidth=srcimg->width()/(float)baselen;
    float imgheight=srcimg->height()/(float)baselen;
    imgrect_width=imgwidth;
    imgrect_height=imgheight;
qDebug()<<"imgwidth"<<imgwidth;
qDebug()<<"imgheight"<<imgheight;

  /*  GLfloat vertices[] = {
        //positions                                    //texture coords
        imgwidth/baselen,  imgheight/baselen, 0.0f,      1.0f,1.0f, // top right
        imgwidth/baselen,  -imgheight/baselen, 0.0f,     1.0f,0.0f,// bottom right
        -imgwidth/baselen,  -imgheight/baselen, 0.0f,    0.0f,0.0f,// bottom left
        -imgwidth/baselen,  imgheight/baselen, 0.0f,     0.0f,1.0f// top left
    };*/

    GLfloat vertices[] = {
           //positions                                    //texture coords
           imgwidth,  imgheight, 0.0f,      1.0f,1.0f, // top right
           imgwidth,  -imgheight, 0.0f,     1.0f,0.0f,// bottom right
           -imgwidth,  -imgheight, 0.0f,    0.0f,0.0f,// bottom left
           -imgwidth,  imgheight, 0.0f,     0.0f,1.0f// top left
       };


    initializeOpenGLFunctions();

    bool shadersuccess=  CreateShaderProgram( ":/shader/shader/rect.vert",":/shader/shader/rect.frag");
    if(!shadersuccess)return;

    unsigned int indices[] = {  // note that we start from 0!
                                0, 1, 3,  // first Triangle
                                1, 2, 3   // second Triangle
                             };

    m_vao=new QOpenGLVertexArrayObject;
    m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_ebo=new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(vertices,sizeof(vertices));

    m_ebo->create();
    m_ebo->bind();
    m_ebo->allocate(indices,sizeof (indices));

    int attr=-1;
    attr=m_program->attributeLocation("aPos");
    m_program->setAttributeBuffer(attr,GL_FLOAT,0,3,sizeof(GL_FLOAT)*5);
    m_program->enableAttributeArray(attr);

    int textattr=-1;
    textattr=m_program->attributeLocation("aTexCoords");
    m_program->setAttributeBuffer(textattr,GL_FLOAT,sizeof(GL_FLOAT)*3,2,sizeof(GL_FLOAT)*5);
    m_program->enableAttributeArray(textattr);

    m_vbo->release();

    m_curTexture=new QOpenGLTexture(srcimg->mirrored(false, true),QOpenGLTexture::GenerateMipMaps);
    if(!m_curTexture->isCreated())
    {
        qDebug()<<"failed to load texture";
    }
    else {
        m_curTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_curTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        m_curTexture->setMinificationFilter(QOpenGLTexture::Linear);
        m_curTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    }
    m_program->bind();
    QMatrix4x4 model;
    model.setToIdentity();


    float temp=m_width>m_height?m_width:m_height;
    float scale=imgrect_width/ (m_width/temp)>imgrect_height/(m_height/temp)?imgrect_width/ (m_width/temp):imgrect_height/(m_height/temp);
     model.scale(QVector3D(0.95f/scale,0.95f/scale,1.0f));
  m_program->setUniformValue("model", model);
    QMatrix4x4 view;
    view.setToIdentity();
    view.translate(QVector3D(0.0f,0.0f,-3.0f));
    m_program->setUniformValue("view", view);
    QMatrix4x4 projection;
    projection.setToIdentity();

    projection.ortho(-1*m_width/temp, m_width/temp, -1*m_height/temp, m_height/temp, 0.1f, 100.0f);
    qDebug()<<"m_width/temp"<<m_width/temp;
    qDebug()<<"m_height/temp"<<m_height/temp;
    m_program->setUniformValue("projection", projection);
    m_program->setUniformValue("ourTexture", 0);
    m_program->release();

    m_imageInitialized=true;

}
void Shape::SetTranslateVec(QVector3D vec)
{
    Q_UNUSED(vec);

}

void Shape::Resize(int width, int height)
{
    Q_UNUSED(width);
    Q_UNUSED(height);
    m_width=width;
    m_height=height;
    if(m_imageInitialized)
    {
        m_program->bind();
        QMatrix4x4 projection;
        projection.setToIdentity();
        float temp=m_width>m_height?m_width:m_height;
        projection.ortho(-1*m_width/temp, m_width/temp, -1*m_height/temp, m_height/temp, 0.1f, 100.0f);
        m_program->setUniformValue("projection", projection);
        m_program->setUniformValue("ourTexture", 0);
        m_program->release();
    }
}
 bool Shape::CreateShaderProgram(QString vertexpath,QString fragpath)
 {

    m_program = new QOpenGLShaderProgram(this);
    bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexpath);
     if (!success) {
         qDebug() << " addShaderFromSourceFile failed!" << m_program->log();
         return false;
     }

     //加载片段着色器程序
   success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragpath);
   if (!success) {
       qDebug() << " addShaderFromSourceFile failed!" << m_program->log();
       return false;
   }
     //链接着色器程序
     success = m_program->link();

     if(!success) {
            qDebug() << "shaderProgram link failed!" << m_program->log();
        }

     qDebug()<<"program link ok ";
     return true;

 }

