#include <iostream>
#include "TabelaSimb.h"
#include <map>
#include "RelTabela.h"
#include "SectionLink.h"
#include <fstream>
#include <string>
using namespace std;

class Linker
{

public:
  string mod;
  string napravistring(unsigned char second);

  TS tabSekOff ;

  TabelaSimbola *globalSectionList;

  TabelaSimbola *externSymbols;
  TabelaSimbola *globalListOfSymbols;

  map<int, RelokTabele> relTab;
  map<int, TabelaSimbola> simTab;
  map<int, Section> mapSek;

  int pronadjiOffEkst(string naz);
  

  string out;
  Linker();
  ~Linker();

void procitaj(string in,int index);

  void obrada(string out,int brFajlova);
};