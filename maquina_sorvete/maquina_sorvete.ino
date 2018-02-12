/* 
 * Maquina de sorvete
 * 
 * Por Albeiro Lima (Brasil)
 * 12-02-2018
 * 
 * Funções
 * ~~~~~~~
 * 
 * setMotores => função que liga ou desliga os motores, liga a primeiro motor aguarda 5 segundos e liga o outro
 * ==========
 * 
 * postaMensagem => função que escreve a mensagem do LCD e também a mensagem de debbug
 * 
 * millisToStr => transforma millis em uma string no formato HH:MM:SS
 * 
 * testaBotoes => função que teste de algum botão foi acionado
 * 
 * acionaValvula => funcão que aciona a valvula e agenda seu desligamento
 *  
 */

#define motorMisturador      8 //Misturador
#define motorCongelador      9 //Congelador
#define pinoValvula         10 //Valcula
#define botaoRetirar        A0 //Para tudo para retirada do sorvete
#define botaoTurbo          A1 //Liga misturador e congelador por um tempo

#define tempoRotinaInicial      12000UL //20 minutos (1200000)
#define tempoValvulaligada       2000UL //9 segundos    (9000)
#define tempoAntesMotores        4000UL //40 segundos  (40000)
#define tempoMotoresAcionados    8000UL //1,5 minutos  (90000)
#define tempoTurbo              10000UL //10 minutos  (600000)
#define tempoRetirada            5000UL //5 minutos   (300000)

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

  postaMensagem( F("Boas vindas")  );
  setMotores( true ); //liga os motores
  
  //Aguarda processo inicial
  postaMensagem( F( "Aguarde processo inicial") );
  unsigned long tempoAguardar = tempoRotinaInicial;
  while ( tempoAguardar > 0 ) {
    delay(1000);    
    tempoAguardar -= 1000;
    postaMensagem( "Aguarde: " + millisToStr( tempoAguardar ) );
  }  
  setMotores( false ); //desliga os motores para dar inicio ao ciclo padrão

  //Inicia o processo padrão ligando a valvula e definindo a hora de desligar a valvula  
  postaMensagem( F("Iniciando cliclo padrao")  );
  Serial.println();
  acionaValvula();  
}

void loop() {
  
  //Testa botões
  if (!(TurboAcionado||RetirarAcionado)) { //Só testa os botões de não tiver turbo ou retirar ativado
    testaBotoes();  
    if (RetirarAcionado) {
      postaMensagem( F("Retirar acionado")  );      
      contatorRetirada++;
      Serial.println( ": " + String(contatorRetirada));
      setMotores( true );
      //define hora de desligar os motores
      horaDesligarMotores = millis() + tempoRetirada;
      Serial.print( "[retirada], desligar em " + millisToStr( horaDesligarMotores - millis() ));      
    }
    if (TurboAcionado) {
     postaMensagem( F("Turbo acionado")  );      
     acionaValvula();
    }
  }

  //Desliga valvula passado o tempo de acionamento
  if (( horaDesligarValvula > 0 ) &&( millis() >= horaDesligarValvula )) {
    horaDesligarValvula = 0;
    digitalWrite( pinoValvula, LOW );
    //define hora de liga os motores
    if (TurboAcionado) {
      horaLigarMotores = millis(); //liga o motor logo após desligar a valvula
    } else {
      horaLigarMotores = millis() + tempoAntesMotores; 
    }
    postaMensagem( F("Valvula desativada")  );      
  }

  //Liga motores
  if (( horaLigarMotores > 0 ) &&( millis() >= horaLigarMotores )){ 
    horaLigarMotores = 0;
    setMotores( true );
    //define hora de desligar os motores
    if (TurboAcionado) {
      horaDesligarMotores = millis() + tempoTurbo;
      Serial.print("[turbo]");
    } else {
      horaDesligarMotores = millis() + tempoMotoresAcionados;
    }
    Serial.print( ", desligar em " + millisToStr( horaDesligarMotores - millis() ));
  }

  //desliga motores
  if (( horaDesligarMotores > 0 ) && ( millis() >= horaDesligarMotores )) {    
    horaDesligarMotores = 0;        
    if (RetirarAcionado) {
      if ( digitalRead( botaoRetirar ) == HIGH ) {
        delay(5);
        if ( digitalRead( botaoRetirar ) == HIGH ) {
          RetirarAcionado = false;
        }
      }
    }
    if (RetirarAcionado) {
      //define nova hora de desligar os motores na retirada
      Serial.print( F("\n[retirada] reiniciando" ) );
      horaDesligarMotores = millis() + tempoRetirada;               
    } else {
      if (TurboAcionado) {
        Serial.print( F("\n[turbo] voltando ao ciclo normal" ) );
        TurboAcionado = false;
      }      
      setMotores( false );
      acionaValvula();      
    }    
  }
}

void postaMensagem( String mens ) {
  Serial.write('\n');
  Serial.print( mens );
}


void setMotores( boolean ligar ) {
  if ( ligar ) {
    postaMensagem( F( "Ligando motor 1 (ON) ") );
    digitalWrite( motorMisturador, HIGH ); 
    unsigned long tempoAguardar = 5000;
    while ( tempoAguardar > 0 ) {
      Serial.print(String(tempoAguardar/1000) + ", ");
      delay(1000);    
      tempoAguardar -= 1000;
    } 
    Serial.print( F( ": Ligando motor 2 (ON)") );
    digitalWrite( motorCongelador, HIGH );    
  } else { //desliga
    postaMensagem( F( "Motores desligados (OFF)") ); 
    digitalWrite( motorMisturador, LOW );     
    digitalWrite( motorCongelador, LOW );       
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
  if (( btLido == LOW )&&(btTurboAntes == HIGH)) {
    delay(5);
    if ( digitalRead( botaoTurbo ) == LOW ) {
      TurboAcionado = true;
    }    
  }
  btTurboAntes = btLido;  

  btLido = digitalRead( botaoRetirar );
  if (( btLido == LOW )&&(btRetirarAntes == HIGH)) {
    delay(5);
    if ( digitalRead( botaoRetirar ) == LOW ) {
      RetirarAcionado = true;
    }    
  }
  btRetirarAntes = btLido;  

  if (TurboAcionado||RetirarAcionado) {
    //desabilita as oprações padrao
    horaLigarMotores = 0;
    horaDesligarMotores = 0;
    horaDesligarValvula = 0;
  }    
}

void acionaValvula(){
  //Aciona a valvula e programa hora de desligar
  digitalWrite( pinoValvula, HIGH );
  horaDesligarValvula = millis() + tempoValvulaligada;
  postaMensagem( F("Valvula ligada")  );
  Serial.print( ",desligar em " + millisToStr( tempoValvulaligada ) );
}
