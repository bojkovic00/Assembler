#include "SectionLink.h"

void TS::dodajS(S s)
{

  for (int i = 0; i < sek.size(); i++)
  {
    if (sek[i].ime == s.ime)
    {
      vector<S> sekNovo = {};
      for (int k = 0; k <= i; k++)
      {
        sekNovo.push_back(sek[k]);
      }

      s.offset = sek[i].offset + sek[i].size;
      sekNovo.push_back(s);
      for (int j = i + 1; j < sek.size(); j++)
      {
        sek[j].offset += s.size;
        sekNovo.push_back(sek[j]);
      }
      sek = sekNovo;
      return;
    }
  }

  if (sek.size() == 0)
  {
    s.offset = 0;
    sek.push_back(s);
  }
  else
  {
    s.offset = sek[sek.size() - 1].offset + sek[sek.size() - 1].size;
    sek.push_back(s);
  }
}
int TS::dohvOffset(string imeSek, int index)
{
  for (S ss : sek)
  {
    if (ss.ime == imeSek && ss.indexFajla == index)
    {
      return ss.offset;
    }
  }
  return 0;
}

int TS::dohvSize(string imeSek, int index)
{
  for (S ss : sek)
  {
    if (ss.ime == imeSek && ss.indexFajla == index)
    {
      return ss.size;
    }
  }
  return 0;
}

int TS::dohvPomerajUnazad(string imeSek)
{

  for (S ss : sek)
  {
    if (ss.ime == imeSek)
    {
      return ss.offset;
    }
  }
  return 0;
}

int Sek::dodajBajt(char val)
{
  data.insert(std::pair<int, char>(counter, val));
  counter++;
  return counter - 1;
}

void Sek::promeniBajt(int position, char val)
{

  data[position] = val;
}

unsigned char Sek::dohvatiBajt(int position)
{
  return data.at(position);
}

int Sek::dodajDvaBajta(short val)
{
  int pom = counter;
  char MASK = 255;

  unsigned char value = (unsigned char)val & MASK;
  data.insert(std::pair<int, char>(counter, value));
  counter++;
  value = (val >> 8) & MASK;
  data.insert(std::pair<int, char>(counter, value));
  counter++;
  return pom;
}

void Sek::promeniDvaBajta(int position, short val)
{
  unsigned short stvarnavrednost = 0;
  stvarnavrednost <<= 8;
  stvarnavrednost += 0;
  short vr = stvarnavrednost;
  vr += val;

  char MASK = 255;
  unsigned char value = (unsigned char)(vr & MASK); // donji bajt
  data[position] = value;
  value = (vr >> 8) & MASK; // gornji bajt
  data[position + 1] = value;
}

Sek *Section::dohvSekciju(string naziv)
{

  for (Sek *s : sekcije)
  {
    if (s->name == naziv)
      return s;
  }
  return 0;
}
