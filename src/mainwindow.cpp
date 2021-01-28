#include "mainwindow.h"
#include<QFile>
#include<QDockWidget>
#include"imageglwidget.h"
#include<QPalette>
#include<QPushButton>
#include <QGridLayout>
#include<QDebug>
#include<QSurfaceFormat>
#include <QLabel>
#include<QSlider>
#include"math.h"
#include<QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
 setWindowTitle(tr("Hello Triangle"));
 this->setMinimumSize(800,400);
 centralWidget = new QWidget(this);
 centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
 centreimgwidget=new ImageGLWidget(centralWidget);
 centreimgwidget->setGeometry(QRect(0, 0, 351, 251));
 QSurfaceFormat format;
 //设置像素采样的样本个数，用于抗锯齿
 format.setSamples(16);
 centreimgwidget->setFormat(format);
 this->setCentralWidget(centralWidget);
 createDockTool();
 this->resize(1000,500);
}

void MainWindow:: createDockTool()
{
    toolpanel=new QDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea,toolpanel);
    toolpanel->setWindowTitle("工具");
    toolpanel->setMinimumSize(QSize(200,400));
    toolpanel->setAllowedAreas(Qt::RightDockWidgetArea|Qt::LeftDockWidgetArea);
    toolpanel->setFeatures(QDockWidget::DockWidgetMovable);
    toolpanel->setAutoFillBackground(true);
    QPalette pal;
    pal.setColor(QPalette::Background,Qt::gray);
    toolpanel->setPalette(pal);
    dockWidgetContents=new QWidget;
    contwidget=new QWidget(dockWidgetContents);
    contwidget->setGeometry(QRect(25,28,100,400));

    QGridLayout *btnlayout=new QGridLayout(contwidget);

    QPushButton *btnopenfile=new QPushButton(contwidget);
    btnopenfile->setText("OpenFile");
    QPushButton *btngray=new QPushButton(contwidget);
    btngray->setText("Always");


    QPushButton *btnnever=new QPushButton(contwidget);
    btnnever->setText("Never");
    btnnever->hide();

    QPushButton *btnless=new QPushButton(contwidget);
    btnless->setText("Less");
    btnless->hide();

    QPushButton *btnequal=new QPushButton(contwidget);
    btnequal->setText("Equal");
    btnequal->hide();

    QPushButton *btnlequal=new QPushButton(contwidget);
    btnlequal->setText("LEqual");
    btnlequal->hide();
    QPushButton *btngreater=new QPushButton(contwidget);
    btngreater->setText("Greater");
    btngreater->hide();


    btnlayout->addWidget(btnopenfile,0,0);
    btnlayout->addWidget(btngray,1,0);
    btnlayout->addWidget(btnnever,2,0);
    btnlayout->addWidget(btnless,3,0);
    btnlayout->addWidget(btnequal,4,0);
    btnlayout->addWidget(btnlequal,5,0);
    btnlayout->addWidget(btngreater,6,0);

    contwidget->setLayout(btnlayout);
    toolpanel->setWidget(dockWidgetContents);
    connect(btnopenfile,&QPushButton::clicked,this,&MainWindow::btnopenfile_clicked);
    connect(btngray,&QPushButton::clicked,this,&MainWindow::btnalways_clicked);
    connect(btnnever,&QPushButton::clicked,this,&MainWindow::btnnever_clicked);
    connect(btnless,&QPushButton::clicked,this,&MainWindow::btnless_clicked);
    connect(btnlequal,&QPushButton::clicked,this,&MainWindow::btnlequal_clicked);
    connect(btngreater,&QPushButton::clicked,this,&MainWindow::btngreater_clicked);

}

MainWindow::~MainWindow()
{

}
 void MainWindow::resizeEvent(QResizeEvent* event)
 {
     centreimgwidget->setGeometry(QRect(0,0,centralWidget->width(),centralWidget->height()));
      QMainWindow::resizeEvent(event);

 }
 void MainWindow::btnopenfile_clicked(bool clicked)
 {
     Q_UNUSED(clicked);

     QString OpenFile, OpenFilePath;
     QImage image;
     //打开文件夹中的图片文件
     OpenFile = QFileDialog::getOpenFileName(this,
                                             "Open a fundus image",
                                             "",
                                             "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
     if( OpenFile != "" )
     {
         if( image.load(OpenFile) )
         {
             centreimgwidget->SetImage(image);
             centreimgwidget->update();
         }

     }

 }

  void MainWindow::btnalways_clicked(bool clicked)
  {
      Q_UNUSED(clicked);

      centreimgwidget->update();
  }

  void MainWindow::btnnever_clicked(bool clicked)
  {
      Q_UNUSED(clicked);

      centreimgwidget->update();
  }
  void MainWindow::btnless_clicked(bool clicked)
  {
      Q_UNUSED(clicked);

      centreimgwidget->update();
  }

  void MainWindow::btnequal_clicked(bool clicked)
  {
      Q_UNUSED(clicked);

      centreimgwidget->update();
  }
  void MainWindow::btnlequal_clicked(bool clicked)
  {
      Q_UNUSED(clicked);

  }

  void MainWindow::btngreater_clicked(bool clicked)
  {
      Q_UNUSED(clicked);

      centreimgwidget->update();
  }




