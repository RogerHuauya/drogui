/***********************************************************
 * Execute Get-Content .\in.h | .\idk.exe > .\registerMap.h
 * to updtae registerMap.h
 ***********************************************************/

#include <bits/stdc++.h>
using namespace std;

string line;

int main(){
    int cnt = 1, flag = 1;
    cout << "string varNames[] = {\"\"";
    while(getline(cin, line)){
        
        stringstream in(line);
        string word;
        //if(line.empty()){ cout << endl; continue; }
        in >> word;
        if(word == "#define"){
            if(flag){ 
                flag = 0;
                continue;
            }
            string var; 
            in >> var;
            cout << ",\"\","<< '\"' << var << "\"";
        }

    }
    cout << "};\n";


}