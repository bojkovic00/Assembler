#include <fstream>
#include <iostream>
#include <string>
#include "Emulator.h"


int main(int argc, char* argv[]) {
   
 string in=argv[1];



 Emulator* emulator = new Emulator( );

    emulator->ucitaj(in);
    emulator->executeInstruction();
 //  emulator->ispisi();

  }