#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class S
{
public:
  S(string ime, int offset, int size, int indexFajla)
  {
    this->ime = ime;
    this->offset = offset;
    this->size = size;
    this->indexFajla = indexFajla;
  }
  string ime;
  int offset;
  int size;
  int indexFajla;
};

class TS
{
public:
  TS(){};
  vector<S> sek = {};
  void dodajS(S s);

  int dohvOffset(string imeSek, int index);

  int dohvSize(string imeSek, int index);

  int dohvPomerajUnazad(string imeSek);
};

class Sek
{
public:
  Sek(string name, int offset, int size)
  {
    this->name = name;
    this->size = size;
    this->offset = offset;
  }
  string name;
  string sl;
  int size;
  int counter = 0;
  int offset;
  map<int, unsigned char> data;

  int dodajBajt(char val);

  void promeniBajt(int position, char val);

  unsigned char dohvatiBajt(int position);

  int dodajDvaBajta(short val);

  void promeniDvaBajta(int position, short val);
};

class Section
{
public:
  Section(){};
  vector<Sek *> sekcije;
  vector<string> nazivi;

  Sek *dohvSekciju(string naziv);
};