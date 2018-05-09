/*
 * Acionamento de 8 lampadas com 2 botões de toque
 * 
 * A quantidade de toques em cada botão indica qual lampada irá acender
 * 
 * O botão1 é identificado no código por botoes[0]
 * O botão2 é identificado no código por botoes[1]
 * Os reles[0] indica a lampada1 e reles[1] a lampada2, o restante seguem o padrão
 * 
 * O botão1 aciona relé[0] e relé[1], que são as lampadas 1 e 2
 * Um toque no botão1 aciona a lampada 1, dois toque a lampada 2, mais de 2 toques, apaga as lampada1 e 2 se ambas acesas, acende lampada1 e 2 se ambas apagadas
 * 
 * O botão2 aciona os relés[2..7], lampadas de 3 a 8
 * Um toque no botão2 aciona a lampada3, dois toques a lampada4 e assim até 6 toque que aciona a lampada 8
 * mais que 6 toques inverte a lampada3 e coloca o mesmo estado no resto das lampadas,
 * se a lampada3 estiver apagada, acende ele e as de 4 a 8, se estiver acesa apaga ela e de 4 a 8
 * 
 */
#define tempoComando 2000 //2segundos

typedef struct {
  byte pino = 0;
  byte estado = LOW;
  byte estadoAnterior = LOW;
  byte contaPulso = 0;
  unsigned long tempo = 0;
} botao;

botao botoes[2];

byte reles[8] = { 2,3,4,5,6,7,8,9 };

//Método setup, executado uma vez ao ligar o Arduino.
void setup() {
  botoes[0].pino = 10;
  botoes[1].pino = 11;
  
  Serial.begin(9600);
  pinMode(botoes[0].pino, INPUT);
  pinMode(botoes[1].pino, INPUT);

  for ( byte i = 0; i <8; i++){
    pinMode(reles[i], OUTPUT);
    digitalWrite(reles[i], HIGH);
  }
}

void loop() {
  
    //Testa botoes
  for ( byte j = 0; j<2;j++){
    botoes[j].estado = digitalRead(botoes[j].pino);
    if ((botoes[j].estado == HIGH )&&( botoes[j].estado != botoes[j].estadoAnterior)) {
      delay(20);
      botoes[j].estado = digitalRead(botoes[j].pino);
      if ((botoes[j].estado == HIGH )&&( botoes[j].estado != botoes[j].estadoAnterior)) {
        botoes[j].contaPulso++;
        botoes[j].tempo = millis();
      }    
    }
    botoes[j].estadoAnterior = botoes[j].estado;  
  }

  //Ação para botão1
  if (( botoes[0].tempo > 0 )&&( millis() > botoes[0].tempo + tempoComando )) {

    Serial.print("TOTAL PULSO BOTAO 01 : [ ");
    Serial.print(botoes[0].contaPulso);
    Serial.println(" ]");

    if ( botoes[0].contaPulso == 1 ) {
      digitalWrite( reles[0], !digitalRead( reles[0] ) ); //inverte relé1
    } else if (botoes[0].contaPulso == 2) {
      digitalWrite( reles[1], !digitalRead( reles[1] ) ); //inverte relé2
    } else if ( botoes[0].contaPulso > 2 ) {
      if (digitalRead( reles[0] ) == digitalRead( reles[1] ) ) { //Se rele1 e rele2 estiverem ON torna off, se estiverem off torna ON.
        digitalWrite( reles[0], !digitalRead( reles[0] ) );
        digitalWrite( reles[1], !digitalRead( reles[1] ) );
      }
    }
    
    botoes[0].tempo = 0;
    botoes[0].contaPulso = 0;
  }

  //Ação para botão 2
  if (( botoes[1].tempo > 0 )&&( millis() > botoes[1].tempo + tempoComando )) {
    Serial.print("TOTAL PULSO BOTAO 02 : [ ");
    Serial.print(botoes[1].contaPulso);
    Serial.println(" ]");

    if (( botoes[1].contaPulso > 0 )&&(botoes[1].contaPulso < 7)) {
      digitalWrite( reles[botoes[1].contaPulso+1], !digitalRead( reles[botoes[1].contaPulso+1] ) );
    } else if ( botoes[1].contaPulso > 6 ) {
      digitalWrite( reles[2], !digitalRead( reles[2] ) );
      for ( byte k=3;k<8;k++){
        digitalWrite( reles[k], digitalRead( reles[2] ) );
      }    
    }
    botoes[1].tempo = 0;
    botoes[1].contaPulso = 0;
  }  
}
