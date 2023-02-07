#ifndef TOKENI_H
#define TOKENI_H
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include "RTabela.h"
#include "TabelaSimbola.h"
#include "Instrukcija.h"

#include "Section.h"

using namespace std;
class Tokeni
{
public:
    Tokeni(ifstream &in, TabelaSimbola *ts, RelokTabele *rt, Section *s, ofstream &out);
    int locationcounter = 0;
    int poslinstrukcija = 0;
    TabelaSimbola *ts;

    static string tsekcija;
    static string tlabela;
    RelokTabele *rt;
    Section *s;

    Instrukcije instr = Instrukcije();
    void dodajOpKod(string icode);

    int stringToInt(string s);
    string intToBin(string number);
    int binToInt(string bin);

    bool obradiTokene(vector<string> tokens);
    void gurnisimbol(string token, bool skok = false, bool jedanbajt = false);

    bool testirajSkokove(string icode, string token);
    bool testirajPusheve(string icode, string token);

    void jmpLit(string token);
    void jmpSimb(string token);
    void jmpZvezdLit(string token);
    void jmpZvezdReg(string token);
    void jmpZvezdSimb(string token);
    void jmpProcSimb(string token);
    void jmpZvezdZagReg(string token);
    void jmpRegPlusLit(string token);
    void jmpRegPlusSimb(string token);

    void aritmetickaOperacija(string instr, string op1, string op2);
    void logickaOperacija(string instr, string op1, string op2);
    void pomerackaOperacija(string instr, string op1, string op2);

    void proveriOperand(string op, string regD);
};

#endif