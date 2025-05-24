/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"

#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction) {}

CrossEntropy::CrossEntropy(const CrossEntropy& orig) : ILossLayer(orig) {}

CrossEntropy::~CrossEntropy() {}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t) {
  // Todo CODE YOUR
   double epsilon = 1e-7;
    int norm = X.shape()[0];
    m_aYtarget = X ;
    xt::xarray<double> logYpred = xt::log(X + epsilon);
    xt::xarray<double> R = -t * logYpred;

    // Áp dụng giảm nếu cần
    R = xt::sum(R, -1);
    xt::xarray<double> sum = xt::sum(R);
    m_aCached_Ypred = t;
    return (sum/norm)[0];
}
xt::xarray<double> CrossEntropy::backward() {
  // Todo CODE YOUR
  double epsilon = 1e-7;
  int norm = m_aYtarget.shape()[0];
  xt::xarray<double> gradient =  ( m_aCached_Ypred / ( m_aYtarget + epsilon) ) ;

  if (m_eReduction == REDUCE_MEAN)
  {
    return gradient*(-1.0/norm) ;
  }
  else return gradient ;
}