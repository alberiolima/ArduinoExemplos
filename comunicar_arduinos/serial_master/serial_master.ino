/*
 * Protocolo
 * velocidade de comunicação 115200 
 * 
 * 0 => endereço de destino 0 a 255
 * 1 => Indica operacao 1=Entrega de endereço
 * 2 => Indica porta 
 * 3 => Indica valor 
 * Finaliza com newline '\n'
 * 
 * 0000\n Indica que existe um arduino acima
 * 
 * Todos os arduino ficam aguardando o endereço o primeiro arduino vai receber o endereco do master e então passa o endereço ao seguinte, que passa ao seguinte, ...
 * Entrega do primeiro endereço 0110\n
 * quando o master receber uma configuração de endereço,quer dizer que todos já estão conectados
 */

#define _MASTER_

#include <SoftwareSerial.h>

SoftwareSerial serial2( 3, 2 ); //RX, TX

byte cEndComunica = 0;
byte dadosRecebidos[10];
byte posiDado = 0;

void enviaEndereco() {
  enviaCommando( 0, 1, cEndComunica + 1, 0 );
  delay(100);
  enviaCommando( 0, 1, cEndComunica + 1, 0 );
}

void enviaCommando( byte endereco, byte operacao, byte porta, byte dado ) {
  serial2.write( endereco );
  serial2.write( operacao );
  serial2.write( porta );
  serial2.write( dado );
  serial2.write('\n'); 
}

void setup() {
  Serial.begin( 9600 );
  Serial.println( "Comunicacao com varios arduinos" );
  
  serial2.begin( 115200 );  //Inicia porta comunicaçao serial entre arduinos  
  
  //Como esse é o master envia primeiro endereço para o primeiro escravo
  Serial.println( "MASTER");
  cEndComunica = 1; //endereço do master
  delay(100); //aguarda um tempo para todos os arduino estarem ligados
  enviaEndereco();  
}

void loop() {
  enviaCommando( 2, 0, 4, 1 );
  delay(200);    
  enviaCommando( 3, 0, 4, 1 );
  delay(200);
    
  enviaCommando( 4, 0, 4, 1 );
  delay(200);  
  enviaCommando( 4, 0, 4, 0 );
  delay(200);

  enviaCommando( 3, 0, 4, 0 );
  delay(200);  
  enviaCommando( 2, 0, 4, 0 );
  delay(200);        
}

