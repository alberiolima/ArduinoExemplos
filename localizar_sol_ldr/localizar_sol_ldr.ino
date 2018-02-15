#include <Servo.h>

#define pinLDR   A1
#define pinServo  9

#define tempoEntreleituras 125 //8 leituras por segundo 1000/125=8

unsigned long proximaLeitura = 0; //Variável para controle de tempo entre leituras
unsigned long somaLeituras = 0;   //Somas das leituras para calculas a média
unsigned long mediaAnterior = 0;  //Quantidade das leituras para calcular a média
unsigned int quantLeituras = 0;

byte posiServo = 90;  //Posição inicial do servo
char charDirecao = 1; //Sentido de rotação do servo

Servo meuServo; 

void setup() {
  Serial.begin( 9600 );
 
  pinMode( pinLDR, INPUT );
  delay(500); 

  //Valores iniciais das variáveis, para não iniciar zeradas, pois gerariam divisão por zero no calculo da média
  somaLeituras = analogRead( pinLDR );
  quantLeituras = 1;
  mediaAnterior = somaLeituras;

  meuServo.attach( pinServo ); //Configura qual pino será usado para o servo
  meuServo.write( posiServo ); //Posiciona o servo na posição inicial
    
}

void loop() {
  int valorLido = analogRead( pinLDR );
  if ( millis() > proximaLeitura ) {
    proximaLeitura += tempoEntreleituras;
    unsigned long mediaLeituras = somaLeituras / (unsigned long)quantLeituras;
    
    //calcula sentido de rotação do servo
    if ( mediaLeituras < mediaAnterior ) {
      charDirecao *= -1;
    }
    posiServo += charDirecao;    //Calcula posição com base no sentido de rotação do servo
    meuServo.write( posiServo ); //Atualiza posição do servo (movimenta)

    //Zera variáveis para iniciar novo calculo da média
    somaLeituras = 0;
    quantLeituras = 0;        

    //Atualiza média anterior com a média atual
    mediaAnterior = mediaLeituras;
    
    //Debug usar Serial Plotter
    Serial.println();
    Serial.print( valorLido );
    Serial.print( " " );    
    Serial.print( mediaLeituras );
    Serial.print( " " );   
        
  }
  //Somas valores para calcular a média
  somaLeituras += valorLido; 
  quantLeituras++;
}
