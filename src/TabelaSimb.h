#include <map>
#include <string>
#include "SimbolLink.h"
#include <vector>
#include <iostream>

class TabelaSimbola
{
public:
    TabelaSimbola();

    vector<Simbol *> symbols;

    int counter = 0;
    int dodajSimbol(Simbol *s);
    bool dodajEksterni(Simbol *s);
    void dodajSimbolSim(Simbol *s);
    Simbol *dohvSimbolIME(string br);
    Simbol *dohvSimbol(int br);
    int dohvPomeraj(string br);
};