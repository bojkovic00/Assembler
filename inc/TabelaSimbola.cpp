#include "TabelaSimbola.h"
#include <map>
#include <bits/stdc++.h>

using namespace std;
TabelaSimbola::TabelaSimbola()
{
    counter = 0;
    Simbol *s = new Simbol(".UND", ".UND", "section", 0, "local", 0);
    dodajNoviSimbol(*s);
    definisiSimbol(*s);
}
bool TabelaSimbola::dodajNoviSimbol(Simbol &s)
{
    if (symbols.find(s.naziv) == symbols.end())
    {
        s.broj = counter;
        counter++;
        symbols.insert(std::pair<string, Simbol>(s.naziv, s));
        return true;
    }
    else
    {
        return false;
    }
}
Simbol *TabelaSimbola::dohvatiSimbol(string naziv)
{
    auto it = symbols.find(naziv);
    if (it != symbols.end())
    {

        return &it->second;
    }
    else
        return nullptr;
}
void TabelaSimbola::obelezi(string naziv, int gde, bool jedanbajt, string sekc, bool skok)
{
    auto it = symbols.find(naziv);
    it->second.obracanja.push_back(Obracanje(gde, jedanbajt, sekc, skok));
}

bool TabelaSimbola::definisiSimbol(Simbol &s, Section *sekcija, RelokTabele *rt)
{
    string lokalna = "global";
    int broj = 0;

    auto it = symbols.find(s.naziv);

    if (it != symbols.end())
    {
        if (it->second.definisan == true || it->second.ex == true)
            return false;

        definisiSimbol(s);
        broj = it->second.broj;
        lokalna = it->second.lokalna;
    }


    if (s.lokalna == "global")
    {
       

        for (int i = 0; i < it->second.obracanja.size(); i++)
        {
            bool skok = it->second.obracanja.at(i).skok;
            int poz = it->second.obracanja.at(i).pozicije;
            int ofs = it->second.pomeraj;

            string sekc = it->second.obracanja.at(i).sekc;

            for (int i = 0; i < rt->mapa.at(sekc)->relocations.size(); i++)
            {

                if (rt->mapa.at(sekc)->relocations.at(i).pomeraj == poz)
                {
                    rt->mapa.at(sekc)->relocations.at(i).addend += ofs;

                    if (sekc == s.sekcija)
                    {
                        if (rt->mapa.at(sekc)->relocations.at(i).relokTip == "R_386_PC64")
                        {

                            SectionTwo *ssek = sekcija->dohvSekc(sekc);
                            int br;
                            for (auto it : ssek->data)
                            {

                                br = poz + it.first;
                                break;
                            }

                            short vr = rt->mapa.at(sekc)->relocations.at(i).addend - poz;
                            char MASK = 255;
                            unsigned char value = (unsigned char)(vr & MASK);
                            ssek->data.at(br) = value;
                            value = (vr >> 8) & MASK;
                            ssek->data.at(br + 1) = value;

                            rt->mapa.at(sekc)->relocations.erase(rt->mapa.at(sekc)->relocations.begin() + i);
                        }
                    }
                    else
                    {
                        rt->mapa.at(sekc)->relocations.at(i).addend -= ofs;
                    }
                }
            }
        }
    }
    else
    {

        for (int i = 0; i < it->second.obracanja.size(); i++)
        {
            bool skok = it->second.obracanja.at(i).skok;
            int poz = it->second.obracanja.at(i).pozicije;
            bool jedanbajt = it->second.obracanja.at(i).velicine;

            int ofs = it->second.pomeraj;

            if (jedanbajt)
            {
                string sekc = it->second.obracanja.at(i).sekc;
                for (int i = 0; i < rt->mapa.at(sekc)->relocations.size(); i++)
                {
                    if (rt->mapa.at(sekc)->relocations.at(i).pomeraj == poz)
                        rt->mapa.at(sekc)->relocations.at(i).addend += ofs;
                    // ne moze 1b i pcrel
                }
            }
            else
            {
                string sekc = it->second.obracanja.at(i).sekc;

                for (int i = 0; i < rt->mapa.at(sekc)->relocations.size(); i++)
                {

                    if (rt->mapa.at(sekc)->relocations.at(i).pomeraj == poz)
                    {
                        rt->mapa.at(sekc)->relocations.at(i).addend += ofs;

                        if (sekc == s.sekcija)
                        {
                            if (rt->mapa.at(sekc)->relocations.at(i).relokTip == "R_386_PC64")
                            {

                                SectionTwo *ssek = sekcija->dohvSekc(sekc);
                                int br;
                                for (auto it : ssek->data)
                                {

                                    br = poz + it.first;
                                    break;
                                }

                                short vr = rt->mapa.at(sekc)->relocations.at(i).addend - poz;
                                char MASK = 255;
                                unsigned char value = (unsigned char)(vr & MASK);
                                ssek->data.at(br) = value;
                                value = (vr >> 8) & MASK;
                                ssek->data.at(br + 1) = value;

                                rt->mapa.at(sekc)->relocations.erase(rt->mapa.at(sekc)->relocations.begin() + i);
                            }
                        }
                    }
                }
            }
        }
    }

    while (it->second.obracanja.size() != 0)
    {
        it->second.obracanja.pop_back();
    }

    if (lokalna == "local")
    {
        rt->ocisti(broj, dohvatiSimbol(rt->section)->broj, s.sekcija);
    }

    return true;
}

bool TabelaSimbola::definisiSekciju(Simbol &s, string starasekcija, int slc)
{
    auto it = symbols.find(s.naziv);
    it->second.definisan = true;
    it = symbols.find(starasekcija);
    if (it != symbols.end())
    {
        it->second.velicina = slc;
    }
    return true;
}

int TabelaSimbola::dohvatiVrednost(Simbol &s, string sekcija)
{

    auto it = symbols.find(s.naziv);
    if (it != symbols.end())
    {
        if (it->second.sekcija == sekcija)
        {
            return it->second.pomeraj;
        }
    }
    return -1;
}

int TabelaSimbola::dohvatiBrojSekc(string sekcija)
{

    return dohvatiSimbol(sekcija)->broj;

    /*auto it = symbols.find(sekcija);
    if (it != symbols.end())
    {

        return it->second.broj;
    }
    return -1;*/
}

ostream &operator<<(ostream &os, const TabelaSimbola &st)
{
    os << "#.simbolTable" << endl;
    os << "#id\t\t#name\t\t                    #section\t\t#type\t\t#offset\t\t  #scope\t\t#size\t\t#Defined" << endl;

    vector<int> sek;
    for (auto const &sym : st.symbols)
    {
        if (sym.second.seklab == "section")
            sek.push_back(sym.second.broj);
    }

    sort(sek.begin(), sek.end());
    for (int i : sek)
    {
        for (auto const &sym : st.symbols)
        {
            if (sym.second.broj == i)
                os << sym.second << endl;
        }
    }

    vector<int> sim;

    for (auto const &sym : st.symbols)
    {
        if (sym.second.seklab == "label")
            sim.push_back(sym.second.broj);
    }
    sort(sim.begin(), sim.end());

    for (int i : sim)
    {
        for (auto const &sym : st.symbols)
        {
            if (sym.second.broj == i)
                os << sym.second << endl;
        }
    }

    return os;
}
bool TabelaSimbola::definisiSimbol(Simbol &s)
{

    auto it = symbols.find(s.naziv);
    if (it != symbols.end())
    {
        it->second.sekcija = s.sekcija;
        it->second.definisan = true;
        it->second.pomeraj = s.pomeraj;
        it->second.brojSekcije = dohvatiBrojSekc(s.sekcija);
    }
    return true;
}
