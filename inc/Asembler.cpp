#include "Asembler.h"
#include "Tokeni.h"

#include "RTabela.h"
#include "TabelaSimbola.h"
#include "Section.h"

Asembler::Asembler(string in, string out)
{
    this->in = in;
    this->out = out;
}

void Asembler::generisiTokene()
{
    ifstream ins;
    ofstream outs;
    try
    {
        ins.open(in);
        outs.open(out);
        Section *s = new Section();
        RelokTabele *rt = new RelokTabele();
        TabelaSimbola *ts = new TabelaSimbola();
        Tokeni t = Tokeni(ins, ts, rt, s, outs);
        if (outs.is_open())
        {
            outs.close();
        }
        if (ins.is_open())
        {
            ins.close();
        }
        if (ts != nullptr)
            delete (ts);
        if (rt != nullptr)
            delete (rt);
        if (s != nullptr)
            delete (s);
    }
    catch (const ifstream::failure &e)
    {
        cout << "Greska pri otvaranju ili citanju iz fajla!";
    }
}