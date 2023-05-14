/*
   3DPREC: Dispositivo para Reciclagem de Garrafas PET em Filamento para impressao 3D

   Apresentado a Faculdade Engenheiro Salvador Arena (FESA)
   Apresentacao de artigo no SIMAC

   Desenvolvido por:
   Adolfo Dias <061200020@faculdade.cefsa.edu.br>
   Camila Tvardoski <062200002@faculdade.cefsa.edu.br>
   Gustavo Arenas <061200006@faculdade.cefsa.edu.br>
   Pedro Scapin <061200008@faculdade.cefsa.edu.br>
   Rebeca Vitoretti <062200020@faculdade.cefsa.edu.br>

*/

#include "REC_3DP_Config.h"

// Auxiliar: tempo rodando o programa atual na placa
unsigned long currMillis_Debug = 0;
// Auxiliar: referencia de tempo para calcular deltaT em relacao a 'currMillis_'
unsigned long prevMillis_Debug = 0;
// Intervalo de tempo para printar mensagens de debug via portal serial
const int debugTimeInterval = 1000;

void setup() {
  DEBUG_BEGIN(115200);
  DEBUG_PRINTLN("Go!");

  // Inicializa modulo de temperatura
  Init_Timer3();
  Init_MOSFETs();

  // Inicializa o modulo do motor de passo
  Init_Timer2();
  Init_Motor();

  // Inicializa o modulo do encoder
  Init_Encoder();

  // Inicializa o modulo do display LCD
  Init_Display();

  // Habilita as interrupções globais
  sei();
}

void loop() {
  // Controla o cartucho de aquecimento do hotend
  manage_heater();

  // Controla o motor de passo
  run_motor();

  // Atualiza o display LCD
  update_display();

  // Printa a informacao para debug via porta serial a cada segundo decorrido, caso debugging esteja habilitado
  if (one_sec_has_passed() && DEBUG_ENABLED) print_debug();
}


//--------------------------------------------------------------------------------------------
// Auxiliar

// Verifica se passou o intervalo de tempo para debug (padrao 1000 ms)
bool one_sec_has_passed() {
  currMillis_Debug = millis(); // Atualiza o tempo rodando o programa atual na placa
  if (currMillis_Debug - prevMillis_Debug > debugTimeInterval) {
    return true;
  } else return false;

}


// Serial print debug
void print_debug() {
  DEBUG_PRINT("Real T: ");
  DEBUG_PRINT(tempValue);
  DEBUG_PRINT(" | ");
  DEBUG_PRINT("PID out: ");
  DEBUG_PRINT(pid_output);
  DEBUG_PRINT(" | ");
  DEBUG_PRINT("Error: ");
  DEBUG_PRINT(error);
  DEBUG_PRINT(" | ");
  DEBUG_PRINT("Proportional: ");
  DEBUG_PRINT(proportional);
  DEBUG_PRINT(" | ");
  DEBUG_PRINT("Integral: ");
  DEBUG_PRINT(integral);
  DEBUG_PRINT(" | ");
  DEBUG_PRINT("Derivative: ");
  DEBUG_PRINTLN(derivative);

  // Atualiza o ultimo instante em que 'print_debug()' foi chamado
  prevMillis_Debug = currMillis_Debug;
}
