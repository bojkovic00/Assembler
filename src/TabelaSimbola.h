#ifndef TABELASIMBOLA_H
#define TABELASIMBOLA_H
#include <map>
#include <string>
#include "Simbol.h"
#include "Section.h"
#include "RTabela.h"

using namespace std;
class TabelaSimbola
{
public:
    TabelaSimbola();

    map<string, Simbol> symbols;
    int counter;

    bool dodajNoviSimbol(Simbol &s);

    Simbol *dohvatiSimbol(string naziv);
    int dohvatiVrednost(Simbol &s, string sekcija);
    int dohvatiBrojSekc(string sekcija);

    bool definisiSekciju(Simbol &s, string starasekcija, int slc);
    bool definisiSimbol(Simbol &s, Section *sekcija, RelokTabele *rt);
    bool definisiSimbol(Simbol &s);

    friend ostream &operator<<(ostream &os, const TabelaSimbola &st);

    void obelezi(string naziv, int gde, bool jedanbajt, string sekc, bool skok = false);

private:
};

#endif