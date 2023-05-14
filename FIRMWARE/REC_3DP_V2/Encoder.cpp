#include "REC_3DP_Config.h"

// Declara as variaveis do encoder rotativo
int enc_last = 0;
int enc_counter = 0;
int lastEncoded = 1;

// Inicializa Encoder KY-040
void Init_Encoder() {
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_CLK), encoder_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_DT), encoder_isr, CHANGE);
}

// ISR para o encoder rotativo
void encoder_isr() {
  noInterrupts();
  int MSB = digitalRead(ENC_CLK);
  int LSB = digitalRead(ENC_DT);
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 4) | encoded;
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    enc_counter--;
  } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    enc_counter++;
  }
  lastEncoded = encoded;
  sei();
}
