#include "Instrukcija.h"

Instrukcija::Instrukcija(string n, string oc,int on){
this->name=n;
this->operand_number=on;
this->OC=oc;
}

Instrukcije::Instrukcije(){

  instrukcije={};
}


void Instrukcije::dohvSveInstrukcije(){


instrukcije.push_back(Instrukcija("halt", "0x00", 0));

instrukcije.push_back(Instrukcija("int", "0x10", 1));

instrukcije.push_back(Instrukcija("iret", "0x20", 0));

instrukcije.push_back( Instrukcija("call", "0x30", 1));

instrukcije.push_back(Instrukcija("ret", "0x40", 0));

instrukcije.push_back(Instrukcija("jmp", "0x50", 1));
instrukcije.push_back(Instrukcija("jeq", "0x51", 1));
instrukcije.push_back(Instrukcija("jne", "0x52", 1));
instrukcije.push_back(Instrukcija("jgt", "0x53", 1));

//NEPOZNATO ZA PUSH I POP?
//instrukcije.push_back( Instrukcija("push", 0x2, 1));
//instrukcije.push_back(Instrukcija("pop", 0x2, 1));  

instrukcije.push_back(Instrukcija("xchg", "0x60", 2));   

instrukcije.push_back( Instrukcija("add", "0x70", 2));          
instrukcije.push_back(Instrukcija("sub", "0x71", 2));       
instrukcije.push_back(Instrukcija("mul", "0x72", 2));     
instrukcije.push_back(Instrukcija("div", "0x73", 2));   
instrukcije.push_back(Instrukcija("cmp", "0x74", 2));

//zasto 2 za not
instrukcije.push_back(Instrukcija("not", "0x80", 1));  
instrukcije.push_back(Instrukcija("and", "0x81", 2));  
instrukcije.push_back(Instrukcija("or", "0x82", 2));     
instrukcije.push_back(Instrukcija("xor", "0x83", 2));  
instrukcije.push_back(Instrukcija("test", "0x84", 2)); 

instrukcije.push_back(Instrukcija("shl", "0x90", 2));  
instrukcije.push_back(Instrukcija("shr", "0x91", 2));  

instrukcije.push_back(Instrukcija("ldr","0xA0",2));  

instrukcije.push_back(Instrukcija("str","0xB0",2)); 


//NEMA MOV
// Instruction("mov", 0x0C, 2),



}



int Instrukcije::dohvBrojOperanada(string ii){

for(Instrukcija i :instrukcije){
  if(i.name==ii){
    return i.operand_number;
  }
}
return 0;
}


string Instrukcije::dohvOpKod(string ii){


for(Instrukcija i :instrukcije){
  if(i.name==ii){
    return i.OC;
  }
}
return "";
}