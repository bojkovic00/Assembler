#include "TabelaSimb.h"

using namespace std;

TabelaSimbola::TabelaSimbola()
{
}

bool TabelaSimbola::dodajEksterni(Simbol *s)
{
    for (Simbol *sim : symbols)
    {
        if (sim->naziv == s->naziv && sim->lokalna == "global")
        {
            return true;
        }
    }
    cout << "NIJE DEFINISAN " << s->naziv;
    return false;
}

int TabelaSimbola::dodajSimbol(Simbol *s)
{

    for (Simbol *sim : symbols)
    {
        if (sim->naziv == s->naziv)
        {

            if (s->seklab == "label" && s->lokalna == "global" && sim->lokalna == "global")
            {
                cout << "Dvostruko definisan " << s->naziv << endl;
                return 0;
            }
        }
    }

    symbols.push_back(s);
    return 1;
}

// za obicne
void TabelaSimbola::dodajSimbolSim(Simbol *s)
{
    symbols.push_back(s);
}

Simbol *TabelaSimbola::dohvSimbolIME(string br)
{
    for (Simbol *s : symbols)
    {
        if (s->naziv == br)
            return s;
    }
    return 0;
}

Simbol *TabelaSimbola::dohvSimbol(int br)
{
    for (Simbol *s : symbols)
    {
        if (s->broj == br)
            return s;
    }
    return 0;
}

int TabelaSimbola::dohvPomeraj(string br)
{

    for (Simbol *s : symbols)
    {
        if (s->naziv == br)
            return s->pomeraj;
    }

    return 0;
}
