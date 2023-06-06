/*
   REC 3DP: Dispositivo para Reciclagem de Garrafas PET em Filamento para impressao 3D

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

void setup() {
  #if DEBUG_ENABLED
  DEBUG_BEGIN(115200);
  DEBUG_PRINTLN("Debug Iniciado");
  #endif
  
  
  // Inicializa modulo de temperatura
  Temperature::Init_Timer3();
  Temperature::Init();

  // Inicializa o modulo do motor de passo
  Motor::Init_Timer2();
  Motor::Init();

  // Inicializa o modulo do encoder
  Encoder::Init();

  // Inicializa o modulo do display LCD
  Display::Init();

  // Habilita as interrupções globais
  sei();
}

void loop() {
  // Controla o cartucho de aquecimento do hotend
  Temperature::manage_heater();

  // Controla o motor de passo
  Motor::run();

  // Atualiza o display LCD
  Display::update();

  // Printa a informacao para debug via porta serial a cada segundo decorrido, caso debugging esteja habilitado
  UART::debug_handler();

  // Verifica se foi recebido algum dado via porta serial, caso a comunicacao serial esteja habilitada
  UART::serial_handler();
}
