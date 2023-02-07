#include "RTabela.h"
#include <string>

RelokTabela::RelokTabela(string section)
{
    this->section = section;
}

RelokZapis::RelokZapis(short pomeraj, string relokTip, Simbol &s, string sekcija, int addend) : s(s)
{
    this->pomeraj = pomeraj;
   this->sekcija = sekcija;
    this->relokTip = relokTip;
    this->addend = addend;
}

ostream &operator<<(ostream &os, const RelokTabela &rt)
{
    if (rt.section == ".UND")
        os << "#.ret" << rt.section;
    else
        os << "#.ret." << rt.section << ":";
    if (rt.section == ".UND")
        os << ":";
    os << endl;
    os << "#ofset\t\ttip\t\t\tvr[" << rt.section;
    if (rt.section == ".UND")
        os << ":";
    os << "]"
       << "\t\t"
       << "addend" << endl;
    for (RelokZapis r : rt.relocations)
    {
        os << r << endl;
    }
    //   os << endl;
    return os;
}
ostream &operator<<(ostream &os, const RelokZapis &r)
{

    char str[10];
    sprintf(str, "%04X", r.pomeraj);
    string s(str);
   
    if (r.s.lokalna == "local" && r.s.seklab == "label")
    {
        os << s << "\t\t" << r.relokTip << "\t\t" << r.s.broj << "\t\t"
           << "[" << r.sekcija << "]"
           << "\t\t" << r.addend;
    }
    else
    {

        os << s << "\t\t" << r.relokTip << "\t\t" << r.s.broj << "\t\t"
           << "[" << r.sekcija << "]"
           << "\t\t" << r.addend;
    }

    return os;
}

ostream &operator<<(ostream &os, const RelokTabele &r)
{

    for (int i = 0; i < r.sekcije.size(); i++)
    {
        string pom = r.sekcije[i];
        os << *r.mapa.at(pom);
        os << "#kraj"
           << "\n";
    }

    return os;
}

void RelokTabele::ocisti(int broj, int ts,string sek)
{
    for (int i = 0; i < sekcije.size(); i++)
    {
        string pom = sekcije[i];

        RelokTabela *rtt = mapa.at(pom);

        for (int i = rtt->relocations.size() - 1; i >= 0; i--)
        {
            if (rtt->relocations[i].s.broj == broj)
            {
                rtt->relocations[i].s.broj = ts;
                rtt->relocations[i].sekcija=sek;
               
            }
        }
    }
}

RelokTabele::RelokTabele()
{
    sekcije.push_back(".UND");
}

void RelokTabele::staviSekciju(string section)
{
    this->section = section;
    mapa.insert(std::pair<string, RelokTabela *>(section, new RelokTabela(section)));
    sekcije.push_back(section);
}

void RelokTabele::dodaj(RelokZapis *rel)
{
    (mapa.at(section)->relocations).push_back(*rel);
}