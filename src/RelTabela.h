#include <string>
//#include "Simbol.h"
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class Relok
{

public:
  Relok(short pomeraj, string relokTip, int vrSimbola, string sekcija, int addend)
  {
    this->pomeraj = pomeraj;
    this->relokTip = relokTip;
    this->vrSimbola = vrSimbola;
    this->sekcija = sekcija;
    this->addend = addend;
    ispravljen=false;
  }
  bool ispravljen;
  short pomeraj;
  string relokTip;
  int vrSimbola;
  string sekcija;
  int addend;
};

class RelokTabela
{
public:
  RelokTabela(string section) { this->section = section; }
  vector<Relok> relocations;
  string section;
};

class RelokTabele
{
public:
RelokTabele(){};
 
  map<string, RelokTabela *> mapa;
  vector<string> sekcije;

  void dodaj(Relok *rel, string section)
  {
    (mapa.at(section)->relocations).push_back(*rel);
  }
};