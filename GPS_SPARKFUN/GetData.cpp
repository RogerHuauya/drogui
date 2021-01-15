#include<bits/stdc++.h>
#include<string>
using namespace std;
string data_ser,lati = "",longi = "",alti = "";
int cont1= 0,cont2 = 0;
int main(){
    cin >> data_ser;
    //cout << data_ser.size() << endl;
    for( int i = 0; i < data_ser.size(); i++ ){
        //cout << i << endl;
        if( data_ser[i] == 'b' || data_ser[i] == 'c' || data_ser[i]  == 'r' || data_ser[i]  == 'n' || data_ser[i] == '\\' || data_ser[i] == '\'' || data_ser[i] == 't'   ) continue;
        else{   
            cont1++;
            if( cont1 <=  11 )  lati.push_back( data_ser[i] ); 
            else if ( cont1 <=  22 ) longi.push_back( data_ser[i] );
            else{
                alti.push_back(data_ser[i]);
                cont2++;
                if( cont2 >=  12 ) {
                    cout << lati << " " << longi << " " << alti << endl;
                    longi = "";
                    lati = "";
                    alti = "";
                    cont2 = 0;
                    cont1 = 0;
                }
            }


        }
    }
    return 0;
}