#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Definição dos pinos analógicos para as velocidades
int vel1 = A0;
int vel2 = A1;
int speed1 = 0;
int speed2 = 0;

// buzzer
int buzzer = 10; //BUZZER (PINO DIGITAL 10)
// PINOS DO SENSOR ULTRASSÔNICO
int echoPin = 4; // Recebe a informação de volta
int trigPin = 2; // Envia o sinal

// Contadores de ultrapassagens
int ultrapassagem1 = 0;
int ultrapassagem2 = 0;

// Inicialização dos LCDs
LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x20, 16, 2);

// Vetor para armazenar as últimas 2 leituras de distância
long distancias[2] = {0, 0};

// Arrays para armazenar as médias móveis
int mediaMovelVelCarro1[10] = {0};
int mediaMovelVelCarro2[10] = {0};
int contagemLeituras = 0; // CONTADOR DE LEITURAS

unsigned long previousMillis = 0; // Variável para armazenar o tempo de execução anterior
const long interval = 2000; // Intervalo de tempo em milissegundos (2 segundos)

void setup() {

  // Inicializa o serial monitor
  Serial.begin(9600);

  // Configuração dos pinos do sensor ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicialização dos LCDs
  lcd1.init();
  lcd2.init();
  lcd1.backlight();
  lcd2.backlight();

  // ARRAYS DE FREQUÊNCIA E DE DELAY PARA O SOM DE MARIO
  int frequencies[] = {2637, 2637, 2637, 2093, 2637, 3136, 1568};
  int songdelay[] = {100, 100, 200, 200, 100, 200, 320};

  // TOCA O SOM DE INTRODUÇÃO DO MARIO
  for (int i = 0; i < 7; i++) {
    delay(songdelay[i]);
    tone(buzzer, frequencies[i]);
    delay(80); //DURAÇÃO DE CADA BEEP
    noTone(buzzer);
  }

  //INICIALIZA A ANIMAÇÃO DO DISPLAY
  Introdisplay();

  //ESCREVE O LOGO QUE DURA MOMENTÂNEAMENTE
  lcd1.setCursor(6, 0);
  lcd1.print("Mind");
  lcd1.setCursor(6, 1);
  lcd1.print("Sync");
  delay(1000);
  lcd1.clear();

  lcd2.setCursor(6, 0);
  lcd2.print("Mind");
  lcd2.setCursor(6, 1);
  lcd2.print("Sync");
  delay(1000);
  lcd2.clear();
}

void loop() {
  unsigned long currentMillis = millis(); // Pega o tempo atual

  // Leitura e ajuste das velocidades dos carros
  speed1 = analogRead(vel1) / 3;
  speed2 = analogRead(vel2) / 3;

  // Exibição dos LCDS
  lcd();

  // Medição da distância com o sensor ultrassônico
  long duration;
  long distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(4);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medida do tempo de ida e volta do som
  duration = pulseIn(echoPin, HIGH);
  // Cálculo da distância em centímetros com maior precisão
  distance = duration / 58.31;

  // Atualiza o vetor de distâncias
  distancias[1] = distancias[0]; // Move a última leitura para a posição anterior
  distancias[0] = distance; // Armazena a nova leitura na posição atual

  // Adiciona as leituras de velocidade à média móvel
  for (int i = 9; i > 0; i--) {
    mediaMovelVelCarro1[i] = mediaMovelVelCarro1[i - 1];
    mediaMovelVelCarro2[i] = mediaMovelVelCarro2[i - 1];
  }

  mediaMovelVelCarro1[0] = speed1;
  mediaMovelVelCarro2[0] = speed2;
  contagemLeituras++;

  // Verifica se foram feitas 10 leituras
  if (contagemLeituras >= 10) {
    // Calcula a média aritmética das últimas 10 leituras
    int somaVelCarro1 = 0;
    int somaVelCarro2 = 0;
    for (int i = 0; i < 10; i++) {
      somaVelCarro1 += mediaMovelVelCarro1[i];
      somaVelCarro2 += mediaMovelVelCarro2[i];
    }
    speed1 = somaVelCarro1 / 10;
    speed2 = somaVelCarro2 / 10;
  }

  // Verifica se a distância está diminuindo e é menor que 4 metros
  if (distancias[0] < distancias[1] && distancias[0] < 3) {
    // Se a velocidade do carro 1 for maior e a distância estiver diminuindo
    if (speed1 > speed2) {
      ultrapassagem1++;
      lcdClear();
      lcd1.setCursor(0, 0);
      lcd1.print("Corredor 1 ");
      lcd1.print("ultrapassou!");
      lcd2.setCursor(0, 0);
      lcd2.print("Corredor 1 ");
      lcd2.print("ultrapassou!");
      Serial.println("O corredor 1 fez uma ultrapassagem!");
      delay(1500);
      lcdClear();
      lcd();

      if (distancias[1] && distancias[0] <= 0) {
        lcd1.print("Corredor 1 ");
        lcd1.print("ultrapassou!");
        lcd2.setCursor(0, 0);
        lcd2.print("Corredor 1 ");
        lcd2.print("ultrapassou!");
        Serial.println("O corredor 1 fez uma ultrapassagem!");
      }

    } else if (speed2 > speed1) {
      ultrapassagem2++;
      lcdClear();
      lcd1.setCursor(0, 0);
      lcd1.print("Corredor 2 ");
      lcd1.print("ultrapassou!");
      lcd2.setCursor(0, 0);
      lcd2.print("Corredor 2 ");
      lcd2.print("ultrapassou!");
      Serial.println("O corredor 2 fez uma ultrapassagem!");
      delay(1500);
      lcdClear();
      lcd();

      if (distancias[1] && distancias[0] <= 0) {
        lcd2.setCursor(0, 0);
        lcd2.print("Corredor 2 ");
        lcd2.print("ultrapassou!");
        Serial.println("O corredor 2 fez uma ultrapassagem!");
        delay(1500);
      }
    }
  }

  // Pequeno atraso para evitar saturação do loop
  delay(4500);
  lcdClear();
}

void lcd() {
  lcd1.setCursor(0, 0);
  lcd1.print("vel1: ");
  lcd2.setCursor(0, 0);
  lcd2.print("vel2: ");
  lcd1.setCursor(5, 0);
  lcd1.print(speed1);
  lcd2.setCursor(5, 0);
  lcd2.print(speed2);
  lcd1.setCursor(0, 1);
  lcd1.print("passagens ");
  lcd1.setCursor(10, 1);
  lcd1.print(ultrapassagem1);
  lcd1.setCursor(9, 0);
  lcd2.setCursor(0, 1);
  lcd2.print("passagens ");
  lcd2.setCursor(10, 1);
  lcd2.print(ultrapassagem2);
}

void lcdClear() {
  lcd1.clear();
  lcd2.clear();
}

void Introdisplay() {
  //MOVE POSIÇÕES

  for (int positionCounter = 0; positionCounter < 9; positionCounter++) {
    if (positionCounter == 0) {
      lcd1.setCursor(positionCounter, 0);
      lcd1.print("d");
      lcd2.setCursor(positionCounter, 0);
      lcd2.print("d");

    }
    else if (positionCounter == 1) {
      lcd1.setCursor(positionCounter - 1, 0);
      lcd1.print("nd");
      lcd2.setCursor(positionCounter - 1, 0);
      lcd2.print("nd");

    }
    else if (positionCounter == 2) {
      lcd1.setCursor(positionCounter - 2, 0);
      lcd1.print("ind");
      lcd2.setCursor(positionCounter - 2, 0);
      lcd2.print("ind");


    }
    else if (positionCounter == 3) {
      lcd1.setCursor(positionCounter - 3, 0);
      lcd1.print("Mind");
      lcd2.setCursor(positionCounter - 3, 0);
      lcd2.print("Mind");
    }
    else {
      lcd1.setCursor(positionCounter - 3, 0);
      lcd1.print("Mind");
      lcd2.setCursor(positionCounter - 3, 0);
      lcd2.print("Mind");
    }
    lcd1.setCursor(15 - positionCounter, 1);
    lcd1.print("Sync");
    delay(100);
    lcd1.clear();
    
    lcd2.setCursor(15 - positionCounter, 1);
    lcd2.print("Sync");
    delay(100);
    lcd2.clear(); // Limpa o display para o próximo movimento
  }
}
