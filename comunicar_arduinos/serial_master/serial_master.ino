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

#include <SoftwareSerial.h>

SoftwareSerial serial2( 3, 2 ); //RX, TX

byte cEndComunica = 0;

void enviaEndereco(){
  enviaCommando( 0, 1, cEndComunica + 1, 0 );
  delay(100);
  enviaCommando( 0, 1, cEndComunica + 1, 0 );
}

void enviaCommando( byte endereco, byte operacao, byte porta, byte dado ) {
  serial2.write( endereco );  //Endereço do destinatário 
  serial2.write( operacao );  //Operação
  serial2.write( porta );     //Porta para manipular no destinatário
  serial2.write( dado );      //dado para gravar na porta: HIGH, LOW, 0...255,
  serial2.write('\n'); 
}

void setup() {
  Serial.begin( 9600 );
  Serial.println( "Comunicacao com varios arduinos" );
  Serial.println( "MASTER");
    
  serial2.begin( 115200 );  //Inicia porta comunicaçao serial entre arduinos  
  delay(100); //aguarda um tempo para todos os arduino estarem ligados
  
  //Como esse é o master envia primeiro endereço para o primeiro escravo
  cEndComunica = 1; //endereço do master  
  enviaEndereco();  
}

void loop() {
  //Destino, operacao, porta, dado
  enviaCommando( 2, 0, 4, 1 ); delay(100);    
  enviaCommando( 2, 0, 4, 0 ); delay(100);    
  enviaCommando( 3, 0, 4, 1 ); delay(100);  
  enviaCommando( 3, 0, 4, 0 ); delay(100);
  enviaCommando( 4, 0, 4, 1 ); delay(100);  
  enviaCommando( 4, 0, 4, 0 ); delay(100);        
}

