/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this
 * template
 */

/*
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "loader/dataset.h"
#include "tensor/xtensor_lib.h"

using namespace std;

// Todo CODE YOUR
template<typename DType, typename LType>
class DataLoader{
public:
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    /*TODO: add more member variables to support the iteration*/
    public : class Iterator ;
    private :
    xt::xarray<int> indices; 
    int current_index;
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset,
            int batch_size,
            bool shuffle=true,
            bool drop_last=false,
            int seed = -1 ){
        /*TODO: Add your code to do the initialization */
    this->ptr_dataset = ptr_dataset;
    this->batch_size = batch_size;
    this->shuffle=shuffle;
    this->drop_last=drop_last;
    current_index = 0;
    if (ptr_dataset == nullptr) {
      throw std::out_of_range("Index is out of range!");
    }
    int len = ptr_dataset->len();
    indices = xt::arange<unsigned long>(len); 
    if (len == 0) {
      throw std::out_of_range("Index is out of range!");
    }
    if (shuffle) {
      if (seed >= 0){
      xt::random::seed(seed);
      xt::random::shuffle(indices);
    } else {
      xt::random::shuffle(indices);
    }
  }
    }
    virtual ~DataLoader(){}
    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
    
    /*TODO: Add your code here to support iteration on batch*/
    
    Iterator begin() {
    return Iterator(this, 0);
  }

  Iterator end() {
    int dataset_len = ptr_dataset->len();
    int total_batches = (dataset_len + batch_size ) / batch_size;
    if ((!drop_last ) ||  drop_last) {
      total_batches--; 
    }
    return Iterator(this, total_batches * batch_size);
  }
  void reset() {
    current_index = 0;
    if (shuffle) {
      xt::random::shuffle(indices); 
    }
  }

  int get_total_batch(){
      return  int (ptr_dataset->len()/batch_size);
  }
  class Iterator {
  private:
    DataLoader<DType, LType>* loader;
    int batch_start;
  public:
    Iterator(DataLoader<DType, LType>* loader, int batch_start)
        : loader(loader), batch_start(batch_start) {}

    Iterator& operator=(const Iterator& iterator) {
      if (this != &iterator) {
        this->loader = iterator.loader;
        this->batch_start = iterator.batch_start;
      }
      return *this;
    }

    Iterator& operator++() {
      batch_start += loader->batch_size;
      return *this;
      
    }

    Iterator operator++(int) {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator!=(const Iterator& other) const {
      return batch_start != other.batch_start;
    }

    Batch<DType, LType> operator*() const {
      int batch_end = std::min(batch_start + loader->batch_size, loader->ptr_dataset->len());
    if (batch_end == batch_start) {
        return Batch<DType, LType>(xt::xarray<DType>(), xt::xarray<LType>());
    }
    int remain = loader->ptr_dataset->len() - batch_start;
    if (!loader->drop_last && remain < 2 * loader->batch_size) {
        batch_end = loader->ptr_dataset->len();  
    }
    auto data_shape = loader->ptr_dataset->get_data_shape();
    data_shape[0] = batch_end - batch_start;
    xt::xarray<DType> batch_data = xt::empty<DType>(data_shape);
    xt::xarray<LType> batch_labels;

        auto label_shape = loader->ptr_dataset->get_label_shape();
        label_shape[0] = batch_end - batch_start;
        if (label_shape.size() == 1) {
            batch_labels = xt::empty<LType>({batch_end - batch_start});
        } else {
            batch_labels = xt::empty<LType>(label_shape);
        }
    
    
    for (int i = batch_start; i < batch_end; ++i) {
        int index = loader->indices(i);
        auto data_label = loader->ptr_dataset->getitem(index);
        xt::view(batch_data, i - batch_start) = data_label.getData(); 
  
        if (label_shape.size() > 0) {
            xt::view(batch_labels, i - batch_start) = data_label.getLabel(); 
        }
        else  batch_labels(i - batch_start) = data_label.getLabel()(0);
    }
   
        return Batch<DType, LType>(batch_data, batch_labels);
    }
  };
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};

#endif /* DATALOADER_H */