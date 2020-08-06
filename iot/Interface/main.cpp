#include <bits/stdc++.h>
#include <RPI.h>
using namespace std;

void cls(){
    for (int n = 0; n < 10; n++)
      printf( "\n\n\n\n\n\n\n\n\n\n" );
}

int emergencyStop(){
    sendStop();
}

int desplazamiento(){
    sendDesplazamiento();
}


int menu(){
    cls();
    printf("\t\t Principal menu\n");
    printf("[1] Emergency stop\n");
    printf("[2] Desplazamiento (dx, dy , dz, dphi)");
    printf("[3] Show data sensor\n");
    printf("[4] Show image\n");
    printf("[5] Final coordinates\n");
    printf("[6] ARM\n");
    printf("[7] Calibrar ESC\n");
    printf("[8] Zero position\n");
    int op;
    scanf("%d", &op);
    switch(op){
        case 1: emergencyStop(); break;
        case 2: desplazamiento(); break;
        case 3: dataSensor(); break;
        case 4: showImage(); break;
        case 5: finalCoordinates(); break;
        case 6: ARM(); break;
        case 7: calibrateESC(); break;
        case 8: zeroPosition(); break;
        default: printf("%d is not an option, please enter option again\n"); menu(); break;
    }
    
}



int main(){
    while(1){
        menu();
    }
    return 0;
}