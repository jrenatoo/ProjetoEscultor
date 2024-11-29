#include "sculptor.hpp"
#include <iostream>
#include <fstream>

int main(void){
    
    Sculptor trono(10,10,10);

    trono.setColor(0,0,1.0,1.0);
    trono.putBox(0,9,0,9,0,9);
    trono.cutBox(1,8,1,9,1,9);
    trono.writeOFF("trono.off");
    return 0;
}
