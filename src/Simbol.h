#ifndef SIMBOL_H
#define SIMBOL_H
#include <queue>
#include <iterator>
#include <list>
#include <iostream>
#include <string>

using namespace std;

class Obracanje
{
public:
    int pozicije;

    bool velicine;
    bool skok;
    string sekc;
    Obracanje(int pozicije,  bool velicine, string sekc, bool skok)
    {
        this->pozicije = pozicije;
        this->velicine = velicine;
        this->sekc = sekc;
        this->skok = skok;
    }
};

class Simbol
{
public:
    Simbol(string naziv, string sekcija, string seklab, int pomeraj, string lokalna, int velicina, int broj = 0, bool ex = false);
    string naziv;
    bool definisan;
    int velicina;
    int broj;
    int pomeraj;
    bool jeEqu;
    string seklab;
    string sekcija;
    bool ex;
    string lokalna;
 
 int brojSekcije=0;

    vector<Obracanje> obracanja;

    friend ostream &operator<<(ostream &os, const Simbol s);
    void definisi();
};

#endif