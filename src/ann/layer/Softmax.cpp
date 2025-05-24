/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   Softmax.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"

#include <filesystem>  //require C++17

#include "ann/functions.h"
#include "sformat/fmt_lib.h"
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {}

Softmax::~Softmax() {}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
  // Todo CODE YOUR
  m_aCached_Y = softmax(X , m_nAxis);
  return m_aCached_Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
  // Todo CODE YOUR
  
  xt::xarray<double> DZ = xt::zeros_like(DY);

    for (std::size_t i = 0; i < DY.shape()[0]; ++i) {
        xt::xarray<double> dy = xt::view(DY, i);          
        xt::xarray<double> y = xt::view(m_aCached_Y, i);  
        xt::xarray<double> diag_y = xt::diag(y);
        xt::xarray<double> outer_y = xt::linalg::outer(y, y);
        xt::xarray<double> dz = xt::linalg::dot((diag_y - outer_y), dy);
        xt::view(DZ, i) = dz;
    }
    return DZ;
}

string Softmax::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}: {:4d}", "Softmax",
                            this->getname(), m_nAxis);
  return desc;
}
