/*
   Maquina de sorvete

   Por Albeiro Lima (Brasil)
   12-02-2018

   Funções
   ~~~~~~~

   setMotores => função que liga ou desliga os motores, liga a primeiro motor aguarda 5 segundos e liga o outro
   ==========

   millisToStr => transforma millis em uma string no formato HH:MM:SS
   ===========

   testaBotoes => função que teste de algum botão foi acionado
   ===========

   acionaValvula => funcão que aciona a valvula e agenda seu desligamento
   =============

*/

#define motorMisturador      8 //Misturador
#define motorCongelador      9 //Congelador
#define pinoValvula         10 //Valvula
#define botaoRetirar        A0 //Para tudo para retirada do sorvete
#define botaoTurbo          A1 //Liga misturador e congelador por um tempo

#define tempoRotinaInicial      30000UL //20 minutos (1200000)
#define tempoValvulaligada       5000UL //9 segundos    (9000)
#define tempoAntesMotores        4000UL //40 segundos  (40000)
#define tempoMotoresAcionados    8000UL //1,5 minutos  (90000)
#define tempoTurbo              12000UL //10 minutos  (600000)
#define tempoRetirada            6000UL //5 minutos   (300000)

unsigned long horaLigarMotores = 0;
unsigned long horaDesligarMotores = 0;
unsigned long horaDesligarValvula = 0;
boolean TurboAcionado = false;
boolean RetirarAcionado = false;
unsigned int contatorRetirada = 0;

void setup() {
  //Inicializa comunicação serial para debbug
  Serial.begin( 9600 );
  Serial.println();

  //Configura portas
  pinMode( motorMisturador, OUTPUT );
  pinMode( motorCongelador, OUTPUT );
  pinMode( pinoValvula, OUTPUT );
  pinMode( botaoTurbo, INPUT_PULLUP);
  pinMode( botaoRetirar, INPUT_PULLUP);

  Serial.print( F("\n[Boas vindas]")  );
  Serial.print( F( "\n[INICIANDO]\n") );
  setMotores( true ); //liga os motores
  Serial.print( F( "\n" ) );
  //Aguarda processo inicial
  unsigned long tempoAguardar = tempoRotinaInicial;
  while ( tempoAguardar > 0 ) {
    Serial.print( "\nRESTA: " + millisToStr( tempoAguardar ) + " " );
    for ( byte i = 0;i<60&&tempoAguardar > 0;i++){
      Serial.print( "*" );      
      delay(1000);
      tempoAguardar -= 1000;      
    }
    
  }
  setMotores( false ); //desliga os motores para dar inicio ao ciclo padrão

  //Inicia o processo padrão ligando a valvula e definindo a hora de desligar a valvula
  Serial.print( F("\n\nIniciando cliclo padrao")  );
  Serial.println();
  acionaValvula();
}

void loop() {

  //Testa botões
  if (!(TurboAcionado || RetirarAcionado)) { //Só testa os botões se não tiver turbo ou retirar ativado
    testaBotoes(); //ler os botões
    if (RetirarAcionado) { //Acionando retirada
      contatorRetirada++;
      Serial.print( F("\n\nRETIRADA [ON]")  );
      Serial.println( ": " + String(contatorRetirada));      
      setMotores( true ); //liga motores       
      horaDesligarMotores = millis() + tempoRetirada; //define hora de desligar os motores
      Serial.print( "\nRETIRADA " + millisToStr( horaDesligarMotores - millis() ) );
    } else if (TurboAcionado) { //Acionando turbo
      Serial.print( F("\n\nTURBO [ON]")  );
      acionaValvula();
    }
  }

  //Desliga valvula conforme tempo agendado
  if (( horaDesligarValvula > 0 ) && ( millis() >= horaDesligarValvula )) {
    horaDesligarValvula = 0;
    digitalWrite( pinoValvula, LOW );
    Serial.print( F("\nVALVULA [OFF] ")  );
    //define hora de liga os motores
    if (TurboAcionado) {
      horaLigarMotores = millis(); //liga o motor logo após desligar a valvula quando turbo acionado
    } else {
      horaLigarMotores = millis() + tempoAntesMotores;
      Serial.print( F("\nPAUSA ") );
      Serial.print( millisToStr( horaLigarMotores - millis() ) );
    }    
  }

  //Liga motores
  if (( horaLigarMotores > 0 ) && ( millis() >= horaLigarMotores )) {
    horaLigarMotores = 0;
    setMotores( true );
    //define hora de desligar os motores
    if (TurboAcionado) {
      horaDesligarMotores = millis() + tempoTurbo;
      Serial.print(F("\nTURBO "));
    } else {
      horaDesligarMotores = millis() + tempoMotoresAcionados;
      Serial.print( F( "\nMOTORES " ) );
    }
    Serial.print( millisToStr( horaDesligarMotores - millis() ));
  }

  //desliga motores
  if (( horaDesligarMotores > 0 ) && ( millis() >= horaDesligarMotores )) {
    horaDesligarMotores = 0;
    if (RetirarAcionado) {
      if ( digitalRead( botaoRetirar ) == HIGH ) {
        delay(5);
        if ( digitalRead( botaoRetirar ) == HIGH ) {
          Serial.print( F("\nRETIRADA [OFF]")  );
          RetirarAcionado = false;
        }
      }
    }
    if (RetirarAcionado) {
      //define nova hora de desligar os motores na retirada
      Serial.print( F("\nRETIRADA [REINICIO]")  );
      horaDesligarMotores = millis() + tempoRetirada;
    } else {
      if (TurboAcionado) {
        Serial.print( F("\nTURBO [OFF]" ) );
        TurboAcionado = false;
      }
      setMotores( false );
      Serial.print( F("\n" ) );
      acionaValvula();
    }
  }
}

void setMotores( boolean ligar ) {
  if ( ligar ) {
    digitalWrite( motorMisturador, HIGH );
    Serial.print( F("\nMOTOR(1) [ON] ") );
    unsigned long tempoAguardar = 5000;
    while ( tempoAguardar > 0 ) {
      Serial.print( F("*" ) );
      delay(1000);
      tempoAguardar -= 1000;
    }
    digitalWrite( motorCongelador, HIGH );
    Serial.print( F( " MOTOR(2) [ON]") );    
  } else { //desliga
    digitalWrite( motorMisturador, LOW );
    digitalWrite( motorCongelador, LOW );
    Serial.print( F( "\nMOTORES [OFF]") );
  }
}

String millisToStr( unsigned long ulTempo ) {
  int iHoras = 0;
  int iMinutos = 0;
  ulTempo /= 1000;
  if ( ulTempo >= 3600 ) {
    iHoras = (int)(ulTempo / 3600);
    ulTempo -= ( (unsigned long)iHoras * 3600UL );
  }
  if ( ulTempo >= 60 ) {
    iMinutos = (int)(ulTempo / 60);
    ulTempo -= ( (unsigned long)iMinutos * 60UL );
  }
  return ( String( iHoras) + ":" + String( iMinutos ) + ":" + String( ulTempo) );
}

void testaBotoes() {
  static byte btTurboAntes = LOW;
  static byte btRetirarAntes = LOW;

  byte btLido = digitalRead( botaoTurbo );
  if (( btLido == LOW ) && (btTurboAntes == HIGH)) {
    delay(5);
    if ( digitalRead( botaoTurbo ) == LOW ) {
      TurboAcionado = true;
    }
  }
  btTurboAntes = btLido;

  btLido = digitalRead( botaoRetirar );
  if (( btLido == LOW ) && (btRetirarAntes == HIGH)) {
    delay(5);
    if ( digitalRead( botaoRetirar ) == LOW ) {
      RetirarAcionado = true;
    }
  }
  btRetirarAntes = btLido;

  if (TurboAcionado || RetirarAcionado) {
    //desabilita as oprações padrao
    horaLigarMotores = 0;
    horaDesligarMotores = 0;
    horaDesligarValvula = 0;
  }
}

void acionaValvula() {
  //Aciona a valvula e programa hora de desligar
  digitalWrite( pinoValvula, HIGH );
  horaDesligarValvula = millis() + tempoValvulaligada;
  Serial.print( F("\nVALVULA [ON] ")  );
  Serial.print( millisToStr( tempoValvulaligada ));
}
