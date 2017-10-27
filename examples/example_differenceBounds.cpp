/**
 * \example example_differenceBounds.cpp This example shows how to use a representation by difference Bounds.
 */


#include "representations/GeometricObject.h"
#include "datastructures/Halfspace.h"
#include "datastructures/Point.h"
#include "util/Plotter.h"

typedef double Number;
hypro::DifferenceBounds <Number> testDBM;
void createTestDBM();
void testVertices();
void testElapse();
void testRewind();
void testFree();
void testReset();
void testPrint();

int main() {
    createTestDBM();
    testVertices();
    //testElapse();
    //testRewind();
    //testFree();
    testReset();
    testPrint();
    return 0;
}

void createTestDBM(){
    hypro::matrix_t<std::pair<Number, hypro::DifferenceBounds<Number>::BOUND_TYPE >> mat = hypro::matrix_t<std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>>(3,3);
    mat << std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(0,hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ),
            std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(-4.0,hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ),
            std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(-4.0,hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ),
            std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(10.0,hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ),
            std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(0.0,hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ),
            std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(0.0, hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ),
            std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(11.0,hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ),
            std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(2.0,hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ),
            std::pair<Number,hypro::DifferenceBounds<Number>::BOUND_TYPE>(0.0,hypro::DifferenceBounds<Number>::BOUND_TYPE::SMALLER_EQ);
    testDBM = hypro::DifferenceBounds<Number>();
    testDBM.setDBM(mat);
}

void testVertices(){
    // compute vertices
    std::cout<< "Test instance vertices: \n";
    std::vector<hypro::Point<Number>> verts = testDBM.vertices();
    std::cout<< verts;

    //plot to pdf
    hypro::Plotter<Number>::getInstance().clear();
    unsigned obj2 = hypro::Plotter<Number>::getInstance().addObject(verts);
    hypro::Plotter<Number>::getInstance().setObjectColor(obj2, hypro::plotting::colors[hypro::plotting::green]);

    // create a *.plt file (gnuplot).
    hypro::Plotter<Number>::getInstance().setFilename("verticesTest");
    hypro::Plotter<Number>::getInstance().plot2d();
}

void testElapse(){
    // compute vertices
    std::cout<< "Test elapse: \n";

    hypro::DifferenceBounds <Number> elapsed = testDBM.elapse();
    elapsed.setTimeHorizon(20.0);
    std::cout<< elapsed;
    std::vector<hypro::Point<Number>> verts = elapsed.vertices();
    std::cout<< verts;

    //plot to pdf
    hypro::Plotter<Number>::getInstance().clear();
    unsigned obj2 = hypro::Plotter<Number>::getInstance().addObject(verts);
    hypro::Plotter<Number>::getInstance().setObjectColor(obj2, hypro::plotting::colors[hypro::plotting::green]);

    // create a *.plt file (gnuplot).
    hypro::Plotter<Number>::getInstance().setFilename("elapseTest");
    hypro::Plotter<Number>::getInstance().plot2d();
}

void testRewind(){
    // compute vertices
    std::cout<< "Test rewind: \n";

    hypro::DifferenceBounds <Number> rewind = testDBM.rewind();
    rewind.setTimeHorizon(20.0);
    std::cout<< rewind;
    std::vector<hypro::Point<Number>> verts = rewind.vertices();
    std::cout<< verts;

    //plot to pdf
    hypro::Plotter<Number>::getInstance().clear();
    unsigned obj2 = hypro::Plotter<Number>::getInstance().addObject(verts);
    hypro::Plotter<Number>::getInstance().setObjectColor(obj2, hypro::plotting::colors[hypro::plotting::green]);

    // create a *.plt file (gnuplot).
    hypro::Plotter<Number>::getInstance().setFilename("rewindTest");
    hypro::Plotter<Number>::getInstance().plot2d();
}

void testFree(){
    // compute vertices
    std::cout<< "Test free: \n";

    hypro::DifferenceBounds <Number> freed = testDBM.free(2);//free y
    freed.setTimeHorizon(20.0);
    std::cout<< freed;
    std::vector<hypro::Point<Number>> verts = freed.vertices();
    std::cout<< verts;

    //plot to pdf
    hypro::Plotter<Number>::getInstance().clear();
    unsigned obj2 = hypro::Plotter<Number>::getInstance().addObject(verts);
    hypro::Plotter<Number>::getInstance().setObjectColor(obj2, hypro::plotting::colors[hypro::plotting::green]);

    // create a *.plt file (gnuplot).
    hypro::Plotter<Number>::getInstance().setFilename("freeTest");
    hypro::Plotter<Number>::getInstance().plot2d();
}

void testReset(){
    // compute vertices
    std::cout<< "Test reset: \n";

    hypro::DifferenceBounds <Number> reset = testDBM.reset(1,0.0);//reset x
    reset.setTimeHorizon(20.0);
    std::cout<< reset;
    std::vector<hypro::Point<Number>> verts = reset.vertices();
    std::cout<< verts;

    //plot to pdf
    hypro::Plotter<Number>::getInstance().clear();
    unsigned obj2 = hypro::Plotter<Number>::getInstance().addObject(verts);
    hypro::Plotter<Number>::getInstance().setObjectColor(obj2, hypro::plotting::colors[hypro::plotting::green]);

    // create a *.plt file (gnuplot).
    hypro::Plotter<Number>::getInstance().setFilename("resetTest");
    hypro::Plotter<Number>::getInstance().plot2d();
}

void testPrint(){
    std::cout<< "Test instance DBM: \n";
    std::cout<<testDBM;
}
