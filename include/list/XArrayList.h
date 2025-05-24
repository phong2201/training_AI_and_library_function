/*
 * File:   XArrayList.h
 */

#ifndef XARRAYLIST_H
#define XARRAYLIST_H
#include "list/IList.h"
#include <memory.h>
#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class XArrayList : public IList<T> {
 public:
  class Iterator;  // forward declaration

 protected:
  T *data;
  int capacity;
  int count;
  bool (*itemEqual)(T &lhs, T &rhs);
  void (*deleteUserData)(XArrayList<T> *);

 public:
  XArrayList(void (*deleteUserData)(XArrayList<T> *) = 0,
             bool (*itemEqual)(T &, T &) = 0, int capacity = 10);
  XArrayList(const XArrayList<T> &list);
  XArrayList<T> &operator=(const XArrayList<T> &list);
  ~XArrayList();

  // Inherit from IList: BEGIN
  void add(T e);
  void add(int index, T e);
  T removeAt(int index);
  bool removeItem(T item, void (*removeItemData)(T) = 0);
  bool empty();
  int size();
  void clear();
  T &get(int index);
  int indexOf(T item);
  bool contains(T item);
  string toString(string (*item2str)(T &) = 0);
  // Inherit from IList: BEGIN

  void println(string (*item2str)(T &) = 0) {
    cout << toString(item2str) << endl;
  }
  void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0) {
    this->deleteUserData = deleteUserData;
  }

  Iterator begin() { return Iterator(this, 0); }
  Iterator end() { return Iterator(this, count); }

 protected:
  void checkIndex(int index);      // check validity of index for accessing
  void ensureCapacity(int index);  // auto-allocate if needed
  void copyFrom(const XArrayList<T> &list);
  void removeInternalData();

  //! FUNTION STATIC
 protected:
  static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
    if (itemEqual == 0)
      return lhs == rhs;
    else
      return itemEqual(lhs, rhs);
  }

 public:
  static void free(XArrayList<T> *list) {
    typename XArrayList<T>::Iterator it = list->begin();
    while (it != list->end()) {
      delete *it;
      it++;
    }
  }

 public:
  class Iterator {
   private:
    int cursor;
    XArrayList<T> *pList;

   public:
    Iterator(XArrayList<T> *pList = 0, int index = 0) {
      this->pList = pList;
      this->cursor = index;
    }
    Iterator &operator=(const Iterator &iterator) {
      cursor = iterator.cursor;
      pList = iterator.pList;
      return *this;
    }
    void remove(void (*removeItemData)(T) = 0) {
      T item = pList->removeAt(cursor);
      if (removeItemData != 0) removeItemData(item);
      cursor -= 1;  // MUST keep index of previous, for ++ later
    }

    T &operator*() { return pList->data[cursor]; }
    bool operator!=(const Iterator &iterator) {
      return cursor != iterator.cursor;
    }
    // Prefix ++ overload
    Iterator &operator++() {
      this->cursor++;
      return *this;
    }
    // Postfix ++ overload
    Iterator operator++(int) {
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }
  };
};

//! ////////////////////////////////////////////////////////////////////
//! //////////////////////     METHOD DEFNITION      ///////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
XArrayList<T>::XArrayList(void (*deleteUserData)(XArrayList<T> *),
                          bool (*itemEqual)(T &, T &), int capacity) {
  // TODO implement
  this->deleteUserData = deleteUserData;
  this->itemEqual = itemEqual;
  this->capacity = capacity;
  this->count = 0;
  this->data = new T[capacity];
}

template <class T>
XArrayList<T>::XArrayList(const XArrayList<T> &list) {
  // TODO implement
  this->deleteUserData = list.deleteUserData;
  this->itemEqual = list.itemEqual;
  this->capacity = list.capacity; 
  this->count = list.count;
  this->data = new T[capacity];
  for (int i = 0 ; i < list.count ; i++)
  {
      this->data[i] = list.data[i];
  }
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list) {
   if (this != &list) {  
    removeInternalData();  
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    this->capacity = list.capacity;
    this->count = list.count;
    
    this->data = new T[capacity];  
    for (int i = 0; i < list.count; i++) {
      this->data[i] = list.data[i];
    }
  }
  return *this;
}

template <class T>
XArrayList<T>::~XArrayList() {
  // TODO implement
  if (deleteUserData) {
        deleteUserData(this);  
    }
    delete[] data; 
    count = 0 ; 
}

template <class T>
void XArrayList<T>::add(T e) {
  // TODO implement
  if (this->count == capacity) {
    ensureCapacity(this->capacity);  
  }
  this->data[this->count] = e;  
  this->count++;  
}


template <class T>
void XArrayList<T>::add(int index, T e) {
  // TODO implement
 if (index < 0 || index > this->count) {
    throw std::out_of_range("Index is out of range!");
  }
  if (this->count == this->capacity) {
    ensureCapacity(this->capacity);
  }
  for (int i = this->count; i > index; i--) {
    this->data[i] = this->data[i - 1];  
  }
  this->data[index] = e;
  this->count++;
}

template <class T>
T XArrayList<T>::removeAt(int index) {
  // TODO implement
  if (index < 0 || index >= this->count)
  {
    throw std::out_of_range("Index is out of range!");
  }
  if (count == 0 ) {
    throw std::out_of_range("Index is out of range!");
  }
  T del = this->data[index];
  for (int i = index ; i < this->count -1 ; i++)
  {
    this->data[i] = this->data[i+1];
  }
  this->count --;
  return del;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T)) {

  for (int i = 0; i < this->count;) {
    if (equals(this->data[i], item, this->itemEqual)) {
      
      if (removeItemData != nullptr) {
        removeItemData(this->data[i]);  
      }
      removeAt(i);  
      return true;
    } else {
      i++; 
    }
  }
  return false;
}

template <class T>
bool XArrayList<T>::empty() {
  // TODO implement
  return this->count == 0;
}

template <class T>
int XArrayList<T>::size() {
  // TODO implement
  return this->count;
}

template <class T>
void XArrayList<T>::clear() {
  // TODO implement
  if (deleteUserData != nullptr) {
      deleteUserData(this);
  }
  this->count = 0;
}

template <class T>
T &XArrayList<T>::get(int index) {
  // TODO implement
    if (index < 0 || index >= this->count) 
    {
      throw std::out_of_range("Index is out of range!"); 
    }
    return this->data[index];
}

template <class T>
int XArrayList<T>::indexOf(T item) {
  // TODO implement
  for (int i = 0; i < this->count; i++) 
  {
    if (equals(this->data[i], item, this->itemEqual))
    {
      return i;
    }
  }
  return -1;
}

template <class T>
bool XArrayList<T>::contains(T item) {
  // TODO implement
  for (int i = 0; i < this->count; i++) 
  {
    if (equals(this->data[i], item, this->itemEqual))
    {
      return true;
    }
  }
  return false;
}

template <class T>
string XArrayList<T>::toString(string (*item2str)(T &))  {
  // TODO implement
  stringstream ss;
  ss << "[";
  for (int i = 0; i < this->count; i++) {
    if (item2str != nullptr) {
      ss << item2str(this->data[i]);
    } else {
      ss << this->data[i];  
    }
    if (i != this->count - 1) {
      ss << ", "; 
    }
  }
  ss << "]";
  return ss.str(); 
}

//! ////////////////////////////////////////////////////////////////////
//! ////////////////////// (private) METHOD DEFNITION //////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
void XArrayList<T>::checkIndex(int index) {
  /**
   * Validates whether the given index is within the valid range of the list.
   * Throws an std::out_of_range exception if the index is negative or exceeds
   * the number of elements. Ensures safe access to the list's elements by
   * preventing invalid index operations.
   */
  // TODO implement
  if (index < 0 || index >= count) 
  {
    throw std::out_of_range("Index is out of range!");
  }
}


template <class T>
void XArrayList<T>::ensureCapacity(int index) {
  /**
   * Ensures that the list has enough capacity to accommodate the given index.
   * If the index is out of range, it throws an std::out_of_range exception. If
   * the index exceeds the current capacity, reallocates the internal array with
   * increased capacity, copying the existing elements to the new array. In case
   * of memory allocation failure, catches std::bad_alloc.
   */
  // TODO implement
  if (index < 0) {
    throw std::out_of_range("Index is out of range!");
  }
  if (index < capacity) return;
    capacity *= 2;
  try {
    T* newData = new T[++capacity];
    if ( data != nullptr )
    {
    for (int i = 0; i < count; i++) {
      newData[i] = data[i];
    }
    }
    delete[] data;
    data = newData;
  } catch (const std::bad_alloc&) {
    throw std::runtime_error("Memory allocation failed: ");
  }
}

template <class T>
void XArrayList<T>::copyFrom(const XArrayList<T> &list) {
  /*
   * Copies the contents of another XArrayList into this list.
   * Initializes the list with the same capacity as the source list and copies
   * all elements. Also duplicates user-defined comparison and deletion
   * functions, if applicable.
   */
  // TODO implement
   if (this->data) {
    removeInternalData();
  }
  this->deleteUserData = list.deleteUserData;
  this->itemEqual = list.itemEqual;
  this->capacity = list.capacity;
  this->count = list.count; 
  this->data = new T[capacity];
  for (int i = 0; i < list.count; i++) {
    this->data[i] = list.data[i];
  }
}

template <class T>
void XArrayList<T>::removeInternalData() {
  /*
   * Clears the internal data of the list by deleting the dynamic array and any
   * user-defined data. If a custom deletion function is provided, it is used to
   * free the stored elements. Finally, the dynamic array itself is deallocated
   * from memory.
   */
  // TODO implement
    if (deleteUserData != nullptr) {
      deleteUserData(this);
    }
    count = 0;
    delete[] data;
}



#endif /* XARRAYLIST_H */
