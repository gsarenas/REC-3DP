#include "REC_3DP_Config.h"

namespace Motor {
  int motSpeed = 0;
  int motSpeed_ = 0;
  // int lastMotSpeed = 0;

  // Inicializacao do Timer2 para controle de velocidade do motor via pino OC2A
  void Init_Timer2() {
    noInterrupts();

    // Configurando registradores do Timer2

    TCCR2A = (1 << COM2A0) | (1 << WGM21); // Aciona OC2A no modo compare match nao invertido

    TCCR2B |= (1 << CS22) | (0 << CS21) | (1 << CS20); // Configura prescaler do Timer2 em 64
    // TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20)); // Limpa os bits de prescaler para interromper operacao do Timer2, caso necessario

    /* OCR2A configura a frequencia de saida no pino OC2A (REGISTRADOR EXTREMAMENTE IMPORTANTE PARA CONTROLE DE VELOCIDADE DO MOTOR)
       frequencia = (F_CPU) / (2 * prescaler * (OCR2A + 1))
       OCR2A = ((F_CPU) / (2 * prescaler * frequencia)) - 1
    */
    OCR2A = motSpeed;

    interrupts();
  }

  // Setup do driver do motor de passo A4988
  void Init() {

    // Saidas
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);

    // Condicao inicial
    digitalWrite(EN_PIN, HIGH);  // Driver inicialmente desabilitado
    digitalWrite(DIR_PIN, HIGH); // Direcao do motor
  }

  // Controls stepper motor
  void run() {
    // Velocidade positiva -> Giro sentido horario
    if (motSpeed_ > 0) {
      motSpeed = map(motSpeed_, 1, 100, 255, 3);
      OCR2A = motSpeed;
      digitalWrite(DIR_PIN, 1); // Direcao do motor CW
      digitalWrite(EN_PIN, 0);
    }
    // Velocidade negativa -> Giro no sentido anti-horario
    else if (motSpeed_ < 0) {
      motSpeed = map(abs(motSpeed_), 1, 100, 255, 3);
      OCR2A = motSpeed;
      digitalWrite(DIR_PIN, 0); // Direcao do motor CCW
      digitalWrite(EN_PIN, 0);
    }
    // Desabilita o driver do motor
    else
      digitalWrite(EN_PIN, 1);
  }
}
