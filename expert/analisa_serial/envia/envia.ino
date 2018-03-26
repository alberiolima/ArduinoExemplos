unsigned long proxEnvio = 0;
const char cEnvio = 'U'; //B01010101=85

void setup() {
  Serial.begin( 110 );  
}

void loop() {
  if ( millis() >= proxEnvio ) {
    Serial.write(cEnvio);    
    proxEnvio += 1000;
  }
}
