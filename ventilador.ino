#include <Adafruit_NeoPixel.h>

// C++ code
//

#define LEDSTRIP_ANDARES_PIN  4
#define LEDS_COUNT  24
Adafruit_NeoPixel strip(LEDS_COUNT, LEDSTRIP_ANDARES_PIN, NEO_GRB + NEO_KHZ800);
int botoes = A0;
int interruptBotoes = 2;
int velocidadeMax = 10;
int pontoDesligamento = 2000;
int velocidadeAtual = pontoDesligamento; // velocidade inicial tb

enum Estados{DESLIGADO, HORARIO, ANTIHORARIO,DESACELERANDO};

enum Estados estadoVent = DESLIGADO;
enum Estados estadoProx = DESLIGADO; // so vai ser usado qnd estiver desacelerando e chegar no ponto de desligamento
enum Estados direcao = DESLIGADO;

void setup()
{
  Serial.begin(9600);
  pinMode(botoes, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptBotoes), botao, RISING);
}

int posLed = 0;
void loop(){
  strip.clear();
  strip.setPixelColor(posLed % LEDS_COUNT, 0, 0, 255);
  strip.show();
  switch(estadoVent){
    case DESACELERANDO:
    velocidadeAtual += pontoDesligamento*0.1;
    if(velocidadeAtual >= pontoDesligamento){
      velocidadeAtual = pontoDesligamento/2;
      estadoVent = estadoProx;
      if(estadoVent == DESLIGADO){
        Serial.println("Desligado");
      }
      direcao = estadoProx;
      estadoProx = DESLIGADO;
    }
    break;
    case HORARIO:
    velocidadeAtual -= pontoDesligamento*0.1;
    break;
    case ANTIHORARIO:
    velocidadeAtual -= pontoDesligamento*0.1;
    break;
  }
  if(velocidadeAtual < velocidadeMax){
    velocidadeAtual = velocidadeMax;
  }else if(velocidadeAtual > pontoDesligamento){
    velocidadeAtual = pontoDesligamento;
  }
  delay(velocidadeAtual);
  if(direcao == HORARIO){posLed++;}
  else if(direcao == ANTIHORARIO){posLed--;if(posLed <= -1){posLed = LEDS_COUNT;}}
}

void botao(){
  switch(analogRead(botoes)){
    case 1022:
    if(estadoVent != HORARIO){
      Serial.println("Girando sentido horario...");
      estadoVent = DESACELERANDO;
      estadoProx = HORARIO;
    }
    break;
    case 1003:
    if(estadoVent != ANTIHORARIO){
      Serial.println("Girando sentido anti-horario...");
      estadoVent = DESACELERANDO;
      estadoProx = ANTIHORARIO;
    }
    break;
    case 984:
    Serial.println("Desligando...");
    estadoVent = DESACELERANDO;
    estadoProx = DESLIGADO;
    break;
  }
}	