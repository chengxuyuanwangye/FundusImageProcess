#include "imageglwidget.h"
#include<QTimer>
#include<QMatrix4x4>
#include<QOpenGLShaderProgram>
#include<QOpenGLFunctions_3_3_Core>
#include<QOpenGLExtraFunctions>
#include<QOpenGLTexture>
#include<QOpenGLFramebufferObject>
#include<QObject>
#include<QDebug>
#include<iostream>
#include"shape.h"
#include"camera.h"
#include<QKeyEvent>
#include<QMouseEvent>
#include<QPainter>
#include<math.h>

ImageGLWidget::ImageGLWidget(QWidget *parent):
    QOpenGLWidget (parent),
    animateflag(false),
    firstMouse(true),
    leftbuttonpressed(false),
    deltaTime(0.0f),
    lastFrame(0.0f),
    m_frame(0),
    curdepthfunc(DEPTHFUNC::LESS)
{

    textureRect=nullptr;
    plane=nullptr;
    m_timer=new QTimer(this);
    m_timer->setInterval(60);
    m_camera=new Camera(QVector3D(0.0f,0.0f,3.0f));//3.0
    lastX = width() / 2.0f;
    lastY = height() / 2.0f;
    //test
    deltaTime=1.0f;
    setFocusPolicy(Qt::StrongFocus);

    m_vbo=nullptr;//顶点缓冲对象
    m_vao=nullptr;//顶点数组对象
    m_program=nullptr;
    connect(m_timer,&QTimer::timeout,this,&ImageGLWidget::timeoutFunc);

}


ImageGLWidget::~ImageGLWidget()
{
    makeCurrent();
    qDeleteAll(framevec);
    qDeleteAll(cubevec);
    qDeleteAll(grassvec);
    if(plane!=nullptr)
        delete plane;
    delete m_camera;
    delete sky;
    //  delete screenTexture;
    if(m_vbo!=nullptr)delete m_vbo;
    if(m_vao!=nullptr)delete m_vao;
    delete m_program;
    doneCurrent();

}

void ImageGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

}

void ImageGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(textureRect!=nullptr)
        textureRect->Render();
}

void ImageGLWidget::SetImage(QImage srcimg)
{

    m_srcImg=srcimg.copy(0,0,srcimg.width(),srcimg.height());

    makeCurrent();
    //TODO::重复打开操作时，要清除原来的实例，重新生成一个新的实例
    if(textureRect!=nullptr)
        delete  textureRect;

    textureRect=new Shape(this->width(),this->height());
    textureRect->SetTexture(&m_srcImg);

}


void ImageGLWidget::resizeGL(int width, int height)
{
    const qreal retinascale=devicePixelRatio();
    glViewport(0,0,width*retinascale,height*retinascale);

    QVector<Shape*>::iterator i;
    for(i=cubevec.begin();i!=cubevec.end();++i)
    {
        (*i)->Resize(width,height);
    }
    for(i=framevec.begin();i!=framevec.end();++i)
    {
        (*i)->Resize(width,height);
    }
    for(i=grassvec.begin();i!=grassvec.end();++i)
    {
        (*i)->Resize(width,height);
    }
}


void ImageGLWidget::StartAnimate(bool flag)
 {
    Q_UNUSED(flag);
    animateflag=!animateflag;

    if(animateflag)
    {
        qDebug()<<"start";
        m_timer->start();

    }
    else {
        m_timer->stop();
    }

 }

 void ImageGLWidget::timeoutFunc()
 {

 }


 void ImageGLWidget::keyPressEvent(QKeyEvent *event)
 {
     switch (event->key()) {
     case Qt::Key_W:
     {
         m_camera->ProcessKeyboard(FORWARD,deltaTime);
         break;
     }
     case Qt::Key_S:
     {
         m_camera->ProcessKeyboard(BACKWARD,deltaTime);
         break;
     }
     case Qt::Key_D:
     {
         m_camera->ProcessKeyboard(LEFT,deltaTime);
         break;
     }
     case Qt::Key_A:
     {
         m_camera->ProcessKeyboard(RIGHT,deltaTime);
         break;
     }
     case Qt::Key_Up:
     {
         m_camera->ProcessKeyboard(UP,deltaTime);
         break;
     }
     case Qt::Key_Down:
     {
         m_camera->ProcessKeyboard(DOWN,deltaTime);
         break;
     }
     default:
     {
         break;
     }
     }
     update();
 }

 void ImageGLWidget::keyReleaseEvent(QKeyEvent *event)
 {
     Q_UNUSED(event);
     qDebug()<<"keyrelease";
 }

 void ImageGLWidget::mousePressEvent(QMouseEvent *event)
 {
  if(Qt::LeftButton== event->button())
  {
      lastX =event->pos().x();
      lastY = event->pos().y();
      leftbuttonpressed=true;
      update();
  }
 }

 void ImageGLWidget::mouseReleaseEvent(QMouseEvent *event)
 {
      Q_UNUSED(event);
    leftbuttonpressed=false;
 }

 void ImageGLWidget::mouseMoveEvent(QMouseEvent *event)
 {
     if(leftbuttonpressed)
     {
         int xpos = event->pos().x();
         int ypos = event->pos().y();
         int xoffset = xpos - lastX;
         int yoffset =lastY - ypos;// reversed since y-coordinates go from bottom to top
         lastX= event->pos().x();
         lastY= event->pos().y();
         m_camera->ProcessMouseMovement(-1*xoffset, -yoffset);
         update();
     }

 }

 void ImageGLWidget::wheelEvent(QWheelEvent *event)
 {
     QPoint offset = event->angleDelta();
     m_camera->ProcessMouseScroll(offset.y()/20.0f);
     update();
 }


