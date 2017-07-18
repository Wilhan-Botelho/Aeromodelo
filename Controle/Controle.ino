#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define ACELERADORPINENTRADA 0
#define AILERONPINENTRADA 1
#define LEMEPINENTRADA 2
#define PROFUNDORPINENTRADA 3

RF24 radio(7, 8); // CSN, CE
const byte address[6] = "0A0A1";
int vlrAcelerador = 0;
int vlrAileron, vlrLeme, vlrProfundor = 90;
int enviaDados = 0;

void setup() {
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void lerAcelerador(){
  int vlrTmp = map( analogRead(ACELERADORPINENTRADA), 0, 1024, 0, 179);
  if(vlrTmp != vlrAcelerador){
    vlrAcelerador = vlrTmp;
    enviaDados += 1;
  }
}

void lerAileron(){
  int vlrTmp = map( analogRead(AILERONPINENTRADA), 0, 1024, 125, 55);
  if(vlrTmp != vlrAileron){
    vlrAileron = vlrTmp;
    enviaDados += 1;
  }
}

void lerLeme(){
  int vlrTmp = map( analogRead(LEMEPINENTRADA), 0, 1024, 60, 120);
  if(vlrTmp != vlrLeme){
    vlrLeme = vlrTmp;
    enviaDados += 1;
  }
}

void lerProfundor(){
  int vlrTmp = map( analogRead(PROFUNDORPINENTRADA), 0, 1024, 120, 60);
  if(vlrTmp != vlrProfundor){
    vlrProfundor = vlrTmp;
    enviaDados += 1;
  }
}

String zeroaesquerda(int vlr){
  String valor = String(vlr);
  for(int i=valor.length(); i <= 2; i++)
    valor = "0" + valor;
  return valor;
}

void enviaComando(){
  if(enviaDados > 0){
    String dados = zeroaesquerda(vlrAcelerador) + zeroaesquerda(vlrAileron) + zeroaesquerda(vlrLeme) + zeroaesquerda(vlrProfundor);
    const char text[13];
    dados.toCharArray(text, 13);
    radio.write(&text, sizeof(text));
    enviaDados = 0;
  }
}

void loop() {
  lerAcelerador();
  lerAileron();
  lerLeme();
  lerProfundor();
  enviaComando();
  delay(100);
}
