#include <bits/stdc++.h>
using namespace std;

int main(){
    string entrada;
    int DLat, DLon;
    double Lat, Lon, Alt;
    int cont = 0;
    /*while(getline(cin,entrada)){
        if(entrada.size() > 0 && entrada[0] == '+'){
            //+CGPSINFO: 1201.395084,S,07702.904915,W,091220,223317.0,82.6,2.0,132.8
            sscanf(entrada.c_str(), "+CGPSINFO: %2d%lf,%*c,%3d%lf,", &DLat, &Lat, &DLon, &Lon);
            printf("%d\t %lf \t %lf \n", ++cont, DLat + Lat/60, DLon + Lon/60);
        }
    }*/
    printf("A,B,C,D\n");
    while(getline(cin,entrada)){
        sscanf(entrada.c_str(),"%d,%lf,%lf,%lf",&cont, &Lat, &Lon, &Alt);
        printf("%d,%lf,%lf,%lf\n", cont, Lat, Lon, Alt);    
    }
    


    return 0;
}