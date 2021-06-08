#include <bits/stdc++.h>
using namespace std;

int main(){
    int reg = 2;

    string entrada, aux, out;

    while(getline(cin, entrada)){
        if(entrada.size() == 0){ cout << endl; continue; }

        stringstream in(entrada);
        out = "";

        in >> aux; out += aux + " ";
        in >> aux; out += aux + "\t\t";

        cout << out;
        printf("0x%02X", reg);
        reg += 2;
        cout << endl;
        
    }



}