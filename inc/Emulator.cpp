#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Emulator.h"

void Emulator::ucitaj(string in)
{
  // 57344
   cpu.psw = cpu.psw | INTER_MASK;

  ifstream fajl;
  fajl.open(in);
  string line = {};

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

    for (int j = 1; j < to_process.size(); j++)
    {
      masKod.push_back(to_process[j]);
    }
  }

  // INICAJALIZACIJA MEMORIJE
  int i = 0;
  for (string s : masKod)
  {
    short val = stoi(s, nullptr, 16);
    this->setMemoryValue(this->memory + i, val);
    i += 2;
  }
}

void Emulator::ispisi()
{

  cout << "Emulated processor executed halt instruction" << endl;
  cout << "Emlated processor state: psw=0b" << toBinary(cpu.psw) << endl;
  cout << "r0=0x" << napravistring(cpu.r[0]) << "\t";
  cout << "r1=0x" << napravistring(cpu.r[1]) << "\t";
  cout << "r2=0x" << napravistring(cpu.r[2]) << "\t";
  cout << "r3=0x" << napravistring(cpu.r[3]) << "\n";
  cout << "r4=0x" << napravistring(cpu.r[4]) << "\t";
  cout << "r5=0x" << napravistring(cpu.r[5]) << "\t";
  cout << "r6=0x" << napravistring(cpu.r[6]) << "\t";
  cout << "r7=0x" << napravistring(cpu.r[7]) << "\n";

  for (int i = 0; i < 100; i += 2)
  {
    Address adr = this->getMemoryValue(this->memory + i);
  }
}

Address Emulator::getMemoryValue(char *memoryLocation)
{
  Address *mar = (Address *)(memoryLocation);
  return *mar;
}
void Emulator::setMemoryValue(char *memoryLocation, short &value)
{
  Address *mar = (Address *)memoryLocation;
  *mar = value;
}

void Emulator::executeInstruction()
{
  cpu.r[PC] = 92;
  int i;
  while (1)
  {
    i = cpu.r[PC];
  

    if (masKod[i] == "00")
    {
      cpu.r[PC]++;
      ispisi();
      break;
    }
    else if (masKod[i] == "10") // int
    {
      // push psw;
      cpu.r[SP] -= 1;
      short val = (short)cpu.psw;
      this->setMemoryValue(this->memory + cpu.r[SP] * 2, val);

      // push pc
      cpu.r[SP] -= 1;
      short val1 = (short)(cpu.r[PC] + 2);
      this->setMemoryValue(this->memory + cpu.r[SP] * 2, val1);

      int rD = stoi(masKod[i + 1].substr(0, 1));
      string lok1 = masKod[(cpu.r[rD] % 8) * 2];
      string lok2 = masKod[(cpu.r[rD] % 8) * 2 + 1];
      string lok = lok2 + lok1;
      cpu.r[7] = stoi(lok, nullptr, 16);
    }
    else if (masKod[i] == "20") // iret
    {
      // pop pc;
      cpu.r[7] = (short)this->getMemoryValue(this->memory + cpu.r[SP] * 2);
      cpu.r[SP] += 1;

      // pop psw;
      cpu.psw = (short)this->getMemoryValue(this->memory + cpu.r[SP] * 2);
      cpu.r[SP] += 1;

     
    }
    else if (masKod[i] == "30") // call
    {
      uvecajPC(masKod[i + 2]);
      call(masKod[i], masKod[i + 1], masKod[i + 2], masKod[i + 3], masKod[i + 4]);
      continue;
    }
    else if (masKod[i] == "40") // ret
    {
      // pop pc;
      cpu.r[7] = (short)this->getMemoryValue(this->memory + cpu.r[SP] * 2);
      cpu.r[SP] += 1;

     
    }
    else if (masKod[i] == "50" || masKod[i] == "51" || masKod[i] == "52" || masKod[i] == "53") // jmp
    {
      uvecajPC(masKod[i + 2]);
      skokovi(masKod[i], masKod[i + 1], masKod[i + 2], masKod[i + 3], masKod[i + 4]);

      continue;
    }
    else if (masKod[i] == "60") // xchg
    {
      string reg = masKod[i + 1];
      int rD = stoi(reg.substr(0, 1));
      int rS = stoi(reg.substr(1, 1));
      short tmp = cpu.r[rD];
      cpu.r[rD] = cpu.r[rS];
      cpu.r[rS] = cpu.r[rD];

      i++;
      continue;
    }
    else if (masKod[i] == "70" || masKod[i] == "71" || masKod[i] == "72" || masKod[i] == "73" || masKod[i] == "74")
    {

      cpu.r[PC] += 2;
      aritmetickaIns(masKod[i], masKod[i + 1]);
      continue;
    }
    else if (masKod[i] == "80" || masKod[i] == "81" || masKod[i] == "82" || masKod[i] == "83" || masKod[i] == "84")
    {
      cpu.r[PC] += 2;
      logickaIns(masKod[i], masKod[i + 1]);
      continue;
    }
    else if (masKod[i] == "90" || masKod[i] == "91")
    {
      cpu.r[PC] += 2;
      pomerackaIns(masKod[i], masKod[i + 1]);
      continue;
    }
    else if (masKod[i] == "A0") // ldr
    {
      uvecajPC(masKod[i + 2]);

      if (i + 3 <= masKod.size())
      {

        ldr(masKod[i], masKod[i + 1], masKod[i + 2], masKod[i + 3], masKod[i + 4]);
      }
      else
      {
        ldr(masKod[i], masKod[i + 1], masKod[i + 2], "", "");
      }

      /*   if (masKod[i + 3] == "28")
         {
           break;
         }*/
      continue;
      //  break;
    }
    else if (masKod[i] == "B0") // str
    {
      uvecajPC(masKod[i + 2]);
      if (i + 3 <= masKod.size())
      {
        str(masKod[i], masKod[i + 1], masKod[i + 2], masKod[i + 3], masKod[i + 4]);
      }
      else
      {
        str(masKod[i], masKod[i + 1], masKod[i + 2], "", "");
      }
      continue;
    }
  }
};

void Emulator::uvecajPC(string adr)
{
  string adrMod = adr.substr(1, 1);
  if (adrMod == "1" || adrMod == "2")
  {
    cpu.r[PC] += 3;
  }
  else
  {
    cpu.r[PC] += 5;
  }
}

void Emulator::str(string opCode, string reg, string adr, string op1, string op2)
{
  int rD = stoi(reg.substr(0, 1));
  string adrMod = adr.substr(1, 1);
  string u = adr.substr(0, 1);

  if (adrMod == "0") // neposredno
  {
  }
  else if (adrMod == "1") // reg dir    //PROVERENO
  {
    string r = reg.substr(1, 1);
    cpu.r[stoi(r)] = cpu.r[rD];
  }
  else if (adrMod == "2") // reg ind    //PROVERENO
  {
    string r = reg.substr(1, 1);
    short val = (short)cpu.r[rD];

    if (u == "1")
    {
      // push
      cpu.r[6] -= 1;
    }

    this->setMemoryValue(this->memory + cpu.r[stoi(r)] * 2, val);
  }
  else if (adrMod == "3") // reg ind sa pom    //PROVERENO
  {
    string r = reg.substr(1, 1);
    string adr = op2 + op1;
    short val = (short)cpu.r[rD];
    this->setMemoryValue(this->memory + (stoi(adr, nullptr, 16) + cpu.r[stoi(r)]) * 2, val);
  }
  else if (adrMod == "4") // memorijsko  //PROVERENO
  {
    // stavi u memorije ovoj emulatora
    string adr = op2 + op1;
    short val = (short)cpu.r[rD];
    this->setMemoryValue(this->memory + stoi(adr, nullptr, 16) * 2, val);
  }
}
void Emulator::ldr(string opCode, string reg, string adr, string op1, string op2)
{

  // ldr                       //str
  // regD <= operand;          //operand <= regD;
  int rD = stoi(reg.substr(0, 1));
  string adrMod = adr.substr(1, 1);
  string u = adr.substr(0, 1);

  short oper;

  if (adrMod == "0") // neposredno     //PROVERENO
  {
    string adr = op2 + op1;
    oper = stoi(adr, nullptr, 16);
  }
  else if (adrMod == "1") // reg dir  //PROVERENO
  {
    string r = reg.substr(1, 1);
    oper = cpu.r[stoi(r)];
  }
  else if (adrMod == "2") // reg ind   //PROVERENO
  {
    string r = reg.substr(1, 1);
    oper = this->getMemoryValue(this->memory + cpu.r[stoi(r)] * 2);

    if (u == "4")
    {
      // pop
      cpu.r[6] += 1;
    }
  }
  else if (adrMod == "3") // reg ind sa pom  //PROVERENO
  {
    
    string r = reg.substr(1, 1);
    string adr = op2 + op1;
    if (stoi(r) == 6)
    {
      oper = this->getMemoryValue(this->memory + ((stoi(adr, nullptr, 16) / 2) + cpu.r[stoi(r)]) * 2);
    }
    else
    {
      oper = this->getMemoryValue(this->memory + (stoi(adr, nullptr, 16) + cpu.r[stoi(r)]) * 2);
    }
  }
  else if (adrMod == "4") // memorijsko  //PROVERENO
  {
    // vrati iz memorije ovog emulatora

    string adr = op2 + op1;
    oper = this->getMemoryValue(this->memory + stoi(adr, nullptr, 16) * 2);
   
  }

  cpu.r[rD] = oper;
}

void Emulator::call(string opCode, string reg, string adr, string op1, string op2)
{
  // PUSH PC;
  cpu.r[SP] -= 1;
  short val = (short)cpu.r[PC];
  this->setMemoryValue(this->memory + cpu.r[SP] * 2, val);

  // PC<<OPERAND
  string adrMod = adr.substr(1, 1);
  /*
    if (adrMod == "0") // neposredno
    {
      string adr = op2 + op1;
      cpu.r[7] = stoi(adr, nullptr, 16);
      return;
    }
    else if (adrMod == "1") // reg dir
    {
      string r = reg.substr(1, 1);
      cpu.r[7] = cpu.r[stoi(r)];
    }
    else if (adrMod == "2") // reg ind
    {
      string r = reg.substr(1, 1);
      cpu.r[7] = this->getMemoryValue(this->memory + cpu.r[stoi(r)] * 2);
    }
    else if (adrMod == "3") // reg ind sa pom
    {
      string r = reg.substr(1, 1);
      string adr = op2 + op1;
      cpu.r[7] = this->getMemoryValue(this->memory + (stoi(adr, nullptr, 16) + cpu.r[stoi(r)]) * 2);
    }
    else if (adrMod == "4") // memorijsko
    {
      // vrati iz memorije ovoj emulatora

      string adr = op2 + op1;
      cpu.r[7] = this->getMemoryValue(this->memory + stoi(adr, nullptr, 16) * 2);
    }
  */

  if (adrMod == "0") // neposredno //PROVERENO
  {
    string adr = op2 + op1;
    cpu.r[7] = stoi(adr, nullptr, 16);
    return;
  }
  else if (adrMod == "1") // reg dir   //PROVERENO
  {
    string r = reg.substr(1, 1);
    cpu.r[7] = cpu.r[stoi(r)];
  }
  else if (adrMod == "2") // reg ind  //PROVERENO
  {
    string r = reg.substr(1, 1);
    cpu.r[7] = this->getMemoryValue(this->memory + cpu.r[stoi(r)] * 2);
  }
  else if (adrMod == "3") // reg ind sa pom  //PROVERENO
  {
    string r = reg.substr(1, 1);
    string adr = op2 + op1;
    if (r == "7")
    {
      cpu.r[7] += stoi(adr, nullptr, 16);
    }
    else
    {
      cpu.r[7] = this->getMemoryValue(this->memory + (stoi(adr, nullptr, 16) + cpu.r[stoi(r)]) * 2);
    }
  }
  else if (adrMod == "4") // memorijsko //PROVERENO
  {
    string adr = op2 + op1;
    cpu.r[7] = this->getMemoryValue(this->memory + stoi(adr, nullptr, 16) * 2);
  }
}

void Emulator::skokovi(string opCode, string reg, string adr, string op1, string op2)
{
  if (opCode == "50")
  { // jmp
    // nema uslova
  }
  else if (opCode == "51")
  { // jeq

    bool z = cpu.psw & SET_Z;
    if (!z)
      return;
  }
  else if (opCode == "52")
  { // jne
    bool z = cpu.psw & SET_Z;
    if (z)
      return;
  }
  else if (opCode == "53")
  { // jgt
    bool n = cpu.psw & SET_N;
    bool o = cpu.psw & SET_O;
    bool z = cpu.psw & SET_Z;
    if (!((n == 0) && !z))
      return;
  }

  string adrMod = adr.substr(1, 1);

  if (adrMod == "0") // neposredno //PROVERENO
  {
    string adr = op2 + op1;
    cpu.r[7] = stoi(adr, nullptr, 16);
    return;
  }
  else if (adrMod == "1") // reg dir   //PROVERENO
  {
    string r = reg.substr(1, 1);
    cpu.r[7] = cpu.r[stoi(r)];
  }
  else if (adrMod == "2") // reg ind  //PROVERENO
  {
    string r = reg.substr(1, 1);
    cpu.r[7] = this->getMemoryValue(this->memory + cpu.r[stoi(r)] * 2);
  }
  else if (adrMod == "3") // reg ind sa pom  //PROVERENO
  {
    string r = reg.substr(1, 1);
    string adr = op2 + op1;
    if (r == "7")
    {
      cpu.r[7] += stoi(adr, nullptr, 16);
    }
    else
    {
      cpu.r[7] = this->getMemoryValue(this->memory + (stoi(adr, nullptr, 16) + cpu.r[stoi(r)]) * 2);
    }
  }
  else if (adrMod == "4") // memorijsko //PROVERENO
  {
    string adr = op2 + op1;
    cpu.r[7] = this->getMemoryValue(this->memory + stoi(adr, nullptr, 16) * 2);
  }
}

void Emulator::aritmetickaIns(string opCode, string reg)
{
  int rD = stoi(reg.substr(0, 1));
  int rS = stoi(reg.substr(1, 1));
  if (opCode == "70")
  {
    cpu.r[rD] += cpu.r[rS];
  }
  else if (opCode == "71")
  {
    cpu.r[rD] -= cpu.r[rS];
  }
  else if (opCode == "72")
  {
    cpu.r[rD] *= cpu.r[rS];
  }
  else if (opCode == "73")
  {
    cpu.r[rD] /= cpu.r[rS];
  }
  else if (opCode == "74")
  {
    int rD = stoi(reg.substr(0, 1));
    int rS = stoi(reg.substr(1, 1));

    bool srcSign = (cpu.r[rS] & MOST_SIGNIFICANT_BIT);
    bool dstSign = (cpu.r[rD] & MOST_SIGNIFICANT_BIT);

    short temp = cpu.r[rD] - cpu.r[rS];

    bool resSign = (temp & MOST_SIGNIFICANT_BIT);

    bool overflow = (!dstSign && srcSign && resSign) || (dstSign && !srcSign && !resSign);

    if (temp == 0)
    {
      cpu.psw = cpu.psw | SET_Z;
    }
    else
    {
      cpu.psw = cpu.psw & RESET_Z;
    }
    if (temp & MOST_SIGNIFICANT_BIT)
    {
      cpu.psw = cpu.psw | SET_N;
    }
    else
    {
      cpu.psw = cpu.psw & RESET_N;
    }

    if (overflow)
    {
      cpu.psw = cpu.psw | SET_O;
    }
    else
    {
      cpu.psw = cpu.psw & RESET_O;
    }

    bool carry = (!dstSign && srcSign && !resSign) || (dstSign && srcSign && resSign) || (dstSign && !srcSign && resSign);
    if (carry)
    {
      cpu.psw = cpu.psw | SET_C;
    }
    else
    {
      cpu.psw = cpu.psw & RESET_C;
    }
  }
}

void Emulator::logickaIns(string opCode, string reg)
{
  int rD = stoi(reg.substr(0, 1));
  int rS = stoi(reg.substr(1, 1));

  if (opCode == "80") // not
  {

    cpu.r[rD] = ~cpu.r[rS];
  }
  else if (opCode == "81") // and
  {
    cpu.r[rD] &= cpu.r[rS];
  }
  else if (opCode == "82") // or
  {
    cpu.r[rD] |= cpu.r[rS];
  }
  else if (opCode == "83") // xor
  {
    cpu.r[rD] ^= cpu.r[rS];
  }
  else if (opCode == "84") // test
  {

    Address temp = cpu.r[rD] & cpu.r[rS];

    if (temp == 0)
    {
      cpu.psw = cpu.psw | SET_Z;
    }
    else
    {
      cpu.psw = cpu.psw & RESET_Z;
    }

    if (temp & MOST_SIGNIFICANT_BIT)
    {
      cpu.psw = cpu.psw | SET_N;
    }
    else
    {
      cpu.psw = cpu.psw & RESET_N;
    }
  }
}

void Emulator::pomerackaIns(string opCode, string reg)
{
  int rD = stoi(reg.substr(0, 1));
  int rS = stoi(reg.substr(1, 1));

  bool carry = false;

  for (int i = 0; i < (Address)cpu.r[rS] && i < 16; ++i)
  {
    if (opCode == "90")
    {
      carry = (cpu.r[rD] & MOST_SIGNIFICANT_BIT);
      cpu.r[rD] <<= 1;
    }
    else
    {
      carry = (cpu.r[rD] & LEAST_SIGNIFICANT_BIT);
      cpu.r[rD] >>= 1;
    }
  }

  if (cpu.r[rD] == 0)
  {
    cpu.psw = cpu.psw | SET_Z;
  }
  else
  {
    cpu.psw = cpu.psw & RESET_Z;
  }

  if (cpu.r[rD] & MOST_SIGNIFICANT_BIT)
  {
    cpu.psw = cpu.psw | SET_N;
  }
  else
  {
    cpu.psw = cpu.psw & RESET_N;
  }

  if (carry)
  {
    cpu.psw = cpu.psw | SET_C;
  }
  else
  {
    cpu.psw = cpu.psw & RESET_C;
  }
}

string Emulator::napravistring(short pomeraj)
{
  char str[10];
  sprintf(str, "%04X", pomeraj);
  string s(str);

  return s.substr(s.length() - 4, 4);
}

// g++ -o prog Emulator.cpp Emulator.h main.cpp
//./prog emul.txt