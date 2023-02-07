#ifndef SECTION_H
#define SECTION_H
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <stdint.h>

#include "RTabela.h"

using namespace std;
class SectionTwo
{
public:
    string name;
    int size;
    int counter = 0;
    map<int, unsigned char> data;
    friend ostream &operator<<(ostream &os, const SectionTwo &s);
};

class Section
{

public:
    Section()
    {
        SectionTwo *s = new SectionTwo();
        s->name = ".UND";
        s->counter = counter;
        sekcije.push_back(s);
    }
    int dodajBajt(char val);
    int dodajDvaBajta(short val);

    void novaSekcija(string naziv)
    {
        SectionTwo *s = new SectionTwo();
        s->name = naziv;
        s->counter = counter;
        sekcije.push_back(s);
    }
    static string napravistring(unsigned char second);
    int counter = 0;
    vector<SectionTwo *> sekcije;

SectionTwo* dohvSekc(string name){
    for(SectionTwo* s:sekcije){
        if(s->name==name){return s;}

    }
    cout<<"NEMA SEKCIJE "<<name;
    return 0;
}

    friend ostream &operator<<(ostream &os, const Section &s);
};
#endif