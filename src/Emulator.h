#include <vector>
#include <string>
using namespace std;
#include <iostream>

#define PC 7
#define SP 6

#define INTER_MASK 0xE000

#define CONDITION_MASK 0xC000
#define CONDITION_SHIFT 14

#define OP1_ADDR 0x0300
#define OP1_ADDR_SHIFT 8
#define OP1_REG 0x00E0
#define OP1_REG_SHIFT 5

#define OP2_ADDR 0x0018
#define OP2_ADDR_SHIFT 3
#define OP2_REG 0x0007
#define OP2_REG_SHIFT 0x0

#define RESET_Z 0xFFFE
#define SET_Z 0x0001

#define RESET_O 0xFFFD
#define SET_O 0x0002

#define RESET_C 0xFFFB
#define SET_C 0x0004

#define RESET_N 0xFFF7
#define SET_N 0x0008

#define OUTPUT_REG 0xFFFE

#define NEGATIVE_MASK 0x8000
#define MOST_SIGNIFICANT_BIT 0x8000
#define LEAST_SIGNIFICANT_BIT 0x0001
#define TIMER_INTERRUPT

typedef unsigned short Address;
struct CPU
{

  Address r[8];
  Address psw;
  Address alu;
  Address ir0;
  Address ir1;
};

class Emulator
{
public:
Emulator(){};
void uvecajPC(string adr);
  vector<string> masKod = {};

void ucitaj(string in);
void ispisi();


  void startEmulation();
  void fetchInstruction();
  void getOperands();

  void executeInstruction();

  void aritmetickaIns(string opCode, string reg);
  void logickaIns(string opCode, string reg);
  void pomerackaIns(string opCode, string reg);
  void skokovi(string opCode, string reg, string adr, string op1, string op2);
  void call(string opCode, string reg, string adr, string op1, string op2);
  void ldr(string opCode, string reg, string adr, string op1, string op2);
  void str(string opCode, string reg, string adr, string op1, string op2);

  Address getMemoryValue(char *memoryLocation);
  void setMemoryValue(char *memoryLocation, short &value);

public:
  CPU cpu;
  char *memory=new char[65536];

  Address stackStart;
  Address stackSize;

  int callStack;
  bool running;

 string napravistring(short pomeraj);


string toBinary(int n)
{
     std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
  
    return r;
   
}



};