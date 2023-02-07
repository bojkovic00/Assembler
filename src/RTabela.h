#ifndef RTABELA_H
#define RTABELA_H
#include "Simbol.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class RelokZapis
{
public:
    RelokZapis(short pomeraj, string relokTip, Simbol &s, string sekcija,int addend);
    friend ostream &operator<<(ostream &os, const RelokZapis &r);
    Simbol s;
    short pomeraj;
    string relokTip;
    string sekcija;
    int addend;
};

class RelokTabela
{
public:
    RelokTabela(string section); 
    string section;
    vector<RelokZapis> relocations;
    friend ostream &operator<<(ostream &os, const RelokTabela &rt);
};

class RelokTabele
{
public:
    RelokTabele();
    map<string, RelokTabela *> mapa = {{".UND", new RelokTabela(".UND")}};
   
    string section = ".UND";
    vector<string> sekcije;

    void staviSekciju(string section);
    friend ostream &operator<<(ostream &os, const RelokTabele &rt);
    void ocisti(int broj, int ts,string sek);
    
    void dodaj(RelokZapis *rel);
};

#endif