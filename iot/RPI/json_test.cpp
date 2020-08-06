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

    Value root, child;
    root["NUM"] = 5;
    root["STRING"] = "hello_world";
    child["username"] = "guest";
    child["pass"] = "1234";
    root["child"] = child;
    
    FastWriter fw;
    string s = fw.write(root);
    cout << s << endl;

    return 0;
}