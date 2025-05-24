/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;


template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

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
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };

    class BWDIterator {
private:
    DLinkedList<T> *pList;  
    Node *pNode;          

public:
    BWDIterator(DLinkedList<T> *pList = nullptr, bool begin = true) {
        this->pList = pList;
        if (begin) {
            if (pList != nullptr)
                this->pNode = pList->tail->prev;  
            else
                pNode = nullptr;
        } else {
            this->pNode = (pList != nullptr) ? pList->head : nullptr;
        }
    }

    BWDIterator &operator=(const BWDIterator &bwditerator) {
        if (this != &bwditerator) {
            this->pNode = bwditerator.pNode;
            this->pList = bwditerator.pList;
        }
        return *this;
    }

    void remove(void (*removeItemData)(T) = nullptr) {
    Node *toDelete = pNode; 
    
    if (toDelete->prev != nullptr)
        toDelete->prev->next = toDelete->next;
    if (toDelete->next != nullptr)
        toDelete->next->prev = toDelete->prev;

    if (removeItemData != nullptr)
        removeItemData(toDelete->data);  
    pNode = pNode->next;
    delete toDelete;  

    if (pList != nullptr)
        pList->count--;  
}

    bool operator!=(const BWDIterator &other) const {
        return this->pNode != other.pNode;
    }
    T &operator*() {
        return pNode->data;
    }
    bool operator==(const BWDIterator &other) const {
        return pNode == other.pNode;
    }
    // Prefix -- overload 
    BWDIterator &operator--() {
            pNode = pNode->prev;  
        return *this;
    }
    // Postfix -- overload
    BWDIterator operator--(int) {
        BWDIterator temp = *this;
        --(*this);  
        return temp;
    }
    BWDIterator &operator++() {
            pNode = pNode->prev;  
        return *this;
    }
    // Postfix -- overload
    BWDIterator operator++(int) {
        BWDIterator temp = *this;
        ++(*this);  
        return temp;
    }
};

};

    template <class T>
    using List = DLinkedList<T>;

    template <class T>
    DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *),
                                bool (*itemEqual)(T &, T &)) {
    // TODO implement
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    this->head = new Node();
    this->tail = new Node(); 
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;
    }

    template <class T>
    DLinkedList<T>::DLinkedList(const DLinkedList<T> &list) {
    // TODO implement

        this->deleteUserData = list.deleteUserData;
        this->itemEqual = list.itemEqual;
        this->head = new Node();
        this->tail = new Node();
        this->head->next = this->tail;  
        this->tail->prev = this->head;
        this->count = 0;

        if (list.head->next == list.tail) {
            return;
        }
        Node* temp = list.head->next;
        while (temp != list.tail) {
            this->add(temp->data);
            temp = temp->next;
        }
};

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list) {
  // TODO implement
  if (this == &list) {
        return *this;
    }
    removeInternalData();
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    this->count = 0;
    Node* temp = list.head->next;
    while (temp != list.tail) {
        this->add(temp->data);
        temp = temp->next;
    }
    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList() {
    removeInternalData();
    delete head;
    delete tail;
}

template <class T>
void DLinkedList<T>::add(T e) {
  // TODO implement
  Node* newNode = new Node(e);
  if (this->head->next == this->tail) { 
        this->head->next = newNode;  
        this->tail->prev = newNode;  
        newNode->prev = this->head;  
        newNode->next = this->tail;  
    } 
    else { 
        newNode->prev = this->tail->prev; 
        newNode->next = this->tail;       
        this->tail->prev->next = newNode; 
        this->tail->prev = newNode;      
    }
    this->count++; 
}
template <class T>
void DLinkedList<T>::add(int index, T e) {
  // TODO implement
  if (index < 0 || index > count) {
        throw std::out_of_range("Index is out of range!");
    }
    Node* temp = new Node(e);
    if (index == count) {
        temp->prev = tail->prev;
        temp->next = tail;
        tail->prev->next = temp;
        tail->prev = temp;
    }
    else if (index == 0) {
        temp->next = head->next;
        temp->prev = head;
        head->next->prev = temp;
        head->next = temp;
    }
    else {
        Node* current = head->next;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        temp->next = current;
        temp->prev = current->prev;
        current->prev->next = temp;
        current->prev = temp;
    }
    this->count++;
}

template <class T>
T DLinkedList<T>::removeAt(int index) {
  // TODO implement
  if (index < 0 || index >= count) {
        throw std::out_of_range("Index is out of range!");
    }

    Node* remove = head->next;
    for (int i = 0; i < index; i++) {
        remove = remove->next;
    }

    T value = remove->data;
    remove->prev->next = remove->next;
    remove->next->prev = remove->prev;

    delete remove;
    count--;
    return value;
}

template <class T>
bool DLinkedList<T>::empty() {
  // TODO implement
  return count == 0;
}

template <class T>
int DLinkedList<T>::size() {
  // TODO implement
  return count;
}

template <class T>
void DLinkedList<T>::clear() {
  // TODO implement
   if (deleteUserData) {
            deleteUserData(this);
    }
     Node* current = head->next;
    while (current != tail) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index) {
  // TODO implement
  if (index < 0 || index >= count) {
     throw std::out_of_range("Index is out of range!");
  }
  Node* value = head->next ;
  for (int i = 0 ; i < index ; i++)
  {
      value = value->next;
  }
  return value->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item) {
  // TODO implement
  Node* current = head->next;
  int index = 0;
  while (current != tail)
  {
    if (equals(current->data, item, this->itemEqual)) 
    {
      return index;
    }
    current = current->next;
    index++;
  }
  return -1; 
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)) {
  // TODO implement
  Node* temp = head->next;
  int value = 0;
  while (temp != tail) 
  {
    if (equals(temp->data, item, this->itemEqual))
    {
      
      if (removeItemData != 0)  removeItemData(temp->data);
      removeAt(value);
      return true;                                      
    }
    else { temp = temp->next; value ++ ;}
  }
  return false;
}

template <class T>
bool DLinkedList<T>::contains(T item) {
  // TODO implement
  if (indexOf(item) == -1 ) return false ;
  return true;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &)) {
    stringstream ss;
    ss << "[";
    Node* temp = head->next;  
    bool firstItem = true;    
    while (temp != tail) {    
        if (!firstItem) {
            ss << ", ";  
        }
        if (item2str != nullptr) {
            ss << item2str(temp->data);  
        } else {
            ss << temp->data;  
        }
        firstItem = false;  
        temp = temp->next;
    }
    ss << "]";
    return ss.str();  
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list) {
  // TODO implement
  if (this == &list) return;
    removeInternalData();
    Node* current = list.head->next;
    while (current != list.tail) {
        add(current->data); 
        current = current->next;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData() {

  // TODO implement
  Node* current = head->next;
  if (deleteUserData) {
            deleteUserData(this);
  }
    while (current != tail) {
        Node* next = current->next; 
        delete current;
        current = next;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index) {
  if (index <= 0 || index >= count) {
    throw std::out_of_range("Index is out of range!");
  }
  Node* current;
  if (index <= count / 2) {
    current = head->next;  
    for (int i = 0; i < index - 1; ++i) {
      current = current->next;
    }
  } 
  else {
    current = tail->prev;  
    for (int i = count - 1; i > index; --i) {
      current = current->prev;
    }
  }
  return current;
}

#endif /* DLINKEDLIST_H */
