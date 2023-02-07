#include <iostream>
#include <string>
#include "Asembler.h"

using namespace std;
int main(int argc, char **argv)
{
    if (argc < 4 || argc > 4)
    {
        cout << "argv[1]=-o argv[2]=output argv[3]=input!" << endl;
        return 1;
    }
    else
    {
        string input = argv[3];
        string output = argv[2];

        Asembler a = Asembler(input, output);
        a.generisiTokene();
    }

    return 0;
}