#include <Servo.h>

#define pinLDR A1
#define pinServo 9

unsigned long proximaLeitura = 0;
unsigned long somaLeituras = 0;
unsigned int quantLeituras = 0;
unsigned long mediaAnterior = 0;
byte posiServo = 90;
char charDirecao = 1;

Servo meuServo; 

void setup() {
  Serial.begin( 9600 );
  pinMode( pinLDR, INPUT );
  delay(500); 

  somaLeituras = analogRead( pinLDR );
  quantLeituras = 1;
  mediaAnterior = somaLeituras;

  meuServo.attach(pinServo);

  meuServo.write( posiServo );
    
}

void loop() {
  int valorLido = analogRead( pinLDR );
  if ( millis() > proximaLeitura ) {
    proximaLeitura += 125;
    Serial.println();
    Serial.print( valorLido );
    Serial.print( " " );
    unsigned long mediaLeituras = somaLeituras / (unsigned long)quantLeituras;
    Serial.print( mediaLeituras );
    Serial.print( " " );   
    somaLeituras = 0;
    quantLeituras = 0;    
    
    if ( mediaLeituras < mediaAnterior ) {
      charDirecao *= -1;
    }
    posiServo += charDirecao;
    meuServo.write( posiServo );
    mediaAnterior = mediaLeituras;
  }
  somaLeituras += valorLido; 
  quantLeituras++;
}
