#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cimageweights.h"
#include <QFileDialog>
#include <QImage>
#include <iostream>
#include <QPainter>
#include <QMouseEvent>
#include <qevent.h>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QColorDialog>
#include "Eigen/Cholesky"
#include "QElapsedTimer"
#include "Eigen/SparseCholesky"
#include "Eigen/Sparse"
using namespace std;
using namespace cv;

/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	MainWindow :: MainWindow

Purpose of Function		:   Contrusctor to initialize GUI

Calls				:

Inputs				:

Return				:

Remarks				:
****************************************************************************************************************************************
****************************************************************************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    colorFlag=0;

    colortoset=Qt::white;
    MousePressed = false;
    ui->colorPushB->hide();
    ui->resetPushB->hide();
    ui->runPushB->hide();
}

/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	MainWindow ::~MainWindow()

Purpose of Function		:   Destructor of MainWindow class


Calls				:

Inputs				:

Return				:

Remarks				:
****************************************************************************************************************************************
****************************************************************************************************************************************/

MainWindow::~MainWindow()
{
    delete ui;

}

/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	on_brwsePushB_clicked()

Purpose of Function		:   Method for callback of browse button clicked

Calls                   :

Inputs                  :

Return                  :

Remarks                 : Used to browse image from the directory for the input
****************************************************************************************************************************************
****************************************************************************************************************************************/


void MainWindow::on_brwsePushB_clicked()
{
    QDir currDirName;

    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), currDirName.dirName(), tr("Image Files (*.png *.jpg *.bmp)"));


    inputImage = cv::imread(fileName.toStdString().c_str());
    this->resize(inputImage.cols,inputImage.rows+60);
    ui->brwsePushB->setGeometry(0+inputImage.cols/2-121,inputImage.rows,121,30);
    ui->runPushB->setGeometry(0+inputImage.cols/2-121,inputImage.rows+ui->brwsePushB->height(),ui->runPushB->width(),ui->runPushB->height());
    ui->resetPushB->setGeometry(0+inputImage.cols/2-121+ui->runPushB->width(),inputImage.rows+ui->colorPushB->height(),ui->runPushB->width(),ui->runPushB->height());
    ui->colorPushB->setGeometry(0+inputImage.cols/2-121+ui->brwsePushB->width(),inputImage.rows,ui->runPushB->width(),ui->runPushB->height());
    thissetsimage.load(fileName);

    inLabel->setGeometry(0,0,inputImage.cols,inputImage.rows);
    myImage.load(fileName);
    inLabel->setPixmap(thissetsimage.scaled(inputImage.cols,inputImage.rows,Qt::KeepAspectRatio));

    inLabel->show();
    ui->label->setGeometry(0,0,inputImage.cols,inputImage.rows);
    imgPixmap =  QPixmap(inputImage.cols,inputImage.rows);
    forePixmap = QPixmap(inputImage.cols,inputImage.rows);
    imgPixmap.fill(Qt::transparent);
    forePixmap.fill(Qt::transparent);
    markerMask = Mat(inputImage.rows,inputImage.cols,CV_32FC1, Scalar(0.0));
    ui->resetPushB->show();
    ui->runPushB->show();
    ui->colorPushB->show();

}
/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	mouseMoveEvent()

Purpose of Function		:   Method for mouse event

Calls                   :   QPolyLine poly

Inputs                  :

Return                  :  poly

Remarks                 :  We are storing mouse coordinates in qpolyline obj poly
****************************************************************************************************************************************
****************************************************************************************************************************************/

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    poly.append(e->pos());
}
/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	mouseReleaseEvent()

Purpose of Function		:  Method for the release event of mouse

Calls                   : QPainter painter, QPixmap imgPixmap,forePixmap

Inputs                  :

Return                  :

Remarks                 : This function on mouse release event draws on the pixmap
                          for particular color
****************************************************************************************************************************************
****************************************************************************************************************************************/

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{


    if(!inputImage.empty())
    {   if(colortoset==Qt::white)
        {
            ui->label->raise();
            QPainter painter1(&imgPixmap);

            QPen myPen(colortoset);
            myPen.setWidth(7);
            painter1.setPen(myPen);
            painter1.drawPolyline(poly);
            ui->label->setPixmap(imgPixmap);

            poly.clear();
        }
        else
        {
            // ui->label->raise();
            QPainter painter1(&forePixmap);

            QPen myPen(colortoset);
            myPen.setWidth(7);
            painter1.setPen(myPen);
            painter1.drawPolyline(poly);
            ui->label->setPixmap(forePixmap);

            poly.clear();
        }
    }
}
/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	on_runPushB_clicked()

Purpose of Function		:   Method for callback of run button clicked

Modified by/Date		:

Calls                   :  CImageWeights, QPixmap

Inputs                  :

Return                  :

Remarks                 : This function will give the results for segmentation

****************************************************************************************************************************************
****************************************************************************************************************************************/

void MainWindow::on_runPushB_clicked()
{
    //QElapsedTimer timer;
    //qint64 nanoSec;
    //timer.start();

    //Here we convert pixmap to cv mat
    imgWrite= imgPixmap.toImage();
    imgWriteFore = forePixmap.toImage();
    maskedImage=cv::Mat(imgWrite.height(), imgWrite.width(),
                        CV_8UC4, imgWrite.bits(), imgWrite.bytesPerLine());
    ForeMaskedImage=cv::Mat(imgWriteFore.height(), imgWriteFore.width(),
                            CV_8UC4, imgWriteFore.bits(), imgWriteFore.bytesPerLine());
    Mat maskFchannel[4],maskBchannel[4];
    split(maskedImage,maskFchannel);
    split(ForeMaskedImage,maskBchannel);
    maskFchannel[0].convertTo(maskFchannel[0],CV_32FC1);
    maskBchannel[0].convertTo(maskBchannel[0],CV_32FC1);

    //MarkerMask store the mask of seeds
    for (int i=0;i<markerMask.rows;i++)

    {
        for (int j=0;j<markerMask.cols;j++)
        {
            if(maskFchannel[0].at<float>(i,j)!=0)
            {
                markerMask.at<float>(i,j)= -1.0;


            }
            if(maskBchannel[0].at<float>(i,j)!=0)

            {
                markerMask.at<float>(i,j)=1.0;

            }

        }
    }

    qDebug("Marker Mask done");
    //Following will normalize the image for R,G,B channels
    imgOrig = inputImage;
    cv::Mat channel[3];
    cv::split(inputImage,channel);
    channel[0].convertTo(channel[0],CV_32F);
    channel[1].convertTo(channel[1],CV_32F);
    channel[2].convertTo(channel[2],CV_32F);
    cv::normalize(channel[0],channel[0],0,1,cv::NORM_MINMAX);
    cv::normalize(channel[1],channel[1],0,1,cv::NORM_MINMAX);
    cv::normalize(channel[2],channel[2],0,1,cv::NORM_MINMAX);
    cv::merge(channel,3,inputImage);

    //Object instantiation of class CImageWeights to solve the linear system
    CImageWeights imageObj(inputImage, imgOrig, markerMask);
    Rect roi1(0,0,inputImage.cols-1,inputImage.rows-1);
    Rect roi2(1,1,inputImage.cols-1,inputImage.rows-1);
    imageObj.Neighbourhood(roi1,roi2);
    roi1 = Rect(1,0,inputImage.cols-1,inputImage.rows-1);
    roi2 = Rect(0,1,inputImage.cols-1,inputImage.rows-1);
    imageObj.Neighbourhood(roi1,roi2);
    roi1 = Rect(0,0,inputImage.cols-1,inputImage.rows);
    roi2 = Rect(1,0,inputImage.cols-1,inputImage.rows);
    imageObj.Neighbourhood(roi1,roi2);
    roi1 = Rect(0,0,inputImage.cols,inputImage.rows-1);
    roi2 = Rect(0,1,inputImage.cols,inputImage.rows-1);
    imageObj.Neighbourhood(roi1,roi2);
    imageObj.CreateL();
    imageObj.CreateBandI();
    imageObj.SolveSystem();

    namedWindow("Segement 1", 1);
    imshow("Segement 1",imageObj.seg1);
    namedWindow("Segement 2", 1);
    imshow("Segement 2",imageObj.seg2);
    waitKey();

   // nanoSec = timer.nsecsElapsed();
    //qDebug()<<"time"<<nanoSec;

    //ui->colorPushB->hide();
   // ui->runPushB->hide();



}

/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	on_colorPushB_clicked()

Purpose of Function		:   Method for callback of color button clicked

Modified by/Date		:

Calls                   :

Inputs                  :

Return                  :

Remarks                 : This function selects the Blue color for the either Foreground
                          or background
****************************************************************************************************************************************
****************************************************************************************************************************************/


void MainWindow::on_colorPushB_clicked()
{
    QColorDialog colorpal;
    colorFlag=1;
    //colortoset= colorpal.getColor(Qt::white);
   // colortoset = Qt::blue;

    if(colortoset==Qt::white)
    {
        colortoset = Qt::blue;
        ui->colorPushB->setText("Foreground Color");
    }
    else
    {
        colortoset = Qt::white;
        ui->colorPushB->setText("Background Color");
    }
    if(!inputImage.empty())
    {
        ui->colorPushB->show();
    }
    else
    {
        ui->colorPushB->hide();
    }

}

/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	on_resetPushB_clicked()

Purpose of Function		:   Method for callback of reset button clicked

Modified by/Date		:

Calls                   : QPixmap

Inputs                  :

Return                  :

Remarks                 : This function will clear all the pixmaps to redraw on it again.
****************************************************************************************************************************************
****************************************************************************************************************************************/

void MainWindow::on_resetPushB_clicked()
{
    imgPixmap.fill(Qt::transparent);
    forePixmap.fill(Qt::transparent);
       ui->label->setPixmap(imgPixmap);
         ui->label->setPixmap(forePixmap);
      colortoset=Qt::white;

     colorFlag=0;

     //ui->colorPushB->hide();

}
