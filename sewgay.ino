#include <Ultrasonic.h>
#include <AFMotor.h>

#define trigPin 37
#define echoPin 39
#define trigPin2 38
#define echoPin2 36


AF_DCMotor motorE(1);
AF_DCMotor motorD(2);

Ultrasonic ultrasonicF(37, 39);
Ultrasonic ultrasonicT(38, 36);

int pinofrenteE = 8;
int pinofrenteD = 9;
int pinosensorE = 13;
int pinosensorD = 15;
int pinotrazE = 12;
int pinotrazD = 14;

int rangepretoFrente[2] = {600, 1023};
int rangebrancoFrente[2] = {0, 500};

int rangepreto[2] = {920, 1023};
int rangeverde[2] = {810, 880};
int rangebranco[2] = {0, 790};


int velocidade = 100;

//Método setup, executado uma vez ao ligar o Arduino.
void setup() {

  pinMode(echoPin, INPUT); // seta pino como entrada
  pinMode(trigPin, OUTPUT); // seta pino saída
  pinMode(echoPin2, INPUT); // seta pino como entrada
  pinMode(trigPin2, OUTPUT); // seta pino saída

}

void loop() {
  segway();
  verificaFitaVerde();
  verificaT();
  verificaObstaculo();
}


/*
   Função segue linha preta com os dois sensores centrais
*/
void segway() {

  int  sensorE = analogRead(pinofrenteE);
  int  sensorD = analogRead(pinofrenteD);
  motorE.setSpeed(velocidade);
  motorD.setSpeed(velocidade);

  if (ehPretoFrente(sensorE) && ehPretoFrente(sensorD) ) { // Dois sensores na linha preta
    motorE.run(FORWARD);
    motorD.run(FORWARD);
  }
  else if (ehPretoFrente(sensorE) && ehBrancoFrente(sensorD)) { // Apenas sensor esquerdo na linha preta, move motores direito
    motorE.run(RELEASE);
    motorD.run(FORWARD);
  }
  else if (ehBrancoFrente(sensorE)  && ehPretoFrente(sensorD)) { // Apenas sensor direito na linha preta, move motores esquerdo
    motorE.run(FORWARD);
    motorD.run(RELEASE);
  }
  else { // Nenhum sensor na linha preta
    motorE.run(FORWARD);
    motorD.run(FORWARD);
  }
}



/*
   Função verifica se há fita verde nos sensores dos canos e faz curva quando encontrado
*/
void verificaFitaVerde() {

  int sensorfrenteE = analogRead(pinofrenteE);
  int sensorfrenteD = analogRead(pinofrenteD);
  int  sensorE = analogRead(pinosensorE);
  int  sensorD = analogRead(pinosensorD);
  int  sensortrazE = analogRead(pinotrazE);
  int  sensortrazD = analogRead(pinotrazD);
  int  velocidadegiro =  90;

  //Vita verde do lado esquerdo
  if (  ehVerde(sensorE) && ehVerde(sensortrazE)  ) {

    motorE.run(RELEASE);
    motorD.run(RELEASE);
    motorE.setSpeed(velocidadegiro);
    motorD.setSpeed(velocidadegiro);
    delay(10);

    girarParaEsquerda();

  }
  else  if (  ehVerde(sensorD) && ehVerde(sensortrazD)  ) {
    motorE.run(RELEASE);
    motorD.run(RELEASE);
    motorE.setSpeed(velocidadegiro);
    motorD.setSpeed(velocidadegiro);
    delay(10);

    girarParaDireita();
  }
}




/*
   Função verifica curva T
*/
void verificaT() {

  int sensorfrenteE = analogRead(pinofrenteE);
  int sensorfrenteD = analogRead(pinofrenteD);
  int  sensorE = analogRead(pinosensorE);
  int  sensorD = analogRead(pinosensorD);
  int  velocidadegiro =  90;

  //Vita preta do lado esquerdo
  if (  ehPreto(sensorE) && ehBranco(sensorD) && ehBrancoFrente(sensorfrenteE) && ehBrancoFrente(sensorfrenteD)  ) {

    motorE.run(RELEASE);
    motorD.run(RELEASE);
    motorE.setSpeed(velocidadegiro);
    motorD.setSpeed(velocidadegiro);
    delay(10);

    // Recua o robo até fita branca
    motorE.run(BACKWARD);
    motorD.run(BACKWARD);
    while ( true ) {
      if (ehPreto(analogRead(pinosensorE)) ) {
        motorE.run(RELEASE);
        motorD.run(RELEASE);
        break;
      }
      delay(5);
    }

    girarParaEsquerda();

  }
  // Fita preta do lado direito
  else  if (   ehBranco(sensorE) && ehPreto(sensorD) && ehBrancoFrente(sensorfrenteE) && ehBrancoFrente(sensorfrenteD)  ) {
    motorE.run(RELEASE);
    motorD.run(RELEASE);
    motorE.setSpeed(velocidadegiro);
    motorD.setSpeed(velocidadegiro);
    delay(10);

    // Recua o robo até fita branca
    motorE.run(BACKWARD);
    motorD.run(BACKWARD);
    while ( true ) {
      if (ehPreto(analogRead(pinosensorD)) ) {
        motorE.run(RELEASE);
        motorD.run(RELEASE);
        break;
      }
      delay(5);
    }

    girarParaDireita();
  }
}



/*
   Gira para esquerda
*/
void girarParaEsquerda() {

  int  velocidadegiro =  90;
  motorE.setSpeed(velocidadegiro);
  motorD.setSpeed(velocidadegiro);

  //Gira para a esquerda até os dois sensors centrais sairem do preto
  motorE.run(BACKWARD);
  motorD.run(FORWARD);
  while ( true ) {
    if (ehBrancoFrente(analogRead(pinofrenteE)) &&  ehBrancoFrente(analogRead(pinofrenteD))) {
      break;
    }
    delay(5);
  }

  // Gira direita até sensor centrais chegarem no preto
  while ( true ) {
    if (  ehPretoFrente(analogRead(pinofrenteD))) {
      motorE.run(RELEASE);
      motorD.run(RELEASE);
      break;
    }
    delay(5);
  }

  // Gira para esquerda até sensor centrais chegarem no preto
  motorE.run(FORWARD);
  motorD.run(BACKWARD);
  while ( true ) {
    if (  ehPretoFrente(analogRead(pinofrenteE))) {
      motorE.run(RELEASE);
      motorD.run(RELEASE);
      break;
    }
    delay(5);
  }
}


/*
   Gira para direita
*/
void girarParaDireita() {

  int  velocidadegiro =  90;
  motorE.setSpeed(velocidadegiro);
  motorD.setSpeed(velocidadegiro);

  //Gira para a direita até os dois sensors centrais sairem do preto
  motorE.run(FORWARD);
  motorD.run(BACKWARD);
  while ( true ) {
    if (ehBrancoFrente(analogRead(pinofrenteE)) ) {
      break;
    }
    delay(5);
  }

  // Gira para a esquerda até um dos dois centrais chegarem no preto
  while ( true ) {
    if (ehPretoFrente(analogRead(pinofrenteE)) ||  ehPretoFrente(analogRead(pinofrenteD))) {
      motorE.run(RELEASE);
      motorD.run(RELEASE);
      break;
    }
    delay(5);
  }

  // Gira para direita até sensor centrais chegarem no preto
  motorE.run(RELEASE);
  motorD.run(BACKWARD);
  while ( true ) {
    if (  ehPretoFrente(analogRead(pinofrenteD))) {
      motorE.run(RELEASE);
      motorD.run(RELEASE);
      break;
    }
    delay(5);
  }

}

void verificaObstaculo() {
  int sensorfrenteE = analogRead(pinofrenteE);
  int sensorfrenteD = analogRead(pinofrenteD);
  int  velocidadesvio =  100;

  int distanciaF = ultrasonicF.Ranging(CM);
  int distanciaT = ultrasonicT.Ranging(CM);

  if ( distanciaF > 0 && distanciaF <= 7) {
    motorE.run(RELEASE);
    motorD.run(RELEASE);

    motorE.run(FORWARD);
    motorD.run(BACKWARD);
    delay(10);
    while (true) {
      if (ultrasonicT.Ranging(CM) > 0 && ultrasonicT.Ranging(CM) <= 7) {
        motorE.run(RELEASE);
        motorD.run(RELEASE);
        break;
      }
      while (true) {
        if (ultrasonicT.Ranging(CM) > 0 && ultrasonicT.Ranging(CM) <= 5) {

          motorE.run(FORWARD);
          motorD.run(FORWARD);
        }
        else if (ultrasonicT.Ranging(CM) > 5) {
          motorE.run(BACKWARD);
          motorD.run(FORWARD);
        }
        else if (!ehPretoFrente(sensorfrenteD)) {
          //coloca aqui para ele virar
          break;


        }
      }
    }

  }
}



/*
   Retorna true se valor estiver em um range branco
*/
int ehBrancoFrente(int valor) {
  if (valor >= rangebrancoFrente[0] && valor <= rangebrancoFrente[1] ) {
    return 1;
  }
  return 0;
}

/*
   Retorna true se valor estiver em um range preto
*/
int ehPretoFrente(int valor) {
  if (valor >= rangepretoFrente[0] && valor <= rangepretoFrente[1] ) {
    return 1;
  }
  return 0;
}


/*
   Retorna true se valor estiver em um range branco
*/
int ehBranco(int valor) {
  if (valor >= rangebranco[0] && valor <= rangebranco[1] ) {
    return 1;
  }
  return 0;
}

/*
   Retorna true se valor estiver em um range preto
*/
int ehPreto(int valor) {
  if (valor >= rangepreto[0] && valor <= rangepreto[1] ) {
    return 1;
  }
  return 0;
}


/*
   Retorna true se valor estiver em um range verde
*/
int ehVerde(int valor) {
  if (valor >= rangeverde[0] && valor <= rangeverde[1] ) {
    return 1;
  }
  return 0;
}



