#include "serial.h"
#include "io.h"

char str[20];

char rcv_buf[BUFF_LENGTH];
char snd_buf[BUFF_LENGTH];

int rcv_buff_head = 0, rcv_buff_tail = 0;
int snd_buff_head = 0, snd_buff_tail = 0;

bool sending = false;

void serialWriteChar(char data){
    
    if(!sending){
        while(U1STAbits.UTXBF){}
        U1TXREG = data;
        sending = true;
        IEC3bits.U1TXIE = 1;
    }
    else{
        snd_buf[snd_buff_head++] = data;
        snd_buff_head %= BUFF_LENGTH;
        snd_buff_tail += (snd_buff_head == snd_buff_tail);
        snd_buff_tail %= BUFF_LENGTH;
    }
}

void serialWriteString(char* s){
    
    while(*s){
        snd_buf[snd_buff_head++] = *s, s++;
        snd_buff_head %= BUFF_LENGTH;
        snd_buff_tail += (snd_buff_head == snd_buff_tail);
        snd_buff_tail %= BUFF_LENGTH;
    }
    serialWriteChar(snd_buf[snd_buff_tail++]);
}

void initSerial(int baudrate, int priority){

    // UART1
    ANSELB = 0;

    pinMode(PRTB, 5, INPUT);
    pinMode(PRTB, 3, OUTPUT);

    pinMode(PRTF, 1, INPUT);
    pinMode(PRTD, 7, OUTPUT);

    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB5 = 1;

    TRISFbits.TRISF3 = 0;
    TRISDbits.TRISD5 = 1;
    
    //U1RXRbits.U1RXR = 8; //RB5
    //RPB3Rbits.RPB3R = 1; //RB3

    U1RXRbits.U1RXR = 4; //RF1
    RPD7Rbits.RPD7R = 1; //RB3

    U1MODEbits.UARTEN = 1;
    U1MODEbits.UEN = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;
    U1MODEbits.BRGH = 0;
     
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    
    U1STAbits.URXISEL = 0;
    U1STAbits.UTXISEL = 1;

    IPC28bits.U1RXIP = 4;
    IFS3bits.U1RXIF = 0; 
    IEC3bits.U1RXIE = 1;

    IPC28bits.U1TXIP = 4;
    IFS3bits.U1TXIF = 0;
    IEC3bits.U1TXIE = 0;

    U1BRG = (SYS_FREQ / 2) / (16 * baudrate) - 1;
    U1MODEbits.ON = 1;
}


bool serialAvailable(){
    return rcv_buff_head != rcv_buff_tail;
}

char serialReadChar(){
    rcv_buff_tail = (rcv_buff_tail + 1)%BUFF_LENGTH;
    return rcv_buf[ (rcv_buff_tail + BUFF_LENGTH - 1) % BUFF_LENGTH];
}

void uartRxInterrupt(1, 4){
    while(U1STAbits.URXDA){
        rcv_buf[rcv_buff_head++] = U1RXREG;
        rcv_buff_head %= BUFF_LENGTH;
        rcv_buff_tail += (rcv_buff_head == rcv_buff_tail);
        rcv_buff_tail %= BUFF_LENGTH;
    }
    IFS3bits.U1RXIF = 0; 
}

void uartTxInterrupt(1, 4){
    
    if(snd_buff_tail != snd_buff_head){ 
        U1TXREG =  snd_buf[snd_buff_tail];
        snd_buff_tail = (snd_buff_tail + 1) % BUFF_LENGTH;
    }
    else{
        sending = false;
        IEC3bits.U1TXIE = 0;
    }
    IFS3bits.U1TXIF = 0; 
}

void serialReadString(char *s, int timeout){
    int t;
    while(1){
        t = 0;
        while(!serialAvailable() && t < timeout) t++;
        if(t >= timeout) break;
        *s++ = serialReadChar(); 
    }
    *s = '\0';
}



long long serialParseInt(){
    long long ans = 0LL;
    char c = '*';
    bool flag = false;

    while(1){
        while(!serialAvailable());
        c = serialReadChar();
        serialWriteChar(c);
        if(c == '-') flag = true;
        else if(c == '$') break;
        else{
            ans *= 10LL;
            ans += (int) (c - '0');
        }
    }
    serialWriteChar('\n');
    if(flag) ans *= -1LL;
    return ans;
}
