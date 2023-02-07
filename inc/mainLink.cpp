#include <fstream>
#include <iostream>
#include <string>
#include "Linker.h"
using namespace std;

int main(int argc, char *argv[])
{
  string mod = argv[1];
  string out = argv[3];
  Linker *linker = new Linker();

  int brFajl = argc;

  int i = 0;
  while (i <= argc - 5)
  {
    string in = argv[4 + i];
    // cout<<"in "<<4+i<<endl;
    linker->procitaj(in, i);
    i++;
  }

  linker->obrada(out, argc - 5);

  //}
  return 0;
}
