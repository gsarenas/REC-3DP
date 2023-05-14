#include "REC_3DP_Config.h"

// Leitura ADC vs temperatura correspondente
/*
   "Temp table" disponibilizada no codigo-fonte do firmware para impressoras 3D "Marlin"
   Github: <https://github.com/MarlinFirmware/Marlin/blob/bugfix-2.1.x/Marlin/src/module/thermistor/thermistor_1.h>
   Entrada para sinal do termistor configurada com resistor pull-up de 4,7k
*/
const int adcTable[] = {23, 25, 27, 28, 31, 33, 35, 38, 41, 44, 48, 52, 56, 61, 66, 71, 78, 84, 92, 100, 109, 120, 131, 143, 156, 171, 187, 205, 224, 245, 268, 293, 320, 348, 379, 411, 445, 480, 516, 553, 591, 628, 665, 702, 737, 770, 801, 830, 857, 881, 903, 922, 939, 954, 966, 977, 985, 993, 999, 1004, 1008, 1012, 1016, 1020};
const int tempTable[] = {300, 295, 290, 285, 280, 275, 270, 265, 260, 255, 250, 245, 240, 235, 230, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0, -5, -10, -15};

// Temperatura maxima de trabalho permitida
int maxTempAllowed = 240;

// PV: temperatura atual
float tempValue = 0;
float lastPV = 0;
float lastTempValue = 0;

// SP: temperatura desejada
int targetTemp = 0;
int lastTargetTemp = 0;

// Constantes PID
float Kp = 18; // Ganho proporcional
float Ki = 0.01; // Ganho integral
float Kd = 160; // Ganho derivativo

//PID CLASSICO ZIEGLER-NICHOLS
/*float Kp = 21; // Ganho proporcional
  float Ki = 1.448; // Ganho integral
  float Kd = 76.125; // Ganho derivativo*/

// Variaveis de controle

float pid_input = 0; // Temperatura atual
float pid_output = 0; // Sinal de controle PID

// Variaveis para calculo do PID

float proportional = 0;
float error = 0;
float last_error = 0;
float derivative = 0;
float derivative_error = 0;
float integral = 0;
float integral_error = 0;

// Auxiliar: tempo rodando o programa atual na placa para desligar o aquecimento no tuning de PID
long currMillis_Temperature = 0;

// Inicializacao do Timer3 para aquisicao de temperatura
void Init_Timer3() {
  noInterrupts();

  // Operacao normal
  TCCR3A = 0x00;
  // Reset registrador
  TCCR3B = 0x00;

  /* Pre-carga do timer3 = (34286)_10  para int a cada 0,5s
     f_target = f_cpu / (prescaler*(2^n-preload))
     2 = 16*10E6 / (256*(65536-x))
  */
  TCNT3 = 0x85EE;

  // Prescaler 256
  TCCR3B |= (1 << CS32);

  // Habilita interrupcao por overflow
  TIMSK3 |= (1 << TOIE3);

  interrupts();
}


// Setup dos MOSFETs de potencia
void Init_MOSFETs() {
  pinMode(HOTEND_PIN, OUTPUT);
  pinMode(HEATED_BED_PIN, OUTPUT);
  pinMode(FANS_PIN, OUTPUT);

  // Abre os circuitos dos MOSFETs

  digitalWrite(HEATED_BED_PIN, 0); // PCB_ID: M1 -> nao mudar, nao e utilizado
  digitalWrite(HOTEND_PIN, 0); // PCB_ID: M2
  digitalWrite(FANS_PIN, 0); // PCB_ID: M3 -> nao mudar, nao e utilizado
}


// Recebe o valor da porta analogica e converte para temperatura
float analog2temp() {
  // Faz a leitura ADC
  int adcValue = analogRead(THERMISTOR_PIN);
  // Encontra o valor ADC mais proximo na tabela
  int i = 0;
  while (adcValue > adcTable[i] && i < 127 && adcValue > 20 && adcValue < 1023) {
    i++;
  }
  // Interpola o valor da temperatura
  if (i == 0) {
    tempValue = tempTable[0];
  } else if (i == 127) {
    tempValue = tempTable[127];
  } else {
    float slope = (tempTable[i] - tempTable[i - 1]) / (float)(adcTable[i] - adcTable[i - 1]);
    float intercept = tempTable[i] - slope * adcTable[i];
    return tempValue = slope * adcValue + intercept;
  }
}


// Controle o MOSFET de potencia do hotend baseado no PID calculado pela rotina de interrupcao do Timer3
void manage_heater() {

  //Atualiza a saida para controlar a temperatura dentro dos limites pre-estabelecidos
  if (tempValue > maxTempAllowed) analogWrite(HOTEND_PIN, 0);
  else analogWrite(HOTEND_PIN, int(pid_output));

  if (COOLING_ENABLED) {
    // Recurso de resfriamento
    if (targetTemp == 0 && tempValue > 30) {
      analogWrite(HEATED_BED_PIN, 255);
    } else analogWrite(HEATED_BED_PIN, 0);
  }

  if (PID_TUNING_ENABLED) {
    currMillis_Temperature = millis(); // Atualiza o tempo rodando o programa atual na placa
    if (currMillis_Temperature >= 315000 && targetTemp != 0) {
      if (DEBUG_ENABLED) DEBUG_PRINTLN("Stop!");
      if (COOLING_ENABLED) analogWrite(HEATED_BED_PIN, 255);
      targetTemp = 0;
    }
  }
}

// Calcula o PID
void compute_PID() {
  // Recebe como entrada do PID a temperatura atual
  pid_input = tempValue;

  // Calcula o erro
  error = targetTemp - pid_input;
  proportional = Kp * error;

  // Calcula o termo integral
  integral_error += error;
  integral = Ki * integral_error;

  // Calcula o termo derivativo
  derivative_error = error - last_error;
  derivative = Kd * derivative_error;

  // Calcula o sinal de controle usando a formula de PID
  pid_output = proportional + integral + derivative;
  pid_output = constrain((int)pid_output, 0, 255); // Limita o range do sinal de controle de 0 a 255

  // Salva o erro atual para a proxima iteracao
  last_error = error;
}

// Atualiza o valor da temperatura a uma frequencia de 2Hz
ISR(TIMER3_OVF_vect) {
  noInterrupts();
  TCNT3 = 0x85EE; // Reseta a pre-carga do timer1 = (34286)_10
  interrupts();

  analog2temp();

  // Verificando mudanca no SP
  if (lastTargetTemp != targetTemp) {
    proportional = 0;
    error = 0;
    last_error = 0;
    derivative = 0;
    derivative_error = 0;
    integral = 0;
    integral_error = 0;
    lastTargetTemp = targetTemp;
  }

  compute_PID();
}
