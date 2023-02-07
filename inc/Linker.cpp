#include "Linker.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <iomanip>

using namespace std;

Linker::Linker()
{
    tabSekOff = TS();

    externSymbols = new TabelaSimbola();
    globalSectionList = new TabelaSimbola();
    globalListOfSymbols = new TabelaSimbola();
}

void Linker::procitaj(string in, int index)
{

    ifstream fajl;
    fajl.open(in);

    int indexFajla = index;

    int brSek = 0;
    Section sekcijeF = Section();

    string line;
    getline(fajl, line);

    // CITANJE TABELE SIMBOLA
    if (line == "#.simbolTable")
    {
        getline(fajl, line);

        TabelaSimbola ts = TabelaSimbola();

        while (1)
        {
            getline(fajl, line);
            if (line == "")
                break;

            vector<string> to_process;
            std::stringstream ss(line);
            for (string i; ss >> i;)
            {

                to_process.push_back(i);
                while (
                    ss.peek() == ' ')
                    ss.ignore();
            }

            Simbol *s = new Simbol(stoi(to_process[0]), to_process[1], to_process[2], to_process[3], stoi(to_process[4]), to_process[5], stoi(to_process[6]), to_process[7], indexFajla);

            if (s->seklab == "section" && s->naziv != ".end")
            {
                brSek++;

                Sek *se = new Sek(s->naziv, s->pomeraj, s->velicina);
                sekcijeF.sekcije.push_back(se);
                S skc = S(s->naziv, s->pomeraj, s->velicina, s->indexFajla);
                tabSekOff.dodajS(skc);
            }

            if (to_process[2] == ".UND:" && to_process[5] == "global")
            {
                // externi
                externSymbols->dodajSimbolSim(s);
            }

            ts.dodajSimbolSim(s);
        }

        simTab.insert(std::pair<int, TabelaSimbola>(indexFajla, ts));
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // CITANJE RELOK

    getline(fajl, line);
    int j = 0;
    string sekc;

    RelokTabele r = RelokTabele();

    while (j < brSek)
    {

        getline(fajl, line);
        if (line == "")
            break;

        string s = line.substr(0, 2);
        if (s == "#.")

            sekc = line.substr(6, line.length() - 7);

        RelokTabela *rt = new RelokTabela(sekc);
        rt->section = sekc;

        r.sekcije.push_back(sekc);
        r.mapa.insert(std::pair<string, RelokTabela *>(sekc, rt));
        ////////////////////
        getline(fajl, line);
        while (1)
        {
            getline(fajl, line);
            if (line == "#kraj")
                break;
            vector<string> to_process;
            std::stringstream ss(line);
            for (string i; ss >> i;)
            {

                to_process.push_back(i);
                while (
                    ss.peek() == ' ')
                    ss.ignore();
            }

            Relok rel = Relok(stoi(to_process[0], 0, 16), to_process[1], stoi(to_process[2]), to_process[3], stoi(to_process[4]));
            r.mapa.at(sekc)->relocations.push_back(rel);
        }

        j++;
    }
    relTab.insert(std::pair<int, RelokTabele>(indexFajla, r));

    // CITANJE SEKCIJA
    getline(fajl, line);
    getline(fajl, line);
    j = 0;

    while (j < brSek)
    {
        getline(fajl, line);
        if (line == "")
            break;

        string s = line.substr(0, 2);
        if (s == "#.")

            sekc = line.substr(2, line.length() - 3);

        while (1)
        {
            getline(fajl, line);
            if (line == "#kraj")
                break;

            vector<string> to_process;
            std::stringstream ss(line);
            for (string i; ss >> i;)
            {

                to_process.push_back(i);
                while (
                    ss.peek() == ' ' || ss.peek() == '\t')
                    ss.ignore();
            }

            for (Sek *s : sekcijeF.sekcije)
            {
                if (s->name == sekc)
                {

                    for (string str : to_process)
                    {
                        s->dodajBajt(stoi(str, 0, 16));
                    }
                }
            }
        }
        j++;
    }
    mapSek.insert(std::pair<int, Section>(indexFajla, sekcijeF));

    //   PREDJI NA SLEDECI FAJL
    fajl.close();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Linker::obrada(string out, int brFajlova)
{
    // Formiranje globalne liste simbola i azuriranje njihovih rednih brojeva u tabelama relokacija
    // globalna lista da se proveri jel je neki dvostruko def
    for (std::map<int, TabelaSimbola>::iterator it = simTab.begin(); it != simTab.end(); ++it)
    {
        for (Simbol *sim : it->second.symbols)
        {
            if (!(sim->sekcija == ".UND:" && sim->lokalna == "global"))
            {
                if (!globalListOfSymbols->dodajSimbol(sim))
                    return;
            }
        }
    }

    // ubaci eksterne
    for (Simbol *sim : externSymbols->symbols)
    {
        if (!globalListOfSymbols->dodajEksterni(sim))
        {
            return;
        }
    }

    // SPAJANJE SEKCIJA
    // PROMENA OFFSETA BAS STVARNA VREDNOST SIMBOLA
    //  prodjemo kroz listu globalnih povecamo ofset svakog ne eksternog simbola za pocetak bas njegove sekcije!
    for (std::map<int, TabelaSimbola>::iterator it = simTab.begin(); it != simTab.end(); ++it)

        for (Simbol *sim : it->second.symbols)
        {
            if (!(sim->sekcija == ".UND:" && sim->lokalna == "global"))
            {
                sim->pomeraj += tabSekOff.dohvOffset(sim->sekcija, sim->indexFajla);
            }
        }

    for (std::map<int, TabelaSimbola>::iterator it = simTab.begin(); it != simTab.end(); ++it)

        for (Simbol *sim : it->second.symbols)
        {
            if (sim->sekcija == ".UND:" && sim->lokalna == "global")
            {
                sim->pomeraj = pronadjiOffEkst(sim->naziv);
            }
        }

    // AZURIRANJE SVIH SEKCIJA(prodjemo kroz sve relokacije,pa odatle hvatamo sekciju) SA APSOLUTNIM ADRESIRANJEM PROMENA NA MESTU
    // SEK.AT(#OFFSET)=BAS STVARNA VREDNOST SIMBOLA + ADDEND

   int indexFajla = 0;
    while (indexFajla <= brFajlova)
    {
        for (std::map<string, RelokTabela *>::iterator it = relTab.at(indexFajla).mapa.begin(); it != relTab.at(indexFajla).mapa.end(); ++it)
        {

            for (int k = 0; k < it->second->relocations.size(); k++)
            {
                if (it->second->relocations[k].relokTip == "R_386_16")
                {
                    Simbol *sim = simTab.at(indexFajla).dohvSimbol(it->second->relocations[k].vrSimbola);

                    int pom = sim->pomeraj;
                    string sekccc = it->second->section;
                    mapSek.at(indexFajla).dohvSekciju(sekccc)->promeniDvaBajta(it->second->relocations[k].pomeraj, pom + it->second->relocations[k].addend);
                }
                else if (it->second->relocations[k].relokTip == "R_386_8")
                {
                    Simbol *sim = simTab.at(indexFajla).dohvSimbol(it->second->relocations[k].vrSimbola);
                    int pom = sim->pomeraj;
                    string sekccc = it->second->section;
                    mapSek.at(indexFajla).dohvSekciju(sekccc)->promeniBajt(it->second->relocations[k].pomeraj, pom + it->second->relocations[k].addend);
                }
                else
                {
                    Simbol *sim = simTab.at(indexFajla).dohvSimbol(it->second->relocations[k].vrSimbola);
                    int pom = sim->pomeraj;
                    string sekccc = it->second->section;
                    Simbol *sek = simTab.at(indexFajla).dohvSimbolIME(sekccc);
                    int mestoPrep = it->second->relocations[k].pomeraj + sek->pomeraj;
                    mapSek.at(indexFajla).dohvSekciju(sekccc)->promeniDvaBajta(it->second->relocations[k].pomeraj, pom + it->second->relocations[k].addend - mestoPrep);
                }
            }
        }
        indexFajla++;
    }

    // ISPIS
    vector<unsigned char> konacno = {};
    for (S sk : tabSekOff.sek)
    {
        string n = sk.ime;

        if (mapSek.at(sk.indexFajla).dohvSekciju(n))
        {
            if (mapSek.at(sk.indexFajla).dohvSekciju(n)->data.size() != 0)
            {
                for (std::map<int, unsigned char>::iterator it2 = mapSek.at(sk.indexFajla).dohvSekciju(n)->data.begin(); it2 != mapSek.at(sk.indexFajla).dohvSekciju(n)->data.end(); ++it2)
                {
                    konacno.push_back(it2->second);
                }
            }
        }
    }
    ofstream outs;
    outs.open(out);

    // KONACAN ISPIS
    for (int j = 0; j < konacno.size(); j++)
    {
        if (j == 0)
        {
            outs << std::setfill('0') << std::setw(4) << std::hex << j << ": ";
        }
        else if (j % 8 == 0)
            outs << endl
                 << std::setfill('0') << std::setw(4) << std::hex << j << ": ";
        outs << napravistring(konacno[j]) << " ";
    }

    outs << endl;
    outs << endl;
}

string Linker::napravistring(unsigned char second)
{
    char str[10];
    sprintf(str, "%02X", second);
    string out(str);
    return out;
}

// g++ -o prog Linker.cpp Linker.h main.cpp  RelTabela.h Section.cpp Section.h Simbol.cpp Simbol.h TabelaSimbola.cpp TabelaSimbola.h

int Linker::pronadjiOffEkst(string naz)
{
    for (std::map<int, TabelaSimbola>::iterator it = simTab.begin(); it != simTab.end(); ++it)
    {
        for (Simbol *sim : it->second.symbols)
        {
            if (sim->naziv == naz)
            {
                if (!(sim->sekcija == ".UND:" && sim->lokalna == "global"))
                {
                    return sim->pomeraj;
                }
            }
        }
    }
    return 0;
}