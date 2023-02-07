
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

using namespace std;


class Instrukcija{
public:
    string  name;
    
    string OC;
    int operand_number;
    Instrukcija(string n,string oc,int on);
  
};

class Instrukcije{

public:
Instrukcije();
vector<Instrukcija>  instrukcije;

void dohvSveInstrukcije();
int dohvBrojOperanada(string ii);
string dohvOpKod(string ii);
};