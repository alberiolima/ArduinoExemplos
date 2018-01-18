/* 
 * Exemplo de como ler dois dispositivos na mesma porta analógica
 * Aqui estamos lendo um LDR e a tensão da bateria que alimenta o sistema
 */
#define pinAtivaLDR  2 //porta digital que aciona a leitura do LDR
#define pinAtivaBAT  3 //porta digital que aciona a leitura da bateria
#define pinAnalogico A0 //Unica porta usada para leitura

boolean lerLDR;
void setup() {
  Serial.begin( 9600 );
  pinMode( pinAnalogico, INPUT );  //Configura a porta de leitura
  pinMode( pinAtivaBAT, OUTPUT );  //Configura porta que ativa a leitura da bateria como saída
  pinMode( pinAtivaLDR, OUTPUT );  //Configura porta que ativa a leitura do LDR como saída
  
  //Inicia lendo LDR
  lerLDR = true;
  digitalWrite( pinAtivaLDR, HIGH); //Leitura do LDR ativada
  digitalWrite( pinAtivaBAT, LOW);  //Leitura da bateria desativada
}

void loop() {
  if ( Serial.available() > 0 ) { //Quando qualquer coisa vier pela serial é feita a alternancia entre LDR e bateria
    while ( Serial.available() > 0 ) {
      Serial.read();
    }
    lerLDR = !lerLDR;a 
    if (lerLDR){ //Ativa leitura LDR
      digitalWrite( pinAtivaBAT, LOW );
      pinMode( pinAtivaLDR, OUTPUT );
      digitalWrite( pinAtivaLDR, HIGH );
    } else { //Ativa leitura bateria
      pinMode( pinAtivaLDR, INPUT );
      digitalWrite( pinAtivaBAT, HIGH );      
    }
  }
  //Mostra os dados pela Serial
  int valLido = analogRead( pinAnalogico );
  Serial.print(  valLido );
  Serial.print( " " );
  Serial.print(  (float)valLido * (5.0 / 1024.0), 2 );
  Serial.println();
  delay(50);
}
