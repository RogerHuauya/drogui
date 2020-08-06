#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;
using namespace Json;

int main(){
    ifstream ifs("file.json");
    Reader r;
    Value v;
    r.parse(ifs, v);
    cout << "Last name: " << v["lastname"].asString() << endl;
    cout << "First name: " << v["firstname"].asString() << endl;
    cout << "DNI: " << v["dni"].asInt()<< endl;
    return 0;
}