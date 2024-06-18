# Sistema de Monitoramento de Velocidade e Ultrapassagem de Veículos
Este projeto consiste em um sistema de monitoramento de velocidade e ultrapassagem de veículos, utilizando sensores ultrassônicos e displays LCD. O sistema é capaz de detectar a velocidade de dois carros e monitorar quando um veículo ultrapassa o outro. Além disso, emite um alerta sonoro quando uma ultrapassagem é detectada.

Componentes Utilizados
2 Displays LCD 16x2 com I2C
1 Sensor Ultrassônico (HC-SR04)
1 Buzzer
2 Sensores de Velocidade (Potenciômetros conectados aos pinos A0 e A1 do Arduino)
Arduino UNO R3

## Configuração do Hardware
O circuito é montado de acordo com as seguintes conexões entre o Arduino UNO R3 e os componentes adicionais:

Pino A0: Conectado ao sensor de velocidade 1
Pino A1: Conectado ao sensor de velocidade 2
Pino 10: Conectado ao Buzzer
Pino 4: Conectado ao Echo do sensor ultrassônico
Pino 2: Conectado ao Trig do sensor ultrassônico
Pinos I2C (A4, A5): Conectados aos displays LCD

## Funcionamento do Software
O código embarcado no Arduino é responsável por ler os valores dos sensores de velocidade, calcular a média móvel das velocidades e detectar ultrapassagens com base na leitura do sensor ultrassônico. O sistema opera da seguinte maneira:

## Configuração Inicial
- Inicializa os sensores, os displays e o serial monitor.
- Executa uma animação de introdução nos displays LCD.
- Toca um som de introdução usando o buzzer.

## Loop Principal
- Lê os valores dos sensores de velocidade e ajusta as velocidades dos carros.
- Mede a distância usando o sensor ultrassônico.
- Calcula a média móvel das últimas 10 leituras de velocidade para suavizar os dados.
- Verifica se a distância está diminuindo e é menor que 3 metros.
- Determina se ocorreu uma ultrapassagem com base nas velocidades e nas leituras de distância.
- Atualiza os displays com as informações de velocidade e contagem de ultrapassagens.
- Aguarda 4.5 segundos antes da próxima leitura para evitar saturação do loop.
