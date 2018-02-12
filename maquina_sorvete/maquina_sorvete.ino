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
 * postaMensagem => função que escreve a mensagem do LCD e também amensagem de debbug
 * 
 * millisToStr => transforma millis em uma string no formato HH:MM:SS
 *  
 */

#define motorMisturador      8 //Misturador
#define motorCongelador      9 //Congelador
#define acionaValvula       10 //Valcula
#define botaoRetirar        A0 //Para tudo para retirada do sorvete
#define botaoTurbo          A1 //Liga misturador e congelador por um tempo

#define tempoRotinaInicial      20000 //20 minutos
#define tempoValvulaligada       2000 //20 segundos
#define tempoAntesMotores        4000 //40 segundos
#define tempoMotoresAcionados    8000 //1,5 minutos

unsigned long horaLigarMotores = 0;
unsigned long horaDesligarMotores = 0;
unsigned long horaDesligarValvula = 0;

void setup() {
  //Inicializa comunicação serial para debbug
  Serial.begin( 9600 );
  Serial.println();

  //Configura portas
  pinMode( motorMisturador, OUTPUT );
  pinMode( motorCongelador, OUTPUT );
  pinMode( acionaValvula, OUTPUT );

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
  digitalWrite( acionaValvula, HIGH );
  horaDesligarValvula = millis() + tempoValvulaligada;
  postaMensagem( F("Valvula ligada")  );
  
}

void loop() {
  
  //[PADRAO] Desliga valvula passado o tempo de acionamento
  if (( horaDesligarValvula > 0 ) &&( millis() >= horaDesligarValvula )) {
    horaDesligarValvula = 0;
    digitalWrite( acionaValvula, LOW );
    //define hora de liga os motores
    horaLigarMotores = millis() + tempoAntesMotores; 
    postaMensagem( F("Valvula desativada")  );      
  }

  //[PADRAO] Liga motores
  if (( horaLigarMotores > 0 ) &&( millis() >= horaLigarMotores )){ 
    horaLigarMotores = 0;
    setMotores( true );
    //define hora de desligar os motores
    horaDesligarMotores = millis() + tempoMotoresAcionados;
    Serial.print( ", desligar em " + millisToStr( tempoMotoresAcionados ));
  }

  //[PADRAO] desliga motores
  if (( horaDesligarMotores > 0 ) && ( millis() >= horaDesligarMotores )) {    
    setMotores( false );    
    horaDesligarMotores = 0;
    
    //Aciona a valvula e programa hora de desligar
    digitalWrite( acionaValvula, HIGH );
    horaDesligarValvula = millis() + tempoValvulaligada;
    postaMensagem( F("Valvula ligada")  );
    Serial.print( ",desligar em " + millisToStr( tempoValvulaligada ) );
  }
}

void postaMensagem( String mens ) {
  Serial.println();
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


