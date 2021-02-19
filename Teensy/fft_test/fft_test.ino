#include <arduinoFFT.h>

arduinoFFT FFT = arduinoFFT(); 

const uint16_t samples = 8;
uint8_t exponent;

double vReal[samples] = {1,1,1,1,0,0,0,0};
double vImag[samples];

unsigned long time;

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
  exponent = FFT.Exponent(samples);
}

void loop()
{
    for (uint16_t i = 0; i < samples; i++){
      vImag[i] = 0; 
    }
    time=millis();
    //FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD); 
    FFT.Compute(vReal, vImag, samples, exponent, FFT_FORWARD); 
    for(int i = 0; i< samples; i++){
      Serial.print('(');
      Serial.print(vReal[i]);
      Serial.print(',');
      Serial.print(vImag[i]);
      Serial.println(')');
    }
    FFT.ComplexToMagnitude(vReal, vImag, samples); 
    Serial.println("Magnitude");
    for(int i = 0; i< samples; i++){
      Serial.print('(');
      Serial.print((i>0?2:1)*vReal[i]/samples);
      Serial.println(')');
    }
    while(1){};
}
