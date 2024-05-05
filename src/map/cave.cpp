#include "cave.hpp"

cave::cave()
{

}

void cave::initialize(){
    CaveMesh.initialize();
}

void cave::draw(environment_structure environment){
    CaveMesh.draw(environment);
}
