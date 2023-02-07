#include "SimbolLink.h"

Simbol::Simbol(int id, string naziv, string sekcija, string seklab, int pomeraj, string lokalna, int velicina, string defined,int indexFajla)
{

  this->broj = id;
  this->naziv = naziv;
  this->sekcija = sekcija;
  this->seklab = seklab;
  this->pomeraj = pomeraj;
  this->lokalna = lokalna;
  this->velicina = velicina;

this->indexFajla=indexFajla;

  if (defined == "yes")
    this->defined = true;
  else
    this->defined = false;
}