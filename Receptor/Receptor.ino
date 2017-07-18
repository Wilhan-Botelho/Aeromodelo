#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define ACELERADORSAIDA 3
#define AILERONSAIDA 5
#define LEMESAIDA 6
#define PROFUNDORSAIDA 9

Servo acelerador, aileron, leme, profundor;
RF24 radio(7, 8); // CSN, CE
const byte address[6] = "0A0A1";
int vlrAcelerador = 0;
int vlrAileron    = 90;
int vlrLeme       = 90;
int vlrProfundor  = 90;
long tempoSemReceberComando       = 0;

void setup() {
  //radio
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //servos
  acelerador.attach(ACELERADORSAIDA);
  aileron.attach(AILERONSAIDA);
  leme.attach(LEMESAIDA);
  profundor.attach(PROFUNDORSAIDA);
  acelerador.write(vlrAcelerador);
  aileron.write(vlrAileron);
  leme.write(vlrLeme);
  profundor.write(vlrProfundor);
}

void reiniciaMedidas(){
  executaAcelerador(70);
  executaAileron(90);
  executaLeme(90);
  executaProfundor(90);
}

void executaAcelerador(int vlrTmp){
  if(vlrTmp != vlrAcelerador){
    vlrAcelerador = vlrTmp;
    acelerador.write( vlrAcelerador );
  }
}

void executaAileron(int vlrTmp){
  if(vlrTmp != vlrAileron){
    vlrAileron = vlrTmp;
    aileron.write( vlrAileron );
  }
}

void executaLeme(int vlrTmp){
  if(vlrTmp != vlrLeme){
    vlrLeme = vlrTmp;
    leme.write( vlrLeme );
  }
}

void executaProfundor(int vlrTmp){
  if(vlrTmp != vlrProfundor){
    vlrProfundor = vlrTmp;
    profundor.write( vlrProfundor );
  }
}

void processarDadosRecebidos(char dados[25]){
  String str(dados);
  executaAcelerador( str.substring(0, 3).toInt() );
  executaAileron( str.substring(3, 6).toInt() );
  executaLeme( str.substring(6, 9).toInt() );
  executaProfundor( str.substring(9, 12).toInt() );
}

void loop() {
  if (radio.available()) {
    char text[25] = "";
    radio.read(&text, sizeof(text));
    processarDadosRecebidos(text);
    tempoSemReceberComando = 0;
  }
  tempoSemReceberComando = tempoSemReceberComando + 100;

  if(tempoSemReceberComando > 12000){
    reiniciaMedidas();
    tempoSemReceberComando = 0;
  }
  delay(100);
}
