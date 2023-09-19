#include "System.h"

System::System(){
    this->state = DiodeSelecting;
}
System::State System::getState(){
    return this->state;
}
void System::setState(State nextState){
    this->state = nextState;
}