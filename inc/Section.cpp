#include "Section.h"

int Section::dodajBajt(char val)
{
    counter++;
    SectionTwo *s = sekcije.at(sekcije.size() - 1);
    s->data.insert(std::pair<int, char>(s->counter, val));
    s->counter++;
    return s->counter - 1;
}

ostream &operator<<(ostream &os, const SectionTwo &s)
{
    if (s.name != ".UND")
        os << endl;
    for (auto it : s.data)
    {
        string out = Section::napravistring(it.second);
        os << out << "\t";
    }
    os << endl;
    os << "#kraj"
       << "\n";
    return os;
}
string Section::napravistring(unsigned char second)
{
    char str[10];
    sprintf(str, "%02X", second);
    string out(str);
    return out;
}
ostream &operator<<(ostream &os, const Section &s)
{

    for (int i = 0; i < s.sekcije.size(); i++)
    {

        if (s.sekcije.at(i)->name == ".UND")
            os << "#" << s.sekcije.at(i)->name << ":";
        else
            os << "#." << s.sekcije.at(i)->name << ":";

        os << *s.sekcije.at(i);
    }

    return os;
}

int Section::dodajDvaBajta(short val)
{
    SectionTwo *s = sekcije.at(sekcije.size() - 1);
    int pom = s->counter;
    char MASK = 255;
    unsigned char value = (unsigned char)val & MASK;
    s->data.insert(std::pair<int, char>(s->counter, value));
    s->counter++;
    value = (val >> 8) & MASK;
    s->data.insert(std::pair<int, char>(s->counter, value));
    s->counter++;
    ////////////////////
    counter += 2;
    return pom;
}
