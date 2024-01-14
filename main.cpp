#include <iostream>
#include "array.h"
#include <math.h>
#include <fstream>
#include <chrono>

class Vertex{
public:
    double x;
    double y;

    Vertex() : x(0.0),y(0.0){};
    Vertex(double x, double y) : x(x), y(y){};

    string getVertexString(){
        return "( " + to_string(x) +", " + to_string(y)+" )";
    }
};

class Surface{
public:
    dynamicArray<Vertex> * vertices;
    int elements;

    Surface(): vertices(new dynamicArray<Vertex>()),elements(0){};

    void add(Vertex * vertex){
        vertices->add(*vertex);
        elements = vertices->length;
    }

    string getSurfaceString(){
        string result;
        result += "[";
        for(int i = 0; i<elements-1;i++){
            result+=vertices->index(i)->getVertexString() + ", ";
        }
        result+=vertices->index(elements-1)->getVertexString() + "]";
        return result;

    }

};
class Hull{
public:
    linkedList<Vertex> * hullVertices;
    Hull() : hullVertices(new linkedList<Vertex>()){};
    string getHullString(){
        string result = "[";
        auto tmp = hullVertices->getHead();
        while(tmp->next){
            result+=tmp->data->getVertexString() + ", ";
            tmp = tmp->next;
        }
        result+=tmp->data->getVertexString() + "]\n";
        return result;
    }
};
int compareGrahamScan(Vertex * a, Vertex* b, Vertex * c){
    double vectorsProduct = (b->y - a->y)*(c->x - b->x) - (b->x - a->x)*(c->y - b->y) ;

    if(vectorsProduct < 0){//counter-clockwise
        return 1;
    }
    if(vectorsProduct > 0){//clockwise
        return -1;
    }
    //collinear
    return 0;
}

template <class T>
void swapValues(T & value1, T & value2){
    auto temp =value1;
    value1 = value2;
    value2 = temp;
}


Vertex * searchMin(dynamicArray<Vertex> * vertices){
    Vertex * min = vertices->index(0);
    for(int i = 0; i < vertices->length;i++){
        auto temp = vertices->index(i);
        if(temp->y < min->y){
            min = temp;
        }else if(temp->y == min->y){
            if(temp->x < min->x){
                min = temp;
            }
        }
    }
    return min;
}


dynamicArray<Vertex> * sortVertices(Surface * surface){
    auto *vertices = new dynamicArray<Vertex>();
    for(int i = 0; i<surface->elements;i++){
        vertices->add(*surface->vertices->index(i));
    }
    auto min = searchMin(surface->vertices);
    swapValues(*min,*vertices->index(0));

    for(int i = 1; i< vertices->length; i++ ){
        for(int j = 1; j < vertices->length - i -1; j++){
            auto temp = vertices->index(j);
            auto temp1 = vertices->index(j+1);
            auto angleJ = atan2(temp->y - min->y, temp->x - min->x);
            auto angleJ1 = atan2(temp1->y - min->y, temp1->x - min->x);

            if(angleJ <angleJ1 ){
                swapValues(*temp,*temp1);
            }

        }
    }

    return vertices;
}
Hull* grahamScan(Surface * surface){
    auto start = chrono::high_resolution_clock::now();


    auto * hull = new Hull();
    auto min = searchMin(surface->vertices);

    auto sortStart = chrono::high_resolution_clock::now();

    auto vertices = sortVertices(surface);

    auto sortEnd = chrono::high_resolution_clock::now();
    auto sortDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(sortEnd - sortStart);
    printf("Sort %d elements: Time measured: %.3d nanoseconds.\n",surface->elements, sortDuration.count());


    auto mainStart = chrono::high_resolution_clock::now();

    hull->hullVertices->push_tail(min);
    hull->hullVertices->push_tail(vertices->index(1));

    for(int i = 2; i<surface->elements;i++){
        auto tempVertex = vertices->index(i);
        while((compareGrahamScan(hull->hullVertices->getTail()->prev->data,hull->hullVertices->getTail()->data,tempVertex) > 0) && hull->hullVertices->getSize() > 2){
            hull->hullVertices->deleteLast();
        }
        hull->hullVertices->push_tail(tempVertex);
    }
    while((compareGrahamScan(hull->hullVertices->getTail()->prev->data,hull->hullVertices->getTail()->data,min) > 0) && hull->hullVertices->getSize() > 2){
        hull->hullVertices->deleteLast();
    }

    auto mainEnd = chrono::high_resolution_clock::now();
    auto mainDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(mainEnd - mainStart);
    printf("Main loop: Time measured: %.3d nanoseconds.\n", mainDuration.count());

    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Search convex hull for %d elements: Time measured: %.3d nanoseconds.\n",surface->elements, duration.count());

    return hull;
}

Surface * readFromFile(string fileName){
    auto * surface = new Surface();
    ifstream inputFile("./"+fileName+".txt",ifstream::in);

    if (!inputFile.is_open()) {
        cerr << "Unable to open the file." << endl;
        return nullptr;
    }
    int numberOfVertex;
    inputFile>> numberOfVertex;
    for(int i = 0; i< numberOfVertex;i++){
        double x,y;
        inputFile >> x >> y;
        surface->add(new Vertex(x,y));
    }
    return surface;
}

void writeDataToFile(Surface * surface, Hull * hull){
    ofstream writeSurface("./surface.txt");
    if(writeSurface.is_open()){
        writeSurface << surface->getSurfaceString();
        writeSurface.close();
    }
    ofstream writeHull("./hull.txt");
    if(writeHull.is_open()){
        writeHull<<hull->getHullString();
        writeHull.close();
    }
}


int main() {
    Surface * surface = readFromFile("points2");
    auto hull = grahamScan(surface);
    cout<<"Elements: " << hull->hullVertices->getSize()<<endl;
    cout<<hull->getHullString();
    writeDataToFile(surface,hull);
    int drawHull = system("python draw.py");


    return 0;
}
