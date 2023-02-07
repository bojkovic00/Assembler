#include "Tokeni.h"
using namespace std;

Tokeni::Tokeni(ifstream &in, TabelaSimbola *ts, RelokTabele *rt, Section *s, ofstream &out)
{
    string line;
    this->ts = ts;
    this->rt = rt;
    this->s = s;
    while (getline(in, line))
    {

        vector<string> tokens;

        while (!line.empty() && line.size() > 0)
        {
            if (line.at(0) == '\n')
            {
                line.resize(0);
                break;
            }
            if (line.at(0) == ' ' || line.at(0) == '\t' || line.at(0) == ',')
            {
                int i = 0;
                while (i < line.length() && (line.at(i) == ' ' || line.at(i) == '\t' || line.at(i) == ','))
                {
                    if (line.at(i) == ',')
                    {
                        tokens.push_back(",");
                    }

                    i++;
                }
                line.erase(0, i);
            }
            if (line.size() == 0)
                break;
            if (line.at(0) == '#')
                break;
            int brojac = 0;
            while (!(line.at(brojac) == ' ' || line.at(brojac) == '\t' || line.at(brojac) == ',' || line.at(brojac) == '\n'))
            {
                brojac++;
                if (brojac == line.size())
                    break;
            }
            if (brojac > 0)
            {
                tokens.push_back(line.substr(0, brojac));
                line.erase(0, brojac);
            }
            else if (brojac <= 0)
            {
                break;
            }
        }

      /*  for (int i = 0; i < tokens.size(); i++)
        {
            cout << tokens[i] << endl;
        }*/

        ///////////////////////////////////////////////////////////

        if (tsekcija == ".end" && tokens.size() != 0)
        {
            cout << endl
                 << "GRESKA " << line << endl;
            break;
        }

        if (obradiTokene(tokens) == false)
        {
            cout << endl
                 << "GRESKA " << line << endl;
            break;
        }
    }

    // proveriti da li se zavrsava sa .end
    string tekst = "";
    if (tsekcija == ".end")
    {
        bool kraj = true;
        for (auto const &sym : ts->symbols)
        {
            if (sym.second.definisan == false)
            {
                if (sym.second.ex == false)
                {
                    tekst += "Simbol " + sym.second.naziv + " nije definisan!!\n";
                    kraj = false;
                }
            }
        }
        if (kraj)
        {
            out << *ts << endl;
            out << endl
                << *rt << endl;
            out << endl
                << *s << endl;
        }
        else
        {

            if (tekst == "" && tsekcija == ".end")
                cout << "Obracanja nisu razresena!" << endl;
            else if (tsekcija != ".end")
            {
                cout << ".end ne postoji!" << endl;
            }
            else
                cout << tekst << endl;
        }
    }
    else
    {
        if (tekst == "" && tsekcija == ".end")
            cout << "Obracanja nisu razresena!" << endl;
        else if (tsekcija != ".end")
        {
            cout << ".end ne postoji!" << endl;
        }
        else
            cout << tekst << endl;
    }
};

string Tokeni::tsekcija = "";
string Tokeni::tlabela = "";

bool Tokeni::obradiTokene(vector<string> tokens)
{
    regex instrukcija{"(halt|int|iret|call|ret|jmp|jeq|jne|jgt|push|pop|xchg|add|sub|mul|div|cmp|not|and|or|xor|test|shl|shr|ldr|str)(b|w)?"};
    regex labela{"([a-zA-Z][a-zA-Z0-9_]*):"};
    regex direktiva{"\\.(byte|word|equ|skip)"};
    regex literalap{"(^[0-9][0-9]*$)|(^0x[0-9a-fA-F]*$)"};
    regex symbol{("^([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex sekcija{"^(\\.section)$"};
    regex section{"([a-zA-Z][a-zA-Z0-9_]*)"};
    smatch match;

    instr.dohvSveInstrukcije();

    for (int i = 0; i < tokens.size(); i++)
    {
        string input = tokens[i];
        if (input == ".end")
        {
            string starasekcija = Tokeni::tsekcija;
            Tokeni::tsekcija = input;
            int slc = locationcounter;
            Tokeni::tsekcija = tokens[i];
            locationcounter = 0;
            Simbol *s = new Simbol(tsekcija, tsekcija, "section", locationcounter, "local", 0);
            if (!ts->dodajNoviSimbol(*s))
            {
                cout << "Dva puta definisana sekcija!" << endl;
                return false;
            }
            ts->definisiSekciju(*s, starasekcija, slc);
            if (++i < tokens.size())
            {
                cout << ".end nije poslednji a trebalo bi da bude!" << endl;
                return false;
            }
            return true;
        }
        else if (input == ".global")
        {
            i += 1;
            for (; i < tokens.size(); i++)
            {
                if (regex_match(tokens[i], symbol))
                {

                    Simbol *s = new Simbol(tokens[i], ".UND", "label", locationcounter, "global", 0);
                    if (ts->dohvatiSimbol(s->naziv) != nullptr)
                    {
                        cout << "Vec definisan simbol!" << endl;
                        return false;
                    }
                    else
                    {
                        ts->dodajNoviSimbol(*s);
                    }
                }
                else if (tokens[i] == ",")
                {
                    continue;
                }
                else
                {
                    cout << "Neispravan format " << input << endl;
                    return false;
                }
            }
        }
        else if (input == ".extern")
        {
            i += 1;
            for (; i < tokens.size(); i++)
            {
                if (regex_match(tokens[i], symbol))
                {

                    Simbol *s = new Simbol(tokens[i], ".UND", "label", locationcounter, "global", 0, 0, true);
                    if (ts->dohvatiSimbol(s->naziv) != nullptr)
                    {
                        cout << "Vec definisan simbol!" << endl;
                        return false;
                    }
                    else
                    {
                        ts->dodajNoviSimbol(*s);
                    }
                }
                else if (tokens[i] == ",")
                {
                    continue;
                }
                else
                {
                    cout << "Neispravan format " << input << endl;
                    return false;
                }
            }
        }
        else if (regex_match(input, labela))
        {
            Tokeni::tlabela = input;
            string pom = tlabela;
            pom.replace(pom.length() - 1, 1, "");
            Simbol *si = new Simbol(pom, tsekcija, "label", locationcounter, "local", 0);
            if (!ts->dodajNoviSimbol(*si))
            {
                if (ts->dohvatiSimbol(si->naziv)->definisan)
                {
                    cout << "Druga definicija iste labele!" << endl;
                    return false;
                }
                if (ts->dohvatiSimbol(si->naziv)->ex == true)
                {
                    cout << "SIMBOL JE EXTERNI!" << si->pomeraj << " " << si->naziv << " " << si->sekcija << endl;
                    return false;
                }
            }
            bool dobro;
            dobro = ts->definisiSimbol(*si, s, rt);
            if (!dobro)
            {
                cout << "SIMBOL JE EXTERN!" << si->pomeraj << " " << si->naziv << " " << si->sekcija << endl;

                return false;
            }
        }
        else if (regex_match(input, sekcija))
        {
            i++;
            if (i != tokens.size() && regex_match(tokens[i], section))
            {
                string starasekcija = Tokeni::tsekcija;
                int slc = locationcounter;
                Tokeni::tsekcija = tokens[i];
                locationcounter = 0;
                rt->staviSekciju(Tokeni::tsekcija);
                s->novaSekcija(Tokeni::tsekcija);
                Simbol *s = new Simbol(tsekcija, tsekcija, "section", locationcounter, "local", 0);
                if (!ts->dodajNoviSimbol(*s))
                {
                    cout << "Dva puta definisana sekcija!" << endl;
                    return false;
                }
                ts->definisiSekciju(*s, starasekcija, slc);
            }
            else
            {
                cout << "section nije prosla";
                return false;
            }
        }
        else if (regex_match(input, direktiva))
        {
            if (Tokeni::tsekcija == "")
            {
                cout << "Direktive se ne smeju nalaziti u .UND  sekciji!" << endl;
                return false;
            }
            else if (tokens[i] == ".byte")
            {
                i += 1;
                for (; i < tokens.size(); i++)
                {
                    string znak = "+";
                    if (tokens[i] == "+" || tokens[i] == "-")
                    {
                        znak = tokens[i];
                        i++;
                    }

                    if (regex_match(tokens[i], literalap))
                    {
                        int payload;

                        if (znak == "+")
                        {
                            payload = stringToInt(tokens[i]);
                        }
                        else
                        {
                            payload = -stringToInt(tokens[i]);
                        }
                        s->dodajBajt(payload);
                        locationcounter += 1;
                    }
                    else if (regex_match(tokens[i], symbol))
                    {
                        gurnisimbol(tokens[i], false, true);
                        locationcounter += 1;
                    }
                    else if (tokens[i] == ",")
                    {
                        continue;
                    }
                    else
                    {
                        cout << "Neispravan format " << input << endl;
                        return false;
                    }
                }
            }
            else if (tokens[i] == ".word")
            {
                i += 1;
                for (; i < tokens.size(); i++)
                {
                    string znak = "+";
                    if (tokens[i] == "+" || tokens[i] == "-")
                    {
                        znak = tokens[i];
                        i++;
                    }

                    if (regex_match(tokens[i], literalap))
                    {
                        int payload;

                        if (znak == "+")
                        {
                            payload = stringToInt(tokens[i]);
                        }
                        else
                        {
                            payload = -stringToInt(tokens[i]);
                        }
                        s->dodajDvaBajta(payload);
                        locationcounter += 2;
                    }
                    else if (regex_match(tokens[i], symbol))
                    {
                        gurnisimbol(tokens[i], false, false);
                        locationcounter += 2;
                    }

                    else if (tokens[i] == ",")
                    {
                        continue;
                    }
                    else
                    {
                        cout << "Neispravan format " << input << endl;
                        return false;
                    }
                }
            }
            else if (tokens[i] == ".skip")
            {

                if (tokens.size() != i + 2)
                {
                    cout << "Nekoretkna .skip direktiva";
                    return false;
                }
                i++;
                if (regex_match(tokens[i], literalap))
                {
                    int payload = stringToInt(tokens[i]);
                    locationcounter += payload;
                    for (int j = 0; j < payload; j++)
                    {
                        s->dodajBajt(0);
                    }
                }
                else
                {
                    cout << "Parametar .skip direktive nije literal!";
                    return false;
                }
            }
            else
            {
                cout << "Inkorektna sintaksa direktive" << endl;
                return false;
            }
        }
        else if (regex_match(input, instrukcija))
        {
         

            smatch match;
            regex_match(tokens[i], match, instrukcija);
            string icode = match[1];

            int size;
            if (icode == "push" | icode == "pop")
            {
                size = 1;
            }
            else
            {
                size = instr.dohvBrojOperanada(icode);
            }

            if (size == 0)
            {

                dodajOpKod(icode);
            }

            else if (size == 1)
            {

                if (icode == "push" | icode == "pop")
                {
                }
                else
                {
                    dodajOpKod(icode);
                }

                i++;
                if (icode == "int")
                {
                    // PROVERI STA TREBA 0 jer se ne koristi?
                    string dst = tokens[i].substr(1, 1);
                    string src = "15";

                    s->dodajBajt(stoi(intToBin(dst) + intToBin(src), 0, 2));

                    locationcounter++;
                }
                else if (icode == "not")
                {

                    string dst = tokens[i].substr(1, 1);
                    string src = "0";

                    s->dodajBajt(stoi(intToBin(dst) + intToBin(src), 0, 2));

                    locationcounter++;
                }
                else if (icode == "call" || icode == "jmp" || icode == "jne" || icode == "jgt" || icode == "jeq")
                {

                    string oper = "";
                    for (int j = 1; j < tokens.size(); j++)
                    {
                        oper += tokens[j];
                    }

                    testirajSkokove(icode, oper);

                    return true;
                }
                else if (icode == "push" || icode == "pop")
                {
                    testirajPusheve(icode, tokens[i]);
                }
                else
                {
                    cout << "Neispravna jednoadresna funkcija!" << endl;
                    return false;
                }
            }

            else if (size == 2)
            {

                if (icode == "add" || icode == "sub" || icode == "mul" || icode == "div" || icode == "cmp")
                {
                    aritmetickaOperacija(icode, tokens[i + 1], tokens[i + 3]);
                    return true;
                }
                else if (icode == "and" || icode == "or" || icode == "xor" || icode == "test")
                {
                    logickaOperacija(icode, tokens[i + 1], tokens[i + 3]);
                    return true;
                }
                else if (icode == "shl" || icode == "shr")
                {
                    pomerackaOperacija(icode, tokens[i + 1], tokens[i + 3]);
                    return true;
                }
                else if (icode == "ldr")
                {
                    dodajOpKod(icode);

                    string oper = "";
                    for (int i = 3; i < tokens.size(); i++)
                    {
                        oper += tokens[i];
                    }
                    proveriOperand(oper, tokens[i + 1]);
                    return true;
                }
                else if (icode == "str")
                {

                    string oper = "";
                    for (int i = 3; i < tokens.size(); i++)
                    {
                        oper += tokens[i];
                    }

                    dodajOpKod(icode);

                    proveriOperand(oper, tokens[i + 1]);
                    return true;
                }
                else if (icode == "xchg")
                {
                    dodajOpKod(icode);

                    // RegDescr
                    string dst = tokens[i + 1].substr(1, 1);
                    string src = tokens[i + 3].substr(1, 1);

                    s->dodajBajt(stoi(intToBin(dst) + intToBin(src), 0, 2));
                    locationcounter++;
                    return true;
                }
            }
            else
            {
                cout << "Nesipravno napisana instrukcija! " << endl;
                return false;
            }
        }
        else
        {
            cout << tokens[0];
            cout << "Neadekvatna instrukcija!" << endl;
            return false;
        }
    }
    return true;
}

void Tokeni::dodajOpKod(string icode)
{
    s->dodajBajt(stringToInt(instr.dohvOpKod(icode)));
    poslinstrukcija = s->counter;
    locationcounter++;
}

void Tokeni::proveriOperand(string op, string regD)
{
    //$<literal>
    regex literalimm{"(^\\$[-]*[0-9][0-9]*$)|(^\\$[-]?0x([0-9a-fA-F])*$)"};

    //$<symbol>
    regex symbolimm{"^\\$[a-zA-Z][a-zA-Z0-9_]*$"};

    //<literal>
    regex literalap{"(^[0-9][0-9]*$)|(^0x[0-9a-fA-F]*$)"};

    //<symbol>
    regex symbolap{"^[a-zA-Z][a-zA-Z0-9_]*$"};

    //%<symbol>
    regex pcrel{"^\\%[a-zA-Z][a-zA-Z0-9_]*$"};

    //[<reg>]
    regex regind{"^\\[r[0-9][l|h]?\\]$"};

    // <reg>
    regex regdir{"^r[0-9][l|h]*$"};

    //[<reg> + <literal>]
    regex litreg{"(^\\[r[0-9][l|h]*\\+[0-9][0-9]*\\]$)|(^\\[r[0-9][l|h]*\\-[0-9][0-9]*\\]$)|(^\\[r[0-9][l|h]*\\+0x[0-9a-fA-F]+\\]$)|(^\\[r[0-9][l|h]*\\-0x[0-9a-fA-F]+\\]$)"};

    //[<reg> + <symbol>]
    regex symbreg{"(^\\[r[0-9][l|h]*\\+[a-zA-Z][a-zA-Z0-9_]*\\]$)|(^\\[r[0-9][l|h]*\\-[a-zA-Z][a-zA-Z0-9_]*\\]$)"};

    string r = regD.substr(1, 1);
    string regsDescr = "000" + r;

    if (regex_match(op, literalimm))
    {

        // regsDescr += "0000";
        regsDescr += "1111";
        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 0; // Nema azuriranja + neposredno

        s->dodajBajt(addrMode);
        locationcounter++;

        string lit = op.substr(1, op.length());

        // Payload
        int payload;

        payload = stringToInt(lit);
        s->dodajDvaBajta(payload);

        locationcounter += 2;
    }
    else if (regex_match(op, regdir))
    {
        string reg = intToBin(op.substr(1, 1));
        regsDescr += reg;

        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 1; // Regdir bez azuriranja
        s->dodajBajt(addrMode);
        locationcounter++;
    }
    else if (regex_match(op, symbolimm))
    {
        //  regsDescr += "0000";
        regsDescr += "1111";

        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 0; // Nema azuriranja + neposredno

        s->dodajBajt(addrMode);
        locationcounter++;

        // I OVDE I KOD SKOKA FALSE?
        gurnisimbol(op.substr(1, op.length()), false);
        locationcounter += 2;
    }
    else if (regex_match(op, literalap))
    {
        //        regsDescr += "0000";
        regsDescr += "1111";
        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 4; // Nema azuriranja + memorijsko

        s->dodajBajt(addrMode);
        locationcounter++;

        string lit;
        string znak = "+";
        for (int j = 0; j < op.length(); j++)
        {
            if (op.at(j) == '-')
            {
                znak = "-";
                lit = op.substr(j + 1, op.length());
            }
        }

        // Payload
        int payload;
        if (znak == "+")
        {
            payload = stringToInt(op);
            s->dodajDvaBajta(payload);
        }
        else
        {
            payload = -stringToInt(lit);
            s->dodajDvaBajta(payload);
        }

        locationcounter += 2;
    }
    else if (regex_match(op, symbolap))
    {
        // regsDescr += "0000";
        regsDescr += "1111";

        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 4; // Nema azuriranja + memorijsko

        s->dodajBajt(addrMode);
        locationcounter++;

        gurnisimbol(op, false);
        locationcounter += 2;
    }
    else if (regex_match(op, pcrel))
    {

        string simb = op.substr(1, op.length());

        regsDescr += "0111"; // ip = r7
        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 3; // 0000 0011 = regindpom bez azuriranja
        s->dodajBajt(addrMode);
        locationcounter++;

        gurnisimbol(simb, true);
        locationcounter += 2;
    }
    else if (regex_match(op, regind))
    {

        string reg = intToBin(op.substr(2, 1));
        regsDescr += reg;

        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 2; // Regind bez azuriranja
        s->dodajBajt(addrMode);
        locationcounter++;
    }
    else if (regex_match(op, litreg))
    {

        string reg = intToBin(op.substr(2, 1));

        regsDescr += reg;

        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 3; // Nema azuriranja + regindpom

        s->dodajBajt(addrMode);
        locationcounter++;

        string lit = "";
        string znak;

        for (int j = 0; j < op.length(); j++)
        {
            if (op.at(j) == '+' || op.at(j) == '-')
            {
                znak = op.at(j);
                lit = op.substr(j + 1, op.length() - 5);
            }
        }

        //   string lit = token.substr(5, token.length() - 1); // literal
        // Payload
        int payload = stringToInt(lit);
        if (znak == "+")
        {
            s->dodajDvaBajta(payload);
        }
        else
        {
            s->dodajDvaBajta(-payload);
        }

        locationcounter += 2;
    }
    else if (regex_match(op, symbreg))
    {

        string br = op.substr(2, 1);
        string simb;

        for (int j = 0; j < op.length(); j++)
        {
            if (op.at(j) == '+')
            {
                simb = op.substr(j + 1, op.length() - 5);
            }
        }

        regsDescr += "000" + br;

        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;
        int addrMode = 3; // Nema azuriranja + regindpom

        s->dodajBajt(addrMode);
        locationcounter++;

        gurnisimbol(simb, false);
        locationcounter += 2;
    }
}

/////////////////////////  ARIT/LOG/POM INSTR  /////////////////////////////////////////////
void Tokeni::aritmetickaOperacija(string ins, string op1, string op2)
{
    dodajOpKod(ins);

    // RegDescr
    string dst = op1.substr(1, 1);
    string src = op2.substr(1, 1);

    s->dodajBajt(stoi(intToBin(dst) + intToBin(src), 0, 2));
    locationcounter++;
    return;
}
void Tokeni::logickaOperacija(string ins, string op1, string op2)
{
    dodajOpKod(ins);

    // RegDescr
    string dst = op1.substr(1, 1);
    string src = op2.substr(1, 1);

    s->dodajBajt(stoi(intToBin(dst) + intToBin(src), 0, 2));
    locationcounter++;
    return;
}
void Tokeni::pomerackaOperacija(string ins, string op1, string op2)
{
    dodajOpKod(ins);

    // RegDescr
    string dst = op1.substr(1, 1);
    string src = op2.substr(1, 1);

    s->dodajBajt(stoi(intToBin(dst) + intToBin(src), 0, 2));
    locationcounter++;
    return;
}

///////////////// SKOKOVI ///////////////////////////////////////////////////////////
bool Tokeni::testirajSkokove(string icode, string token)
{

    // regex jmpsymbreg{"^\\*[-]*[a-zA-Z][a-zA-Z0-9_]*\\(%r[0-9][l|h]*\\)$"};
    regex regplussimb{"(^\\*\\[r[0-9][l|h]*\\+[a-zA-Z][a-zA-Z0-9_]*\\]$)|(^\\*\\[r[0-9][l|h]*\\-[a-zA-Z][a-zA-Z0-9_]*\\]$)"};

    // regex jmplitreg{"(^\\*[-]*[0-9][0-9]*\\(%r[0-9][l|h]*\\)$)|(^\\*[-]*0x[0-9a-fA-F]+\\(%r[0-9][l|h]*\\)$)"}; // Promenjeno u verziji 1.5
    regex regpluslit{"(^\\*\\[r[0-9][l|h]*\\+[0-9][0-9]*\\]$)|(^\\*\\[r[0-9][l|h]*\\-[0-9][0-9]*\\]$)|(^\\*\\[r[0-9][l|h]*\\+0x[0-9a-fA-F]+\\]$)|(^\\*\\[r[0-9][l|h]*\\-0x[0-9a-fA-F]+\\]$)"};

    // regex jmpregind{"^\\*\\(%r[0-9][l|h]?\\)$"};
    regex zvezdicazagrreg{"^\\*\\[r[0-9][l|h]?\\]$"};

    // regex jmppcrel{"^\\*[a-zA-Z][a-zA-Z0-9_]*\\(%pc\\/%r7\\)$"};
    regex procenatsimbol{"^\\%[a-zA-Z][a-zA-Z0-9_]*$"};

    // regex jmpliteralimm{ "(^[-]*[0-9][0-9]*$)|(^[-]*0x[0-9a-fA-f]+$)" }; //Promenjeno u verziji 1.5
    regex literal{"(^[-]*[0-9][0-9]*$)|(^[-]*0x[0-9a-fA-f]+$)"};

    // regex jmpsymbolimm{ "^[a-zA-Z][a-zA-Z0-9_]*$" };
    regex symbol{"^[a-zA-Z][a-zA-Z0-9_]*$"};

    // regex jmpregdir{ "^\\*%r[0-9][l|h]*$" };
    regex zvezdicaregistar{"^\\*r[0-9][l|h]*$"};

    // regex jmpliteralap{ "(^\\*[0-9][0-9]*$)|(^\\*0x[0-9a-fA-F]+$)" }; //Promenjen u verziji 1.5
    regex zvezdicaliteral{"(^\\*[0-9][0-9]*$)|(^\\*0x[0-9a-fA-F]+$)"};

    // regex jmpsymbolap{ "^\\*[a-zA-Z][a-zA-Z0-9_]*$" };
    regex zvezdicasymbol{"^\\*[a-zA-Z][a-zA-Z0-9_]*$"};

    string kopija = token;
    //  operandConvertorJumps(kopija, true);

    if (icode == "int" || icode == "call" || icode == "jmp" || icode == "jne" || icode == "jgt" || icode == "jeq")
    {


        if (regex_match(token, literal))
        {
           
            jmpLit(kopija);
        }
        else if (regex_match(token, symbol))
        {
            
            jmpSimb(kopija);
        }
        else if (regex_match(token, zvezdicaliteral))
        {
           
            jmpZvezdLit(kopija);
        }
        else if (regex_match(token, zvezdicasymbol) && !regex_match(token, zvezdicaregistar))
        {
           
            jmpZvezdSimb(kopija);
        }
        else if (regex_match(token, procenatsimbol))
        {
            jmpProcSimb(kopija);
        }
        else if (regex_match(token, zvezdicazagrreg))
        {
         
            jmpZvezdZagReg(kopija);
        }
        else if (regex_match(token, zvezdicaregistar))
        {
           
            jmpZvezdReg(kopija);
        }

        else if (regex_match(token, regpluslit))
        {
             
            jmpRegPlusLit(kopija);
        }
        else if (regex_match(token, regplussimb))
        {
            
            jmpRegPlusSimb(kopija);
        }
        else{

        }
    }
    else
    {
        
        return false;
    }
    return true;
}

void Tokeni::jmpLit(string token)
{
   
    // int regDescr = 240; // 1111 + 0000
    int regDescr = 255;
    int adrMode = 0; // neposredno
    s->dodajBajt(regDescr);
    locationcounter += 1;
    s->dodajBajt(adrMode);
    locationcounter += 1;

    string lit;
    string znak = "+";
    for (int j = 0; j < token.length(); j++)
    {
        if (token.at(j) == '-')
        {
            znak = "-";
            lit = token.substr(j + 1, token.length());
        }
    }

    int payload;
    if (znak == "+")
    {
        payload = stringToInt(token);
        s->dodajDvaBajta(payload);
    }
    else
    {
        payload = -stringToInt(lit);
        s->dodajDvaBajta(payload);
    }

    locationcounter += 2;
    return;
};
void Tokeni::jmpSimb(string token)
{
   
    // int regDescr = 240; // 1111 + 0000
    int regDescr = 255;
    int adrMode = 0; // neposredno
    s->dodajBajt(regDescr);
    locationcounter += 1;
    s->dodajBajt(adrMode);
    locationcounter += 1;

    gurnisimbol(token, false);
    locationcounter += 2;
};
void Tokeni::jmpZvezdLit(string token)
{
    // int regDescr = 240; // 1111 + 0000 ispravka
    int regDescr = 255;
    int adrMode = 4; //  memorijsko

    s->dodajBajt(regDescr);
    locationcounter += 1;
    s->dodajBajt(adrMode);
    locationcounter += 1;

    string pom = token.substr(1, token.length());

    int payload = stringToInt(pom);
    s->dodajDvaBajta(payload);
    locationcounter += 2;
    return;
};
void Tokeni::jmpZvezdReg(string token)
{
    string broj = token.substr(2, 1); // broj

    int regDescr = 240;     // 1111 0000
    regDescr += stoi(broj); // Druga cetvroka oznacava registar
    s->dodajBajt(regDescr);
    locationcounter += 1;

    int adrMode = 1; // regdir = 1, a nema azuriranja
    s->dodajBajt(adrMode);
    locationcounter += 1;

    return;
};
void Tokeni::jmpZvezdSimb(string token)
{
    // int regDescr = 240; // 1111 + 0000
    int regDescr = 255;
    int adrMode = 4; // Nema azuriranja i nacin adresiranja je memorijski(0)
    s->dodajBajt(regDescr);
    locationcounter += 1;
    s->dodajBajt(adrMode);
    locationcounter += 1;

    string simb = token.substr(1, token.length());
    gurnisimbol(simb, false);
    locationcounter += 2;
};
void Tokeni::jmpProcSimb(string token)
{ // PCRel adresiranje = reg indirektno sa pomerajem gde je reg = r7

    // VALJDA 1111
    //  int regDescr = 7; // 0000 0111 = r7

    int regDescr = 247; // 11110111
    int adrMode = 3;    // 0000 0011 = regindpom
    s->dodajBajt(regDescr);
    locationcounter += 1;
    s->dodajBajt(adrMode);
    locationcounter += 1;

    string simb = token.substr(1, token.length());

    gurnisimbol(simb, true);
    locationcounter += 2;
};
void Tokeni::jmpZvezdZagReg(string token)
{
    string br = token.substr(3, 1);

    int regDescr = 240;
    regDescr += stoi(br);
    s->dodajBajt(regDescr);
    locationcounter += 1;

    int adrMode = 2; // regind = 2, a nema azuriranja
    s->dodajBajt(adrMode);
    locationcounter += 1;
};
void Tokeni::jmpRegPlusLit(string token)
{
    int regDescr = 240; // 1111 0000
    string lit = "";
    string znak;
    string br = token.substr(3, 1);
    for (int j = 0; j < token.length(); j++)
    {
        if (token.at(j) == '+' || token.at(j) == '-')
        {
            znak = token.at(j);
            lit = token.substr(j + 1, token.length() - 6);
        }
    }

    regDescr += stoi(br); // broj reg

    s->dodajBajt(regDescr);
    locationcounter += 1;
    int adrMode = 3; // regindpom = 3, bez azuriranja
    s->dodajBajt(adrMode);
    locationcounter += 1;

    int payload = stringToInt(lit);
    if (znak == "+")
    {
        s->dodajDvaBajta(payload);
    }
    else
    {
        s->dodajDvaBajta(-payload);
    }

    locationcounter += 2;
    return;
};
void Tokeni::jmpRegPlusSimb(string token)
{

    int regDescr = 240; // 1111 0000
    string simb = "";

    string br = token.substr(3, 1);
    int len;

    for (int j = 0; j < token.length(); j++)
    {
        if (token.at(j) == '+')
        {
            simb = token.substr(j + 1, token.length() - 6);
        }
    }

    regDescr += stoi(br);
    s->dodajBajt(regDescr);
    locationcounter += 1;
    int adrMode = 3; // regindpom = 3, bez azuriranja
    s->dodajBajt(adrMode);
    locationcounter += 1;

    if (stoi(br) == 7)
    {
        gurnisimbol(simb, true);
    }
    else
    {
        gurnisimbol(simb, false);
    }

    locationcounter += 2;
};
//////////////////////////////////////////////////////////////////////////////////////

bool Tokeni::testirajPusheve(string icode, string op)
{
    if (icode == "push")
    {

        dodajOpKod("str");

        string regsDescr = "000" + op.substr(1, 1);
        regsDescr += "0110";
        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 18; // 0001 0010 -> regind sa predekrementom 2
        s->dodajBajt(addrMode);
        locationcounter++;

        return true;
    }
    else if (icode == "pop")
    { 
        dodajOpKod("ldr");

        string regsDescr = "000" + op.substr(1, 1);
        regsDescr += "0110";
        s->dodajBajt(binToInt(regsDescr));
        locationcounter++;

        int addrMode = 66; // 0100 0010 -> regind sa postinkrementom 2
        s->dodajBajt(addrMode);
        locationcounter++;
        return true;
    }
    else
        return false;
}

void Tokeni::gurnisimbol(string token, bool skok, bool jedanbajt)
{

    Simbol *si = new Simbol(token, tsekcija, "label", locationcounter, "local", 0); // prvi bool je bio true
    ts->dodajNoviSimbol(*si);

    if (ts->dohvatiSimbol(si->naziv)->ex == true)
    {
    
        if (skok)
        {
            // PCREL
            s->dodajDvaBajta(0);
            RelokZapis *rel = new RelokZapis(locationcounter, "R_386_PC64", *ts->dohvatiSimbol(si->naziv), si->naziv, -2);
            rt->dodaj(rel);
        }
        else
        {
            // APSOLUNTO
            if (jedanbajt)
            {
                s->dodajBajt(0);
                RelokZapis *rel = new RelokZapis(locationcounter, "R_386_8", *ts->dohvatiSimbol(si->naziv), si->naziv, 0);
                rt->dodaj(rel);
            }
            else
            {
                s->dodajDvaBajta(0);
                RelokZapis *rel = new RelokZapis(locationcounter, "R_386_16", *ts->dohvatiSimbol(si->naziv), si->naziv, 0);
                rt->dodaj(rel);
            }
        }
        return;
    }

   
    if (ts->dohvatiSimbol(si->naziv)->lokalna == "local")
    {

        if (ts->dohvatiSimbol(si->naziv)->definisan)
        { // LOKALAN DEFINISAN
            int pom = ts->dohvatiVrednost(*si, tsekcija);
            if (pom == -1)
            { // LOKALAN DEFINISAN u drugoj sekciji
                if (skok == false)
                {
                    if (jedanbajt)
                    {
                        s->dodajBajt(0);
                        Simbol *sekcija = ts->dohvatiSimbol(si->naziv);
                        RelokZapis *rel = new RelokZapis(locationcounter, "R_386_8", *ts->dohvatiSimbol(sekcija->sekcija), ts->dohvatiSimbol(si->naziv)->sekcija, ts->dohvatiSimbol(si->naziv)->pomeraj);
                    
                        rt->dodaj(rel);
                    }
                    else
                    {
                        s->dodajDvaBajta(0);
                        Simbol *sekcija = ts->dohvatiSimbol(si->naziv);
                        RelokZapis *rel = new RelokZapis(locationcounter, "R_386_16", *ts->dohvatiSimbol(sekcija->sekcija), ts->dohvatiSimbol(si->naziv)->sekcija, ts->dohvatiSimbol(si->naziv)->pomeraj);
                      
                        rt->dodaj(rel);
                    }
                }
                else
                {
                    s->dodajDvaBajta(0);
                    Simbol *sekcija = ts->dohvatiSimbol(si->naziv);
                    RelokZapis *rel = new RelokZapis(locationcounter, "R_386_PC64", *ts->dohvatiSimbol(sekcija->sekcija), ts->dohvatiSimbol(si->naziv)->sekcija, ts->dohvatiSimbol(si->naziv)->pomeraj - 2);
                    rt->dodaj(rel);
                }
            }
            else
            { // LOKALAN DEFINISAN u istoj sekciji

                if (skok) // PCREL
                {
                    s->dodajDvaBajta(pom - (locationcounter + 2));
                }
                else
                { // APSOLUTNO MORA DA SE DODA RELOKACIONI ZAPIS!
                    if (jedanbajt)
                    {
                        RelokZapis *rel = new RelokZapis(locationcounter, "R_386_8", *ts->dohvatiSimbol(si->naziv), tsekcija, pom);
                        rt->dodaj(rel);
                        int broj = ts->dohvatiSimbol(si->naziv)->broj;
                        if (ts->dohvatiSimbol(si->naziv)->lokalna == "local")
                        {
                            rt->ocisti(broj, ts->dohvatiSimbol(rt->section)->broj, tsekcija);
                        }
                        s->dodajBajt(0);
                    }
                    else
                    {
                        RelokZapis *rel = new RelokZapis(locationcounter, "R_386_16", *ts->dohvatiSimbol(si->naziv), tsekcija, pom);
                        rt->dodaj(rel);
                        int broj = ts->dohvatiSimbol(si->naziv)->broj;
                        if (ts->dohvatiSimbol(si->naziv)->lokalna == "local")
                        {
                            rt->ocisti(broj, ts->dohvatiSimbol(rt->section)->broj, tsekcija);
                        }
                        s->dodajDvaBajta(0);
                    }
                }
            }
        }
        else
        { // LOKALAN NIJE DEFINISAN
            // obracanje unapred
            if (skok) // PCREL
            {
               
                ts->obelezi(si->naziv, locationcounter, jedanbajt, tsekcija, true);
                RelokZapis *rel = new RelokZapis(locationcounter, "R_386_PC64", *ts->dohvatiSimbol(si->naziv), tsekcija, -2);
                rt->dodaj(rel);
                s->dodajDvaBajta(0);
            }
            else
            { // APSOLUTNO
             
                ts->obelezi(si->naziv, locationcounter, jedanbajt, tsekcija, false);

                if (jedanbajt)
                {
                    RelokZapis *rel = new RelokZapis(locationcounter, "R_386_8", *ts->dohvatiSimbol(si->naziv), tsekcija, 0);
                    rt->dodaj(rel);
                    s->dodajBajt(0);
                }
                else
                {
                    RelokZapis *rel = new RelokZapis(locationcounter, "R_386_16", *ts->dohvatiSimbol(si->naziv), tsekcija, 0);
                    rt->dodaj(rel);
                    s->dodajDvaBajta(0);
                }
            }
        }
    }
    else
    {
        //  GLOBALAN
        if (ts->dohvatiSimbol(si->naziv)->definisan)
        { // DEFINISAN

            int pom = ts->dohvatiVrednost(*si, tsekcija);
            if (pom == -1)
            { // DEFINISAN U DRUGOJ SEKCIJI

                if (skok)
                {
                    // PCREL
                    s->dodajDvaBajta(0);
                    RelokZapis *rel = new RelokZapis(locationcounter, "R_386_PC64", *ts->dohvatiSimbol(si->naziv), si->naziv, -2);
                    rt->dodaj(rel);
                }
                else
                {
                    // APSOLUNTO
                    if (jedanbajt)
                    {
                        s->dodajBajt(0);
                        RelokZapis *rel = new RelokZapis(locationcounter, "R_386_8", *ts->dohvatiSimbol(si->naziv), si->naziv, 0);
                        rt->dodaj(rel);
                    }
                    else
                    {
                        s->dodajDvaBajta(0);
                        RelokZapis *rel = new RelokZapis(locationcounter, "R_386_16", *ts->dohvatiSimbol(si->naziv), si->naziv, 0);
                        rt->dodaj(rel);
                    }
                }
            }

            else
            {
                // DEFINISAN ISTOJ SEKCIJI

                if (skok)
                {
                    s->dodajDvaBajta(pom - (locationcounter + 2));
                }
                else
                {
                    // APSOLUNTO
                    if (jedanbajt)
                    {
                        s->dodajBajt(0);
                        RelokZapis *rel = new RelokZapis(locationcounter, "R_386_8", *ts->dohvatiSimbol(si->naziv), si->naziv, 0);
                        rt->dodaj(rel);
                    }
                    else
                    {
                        s->dodajDvaBajta(0);
                        RelokZapis *rel = new RelokZapis(locationcounter, "R_386_16", *ts->dohvatiSimbol(si->naziv), si->naziv, 0);
                        rt->dodaj(rel);
                    }
                }
            }
        }
        else
        {
            // NIJE DEFINISAN

            if (skok)
            {
                // PCREL
                ts->obelezi(si->naziv, locationcounter, jedanbajt, tsekcija, true);
                s->dodajDvaBajta(0);
                RelokZapis *rel = new RelokZapis(locationcounter, "R_386_PC64", *ts->dohvatiSimbol(si->naziv), si->naziv, -2);
                rt->dodaj(rel);
            }
            else
            {
                // APSOLUNTO
                if (jedanbajt)
                {
                    s->dodajBajt(0);
                    RelokZapis *rel = new RelokZapis(locationcounter, "R_386_8", *ts->dohvatiSimbol(si->naziv), si->naziv, 0);
                    rt->dodaj(rel);
                }
                else
                {
                    s->dodajDvaBajta(0);
                    RelokZapis *rel = new RelokZapis(locationcounter, "R_386_16", *ts->dohvatiSimbol(si->naziv), si->naziv, 0);
                    rt->dodaj(rel);
                }
            }
        }
    }
}

int Tokeni::stringToInt(string s)
{
    if (s.length() > 2)
    {
        if (s[0] == '0' and s[1] == 'x')
        {
            return stoi(s, nullptr, 16);
        }
        else
        {

            return stoi(s);
        }
    }
    else
    {
        return stoi(s);
    }
}

string Tokeni::intToBin(string number)
{
    int num = stoi(number);
    string bin = bitset<4>(num).to_string();
    return bin;
}

int Tokeni::binToInt(string token)
{
    token = token.substr(2, token.size() - 2);
    int len = token.size();
    int base = 1;
    int temp = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        temp += (token.at(i) - 48) * base;
        base = base * 2;
    }
    return temp;
}



/*
g++ -o assembler Asembler.cpp Asembler.h main.cpp RTabela.cpp RTabela.h Section.cpp Section.h Simbol.cpp Simbol.h TabelaSimbola.cpp TabelaSimbola.h Tokeni.cpp Tokeni.h Instrukcija.cpp Instrukcija.h
./program -o izz.txt tst.s
*/
/*ASSEMBLER=../Asembler/src/assembler
LINKER=../Linker/linker
EMULATOR=../Emulator/emulator
*/
/*
g++ -o emulator Emulator.cpp Emulator.h mainEmul.cpp
./prog emul.txt


g++ -o linker Linker.cpp Linker.h mainLink.cpp RelTabela.h SectionLink.cpp SectionLink.h SimbolLink.cpp SimbolLink.h TabelaSimb.cpp TabelaSimb.h
./prog -hex -o out.txt in1.txt in2.txt
*/