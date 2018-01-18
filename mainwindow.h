/*---------------------------------------------------------------------------------------------
 * This class handles the creation, and maintenance of GUI handling functions. The member
 * variables are used to store pixel maps, masks and original image
 * -------------------------------------------------------------------------------------------*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFileDialog>
#include <QMainWindow>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include "Eigen/Eigen"
#include "Eigen/SparseCholesky"
#include "Eigen/Sparse"

namespace Ui {
class MainWindow;
}
using namespace cv;
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString  fileName;
    Mat markerMask,inputImage,imgOrig,imgGray;
    QPixmap imgPixmap;QPixmap forePixmap;
    QPixmap transPixmap;
    QPixmap thissetsimage;
    QLabel *inLabel=new QLabel(this);
    bool MousePressed;
    QPixmap p;

private slots:
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void on_brwsePushB_clicked();
    void on_colorPushB_clicked();
    void on_runPushB_clicked();
    void on_resetPushB_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat orgImg;
    QImage myImage;
    QLabel myImageLabel;
    QImage imgWrite,imgWriteFore;
    int colorFlag;
    cv::Mat maskedImage;
    cv::Mat ForeMaskedImage;
    QPolygon poly;
    QColor colortoset;
    void markerMaskMat(int,int);
};


#endif // MAINWINDOW_H
