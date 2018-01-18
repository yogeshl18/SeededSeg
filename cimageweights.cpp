#include "cimageweights.h"
#include <iostream>

//Default Constructor
CImageWeights::CImageWeights()
{

}

/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	CImageWeights()

Purpose of Function		:   Parametric Construction of CImageWeights class
Modified by/Date		:

Calls                   :

Inputs                  :  cv::Rect roi1, cv::Rect roi2

Return                  :

Remarks                 :  Parametric constructer, which initializes the members of the class
                           and assignes the Image to be segmented and the User given mask to
                           respective variables.

****************************************************************************************************************************************
****************************************************************************************************************************************/

CImageWeights::CImageWeights(cv::Mat input1, cv::Mat input2, cv::Mat marker)
{
    img1 = input1;
    img2 = input2;
    mask = marker;
    index = cv::Mat(img1.rows,img1.cols,CV_32FC1,cv::Scalar::all(0)) ;

    for (int i=0;i<img1.rows;++i){
        for (int j=0; j<img1.cols;++j){
            index.at<float>(i,j) = ((j*img1.rows)+i);
        }
    }

     W = SpMat(img1.rows*img1.cols,img1.rows*img1.cols);
     D = SpMat(img1.rows*img1.cols,img1.rows*img1.cols);
     L = SpMat(img1.rows*img1.cols,img1.rows*img1.cols);
     I = SpMat(img1.rows*img1.cols,img1.rows*img1.cols);
     B = Eigen::VectorXd(img1.rows*img1.cols);
     seg1 = cv::Mat(img1.rows,img1.cols,CV_8UC3,cv::Scalar::all(0));
     seg2 = cv::Mat(img1.rows,img1.cols,CV_8UC3,cv::Scalar::all(0));
}

/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	Neighbourhood()

Purpose of Function		:   Method for neighborhood creation

Modified by/Date		:

Calls                   :

Inputs                  : cv::Rect roi1, cv::Rect roi2

Return                  :

Remarks                 :  Function that creates vectors containing the difference between neighbourhood
                           pixels and its index references according to the region of interests given.
****************************************************************************************************************************************
****************************************************************************************************************************************/



void CImageWeights::Neighbourhood(cv::Rect roi1, cv::Rect roi2)
{
    cv::Mat edge, edgem, channe[3], row, col;           //Declaring temporary variables

    edge = cv::abs(img1(roi1) - img1(roi2));
    cv::split(edge,channe);
    edgem = cv::max(channe[2],cv::max(channe[1],channe[0]));
    edgem = edgem.reshape(1,1);                         //Creating vector containing deifferences with neighbourhood

    index(roi1).copyTo(row);
    index(roi2).copyTo(col);
    row = row.reshape(1,1);                             //Creating Row reference to weight vector
    col = col.reshape(1,1);                             //Creating column reference to weight vector
    if (edges.empty()||r.empty()||c.empty())
    {
        edges = edgem;
        r = row;
        c = col;
    }
    else
    {
    cv::hconcat(edges,edgem,edges);
    cv::hconcat(r,row,r);
    cv::hconcat(c,col,c);
    }
}

/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	CreateL()

Purpose of Function		:   To create Laplacian Graph Matrix

Modified by/Date		:

Calls                   :

Inputs                  :

Return                  :

Remarks                 :   Function to create the Laplacian matrix after computing weights
****************************************************************************************************************************************
****************************************************************************************************************************************/


void CImageWeights::CreateL()
{
    int Beta = 130;                                     // Tuning parameter
    int Epsilon = 0.000001;
    edges = edges/cv::norm(edges,cv::NORM_INF);

    edges = -1*Beta*edges.mul(edges);                   //
    cv::exp(edges,edges);                               // Calculation of Weights
    cv::add(edges,cv::Scalar(Epsilon),edges);           //

     // Creating list of Triplets for sparse matrix creation
    for(int i=0; i<r.cols; ++i){
        tripleList.push_back(T(r.at<float>(0,i),c.at<float>(0,i),edges.at<float>(0,i)));
        tripleList.push_back(T(c.at<float>(0,i),r.at<float>(0,i),edges.at<float>(0,i)));
    }
    W.setFromTriplets(tripleList.begin(),tripleList.end());   // sparse weight matrix creation

    std::vector <float_t> d(img1.cols*img1.rows,0);

     // Finding the sum of neighbourhood weights
    for (int k=0; k < r.cols; ++k){
        d.at(r.at<float>(0,k)) = d.at(r.at<float>(0,k)) + edges.at<float>(0,k);
        d.at(c.at<float>(0,k)) = d.at(c.at<float>(0,k)) + edges.at<float>(0,k);
    }

    // Creating Diagonal weighted valency matrix
    for(int i=0; i<D.cols(); ++i){
        D.insert(i,i) = d.at(i);
    }

    L = D - W;

    L = L*L;

}


/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	CreateBandI()

Purpose of Function		:   To create I matrix which equals to I+L

Modified by/Date		:

Calls                   :

Inputs                  :

Return                  :

Remarks                 :  Function to create seed vector B and Diagonal matrix I
****************************************************************************************************************************************
****************************************************************************************************************************************/


void CImageWeights:: CreateBandI()
{
    cv::transpose(mask,mask);
    mask = mask.reshape(1,1);

    for (int j =0; j<mask.cols; ++j){
        B(j) = mask.at<float>(0,j);
        if(mask.at<float>(0,j) != 0)
            I.insert(j,j) = 1.0;
    }
    I = I+L;

    I.makeCompressed();
}


/***************************************************************************************************************************************
****************************************************************************************************************************************
Function Name			:	SolveSystem()

Purpose of Function		:   To solve linear system using Eigen Library:Cholesky Method

Modified by/Date		:

Calls                   :

Inputs                  :

Return                  :

Remarks                 :  Method for solving the sparse linear system of equation using solver from Eigen Library
                           The solution is compared and used for segmenting the image into two different parts
****************************************************************************************************************************************
****************************************************************************************************************************************/

void CImageWeights::SolveSystem()
{
    Eigen::SimplicialCholesky<SpMat,Eigen::Upper> seg;
    seg.compute(I);
    X = seg.solve(B);

    for(int i =0; i<img2.rows; ++i)
    {
        for (int j=0; j<img2.cols; ++j)
        {
            if(X((j*img2.rows)+i) > 0)
                seg1.at<cv::Vec3b>(i,j) = img2.at<cv::Vec3b>(i,j);
            else
                seg2.at<cv::Vec3b>(i,j) = img2.at<cv::Vec3b>(i,j);
        }
    }

}
