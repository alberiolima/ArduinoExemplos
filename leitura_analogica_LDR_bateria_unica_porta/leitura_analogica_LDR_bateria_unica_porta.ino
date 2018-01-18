#define pinAtivaLDR  2 //porta digital que aciona a leitura do LDR
#define pinAtivaBAT  3 //porta digital que aciona a leitura da bateria
#define pinAnalogico A0 //Unica porta usada para leitura

boolean lerLDR;
void setup() {
  Serial.begin( 9600 );
  pinMode( pinAtivaBAT, OUTPUT );
  pinMode( pinAnalogico, INPUT );
  pinMode( pinAtivaLDR, OUTPUT );
  
  //Inicia lendo LDR
  lerLDR = true;
  digitalWrite( pinAtivaLDR, HIGH);
  digitalWrite( pinAtivaBAT, LOW);  
}

void loop() {
  if ( Serial.available() > 0 ) {
    Serial.read();
    lerLDR = !lerLDR;
    if (lerLDR){
      digitalWrite( pinAtivaBAT, LOW );
      pinMode( pinAtivaLDR, OUTPUT );
      digitalWrite( pinAtivaLDR, HIGH );
    } else {
      pinMode( pinAtivaLDR, INPUT );
      digitalWrite( pinAtivaBAT, HIGH );      
    }
  }
  int valLido = analogRead( pinAnalogico );
  Serial.print(  valLido );
  Serial.print( " " );
  Serial.print(  (float)valLido * (5.0 / 1024.0), 2 );
  Serial.println();
  delay(50);
}
