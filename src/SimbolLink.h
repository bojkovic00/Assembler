

#include <string>
using namespace std;

class Simbol
{
public:
    Simbol(int id, string naziv, string sekcija, string seklab, int pomeraj, string lokalna, int velicina, string defined,int indexFajla);
    int broj;
    string naziv;
    string sekcija;
    string seklab;
    int pomeraj;
    string lokalna;
    int velicina;
    bool defined;
    int indexFajla;
};