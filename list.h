#include <iostream>
using namespace std;


template <class  T>

class Node{
private:
    int id;
public:

    Node<T> * next;
    Node<T> * prev;
    T * data;

    Node<T>(T *value) {
        id  = 0;
        next = nullptr;
        prev = nullptr;
        data = value;
    }

    void setId(int id){
        this->id = id;
    }
    int getId(){
        return this->id;
    }

    void printNode(){
        if(this != nullptr){
            cout << "Node value: " <<this->data<<" ; Node id: " << this->getId()<<"; Address: "<< this<<endl;
        }else{
            cout<< "This node is NULL"<<endl;
        }
    }
};


template <class  T>

int compare(T first, T second){
    return first - second < 0 ? -1 :first - second == 0? 0 : 1;
}



template <class  T>
class linkedList {
private:
    int size;
    Node<T> *head;
    Node<T> *tail;
    int idQty;


public:


    linkedList<T>() {
        idQty = 1;
        size = 0;
        head = nullptr;
        tail = nullptr;
    }

    ~linkedList<T>() {
        this->clearList();
    }

    Node<T> *getHead() {
        return this->head;
    }
    Node<T> * getTail(){
        return this->tail;
    }


    int getSize() {
        return this->size;
    }

//
//    void merge(linkedList<T>* list){
//        this->size +=list->size;
//        if(this->tail){
//            this->tail->next = list->head;
//            if(list->head){
//                list->head->prev = this->tail;
//            }
//        }else{
//            this->tail = list->tail;
//            this->head = list->head;
//        }
//
//        list->size = 0;
//        list->head = nullptr;
//        list->tail = nullptr;
//        list->idQty = 1;
//    }


    void add_begin(T *value) {

        auto *newNode = new Node<T>(value);
        newNode->next = this->head;

        if (this->size == 0) {
            this->tail = newNode;
        } else {
            this->head->prev = newNode;
        }
        this->head = newNode;

        newNode->setId(this->idQty);
        this->idQty++;
        this->size++;
    }

    void push_tail(T* value) {
        auto *newNode = new Node<T>(value);
        newNode->prev = this->tail;


        if (size < 1) {
            this->head = newNode;
        } else {
            this->tail->next = newNode;
        }

        this->tail = newNode;

        newNode->setId(this->idQty);
        this->idQty++;
        this->size++;
    }

    Node<T> *searchNode(T* value) {
        auto *tmp = this->getHead();
        while (tmp != nullptr) {
            if (tmp->data == value) {
                return tmp;
            }
            tmp = tmp->next;
        }
        return nullptr;
    }

    void deleteLast() {
        this->tail = this->tail->prev;
        delete this->tail->next;
        this->tail->next = nullptr;
        this->size--;
    }

    void deleteFirst() {
        this->head = this->head->next;
        delete this->head->prev;
        this->head->prev = nullptr;
        this->size--;
    }

    void deleteNodeValue(T* value) {
        auto * temp = searchNode(value);
        if (temp) {
            if(temp == head){
                deleteFirst();
                return;
            }
            if(temp == tail){
                deleteLast();
                return;
            }
            temp->next->prev = temp->prev;
            temp->prev->next = temp->next;
            this->size--;

        } else {
            cout << "Failed: invalid value" << endl;
        }
    }

    void deleteNodeIndex(int index) {
        if (this->index(index)) {
            this->index(index)->next->prev = this->index(index)->prev;
            this->index(index)->prev->next = this->index(index)->next;
            this->size--;

        } else {
            cout << "Failed: invalid index" << endl;
        }
    }


    Node<T> *index(int i) {
        auto *tmp = this->getHead();
        for (int j = 0; j < this->size; j++) {
            if (j == i) {
                return tmp;
            }
            tmp = tmp->next;
        }
        return nullptr;
    }

    void newValue(int index, T *value) {
        if (this->index(index)) {
            this->index(index)->data = value;
            cout << "Success" << endl;
        } else {
            cout << "Failed: invalid index" << endl;
        }
    }



    void clearList() {
        while (size != 0) {
            if (this->size == 1) {
                delete this->head;
                this->head = nullptr;
                this->tail = nullptr;
                this->size = 0;
            } else {
                this->deleteFirst();
            }
        }
    }


    void printList() {
        auto *tmp = this->getHead();
        if (tmp == nullptr) {
            cout << "Empty list" << endl;
            return;
        }
        while (tmp != nullptr) {
            tmp->printNode();
            tmp = tmp->next;
        }
    }
    void orderedInsert(T * value) {
        if(!size){
            add_begin(value);
            return;
        }

        Node<T> *tmp = this->head;
        if (compare(*this->head->data, *value) >= 1) {
            this->add_begin(value);
            return;
        }
        auto *newNode = new Node(value);
        while(true){
            if (compare(*tmp->data, *value) >= 0) {
                newNode->prev = tmp->prev;
                auto tmp1 = tmp->prev;
                newNode->next = tmp;
                tmp->prev = newNode;
                tmp1->next = newNode;
                this->size++;
                newNode->setId(this->idQty);
                this->idQty++;
                return;
            }else{
                if(tmp->next){
                    tmp = tmp->next;
                }else{
                    this->push_tail(value);
                    return;
                }
            }
        }
    }


};

