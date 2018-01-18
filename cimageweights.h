/*-----------------------------------------------------------------------------------
 This class contains the members and functions used for image processing tasks
 to preform Laplacian coordinates based segmentation using user given seeded points
------------------------------------------------------------------------------------*/

#ifndef CIMAGEWEIGHTS_H
#define CIMAGEWEIGHTS_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "Eigen/Sparse"
#include "Eigen/Eigen"
#include "Eigen/SparseCholesky"
#include "Eigen/Sparse"

class CImageWeights
{

private:
    typedef Eigen::SparseMatrix<double> SpMat;      // Typecasting of Sparse matrix datatype
    typedef Eigen::Triplet<double> T;               // Typecasting of Triplet list datatype
    std::vector<T> tripleList;
    cv::Mat img1,img2;                              // Variables to store original and normalized images
    cv::Mat index,mask;                             // index = index reference of pxels, mask - seeded mask input fro user
    cv::Mat edges,r,c;
    //cv::Mat seg1,seg2;
    SpMat W,D,L,I;                                  // Sparse matrices
    Eigen::VectorXd B,X;                            // B = Seed vector, X = Output vector

public:
    cv::Mat seg1,seg2;
    CImageWeights();
    CImageWeights(cv::Mat, cv::Mat, cv::Mat);

    void Neighbourhood(cv::Rect, cv::Rect);
    void CreateL();
    void CreateBandI();
    void SolveSystem();

};

#endif // CIMAGEWEIGHTS_H
