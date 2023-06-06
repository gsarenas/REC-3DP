#ifndef REC3DP_CONFIG_H
#define REC3DP_CONFIG_G

//===========================================================================================================
//================================================= Macros ==================================================
//===========================================================================================================

#define DEBUG_ENABLED true // Habilita debug via Serial.print (altere somente essa linha)
#if DEBUG_ENABLED
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_BEGIN(x) Serial.begin(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_BEGIN(x)
#endif

#define SERIAL_ENABLED false // Habilita comandos via porta serial (altere somente essa linha)
#if SERIAL_ENABLED
  #define SERIAL_PRINT(x) Serial.print(x)
  #define SERIAL_PRINTLN(x) Serial.println(x)
  #define SERIAL_BEGIN(x) Serial.begin(x)
  #define SERIAL_AVAILABLE(x) Serial.available(x)
  #define SERIAL_READSTRINGUNTIL(x) Serial.readStringUntil(x)
#else
  #define SERIAL_PRINT(x)
  #define SERIAL_PRINTLN(x)
  #define SERIAL_BEGIN(x)
  #define SERIAL_AVAILABLE(x)
  #define SERIAL_READ(x)
#endif

#define COOLING_ENABLED true // Habilita fan para cooling
#define PID_TUNING_ENABLED false // Habilita PID tuning

//===========================================================================================================
//=========================================== Configuracao Arduino ==========================================
//===========================================================================================================

/**
 * Configuracoes para pacote de Arduino "MightyCore AVR"
 *
 * Placa: ATmega1284
 * Clock: External 16 MHz
 * BOD: BOD 2.7V (ind)
 * EEPROM: EEPROM retained (ind)
 * Compiler LTO: LTO disabled (ind)
 * Variant: 1284P - low power
 * Pinout: Standard pinout
 * Bootloader: Yes (UART0)
 */

//===========================================================================================================
//=========================================== Pinout ATmega1284P ============================================
//===========================================================================================================

/**
 *
 * O pacote do MCU é TQFP 
 * Abaixo é apresentada uma relacao entre os pinos fisicos do ATmega1284P vs Arduino: n_fisico (n_arduino)
 * Na programacao, devemos nos referenciar aos valores entre parenteses (n_arduino)
 * 
 *                                              -------------------
 *               (PCINT13/ICP3/MOSI) PB5  1  (5)|                 |(4) 44 PB4 (SS/OC0B/PCINT12) 
 *               (PCINT14/OC3A/MISO) PB6  2  (6)|                 |(3) 43 PB3 (AIN1/OC0A/PCINT11)
 *                (PCINT15/OC3B/SCK) PB7  3  (7)|                 |(2) 42 PB2 (AIN0/INT2/PCINT10)
 *                                /RESET  4     |                 |(1) 41 PB1 (T1/CLKO/PCINT9)
 *                                   VCC  5     |                 |(0) 40 PB0 (XCK0/T0/PCINT8)
 *                                   GND  6     |                 |39 GND
 *                                 XTAL2  7     |                 |38 VCC
 *                                 XTAL1  8     |                 |(24/A0) 37 PA0 (ADC0/PCINT0)
 *                 (PCINT24/RXD0/T3) PD0  9  (8)|                 |(25/A1) 36 PA1 (ADC1/PCINT1)
 *                    (PCINT25/TXD0) PD1 10  (9)|                 |(26/A2) 35 PA2 (ADC2/PCINT2)
 *               (PCINT26/RXD1/INT0) PD2 11 (10)|   ATmega1284P   |(27/A3) 34 PA3 (ADC3/PCINT3)
 *               (PCINT27/TXD1/INT1) PD3 12 (11)|                 |(28/A4) 33 PA4 (ADC4/PCINT4)
 *               (PCINT28/XCK1/OC1B) PD4 13 (12)|                 |(29/A5) 32 PA5 (ADC5/PCINT5)
 *                    (PCINT29/OC1A) PD5 14 (13)|                 |(30/A6) 31 PA6 (ADC6/PCINT6)
 *                (PCINT30/OC2B/ICP) PD6 15 (14)|                 |(31/A7) 30 PA7 (ADC7/PCINT7)
 *                    (PCINT31/OC2A) PD7 16 (15)|                 |29 AREF
 *                                   VCC 17     |                 |28 GND
 *                                   GND 18     |                 |27 AVCC
 *                     (PCINT16/SCL) PC0 19 (16)|                 |(23) 26 PC7 (TOSC2/PCINT23)
 *                     (PCINT17/SDA) PC1 20 (17)|                 |(22) 25 PC6 (TOSC1/PCINT22) 
 *                     (PCINT18/TCK) PC2 21 (18)|                 |(21) 24 PC5 (TDI/PCINT21) 
 *                     (PCINT19/TMS) PC3 22 (19)|                 |(20) 23 PC4 (TDO/PCINT20) 
 *                                              -------------------
 */

//===========================================================================================================
//========================================== Configuracoes REC 3DP ==========================================
//===========================================================================================================

// Bibliotecas
#include "Arduino.h"

// Modulos
#include "Temperatura.h"
#include "Motor.h"
#include "Encoder.h"
#include "Display.h"
#include "UART.h"

//===========================================================================================================
//================================================= Encoder =================================================
//===========================================================================================================

/**
 * Encoder rotativo com switch - KY-040 (Handson Technology)
 * Tensao de operacao: 5V
 * 20 pulsos / rotacao
 * Saida: Codigo Gray 2-bit
 * Datasheet: http://handsontec.com/dataspecs/module/Rotary%20Encoder.pdf
 */ 

#define ENC_CLK             10
#define ENC_DT              11
#define ENC_SW              5

//===========================================================================================================
//=============================================== Temperatura ===============================================
//===========================================================================================================

/**
 * MOSFETs de potencia N-Channel - RF1S30N06LESM (Fairchild Semiconductor)
 * Pacote TO-263AB
 * 30A, 60V
 * r_DS(ON) = 0,047 Ohm
 * 2kV protecao ESD 
 * Datasheet: https://www.sparkfun.com/datasheets/Components/General/RFP30N06LE.pdf
 */ 

#define FANS_PIN            4
#define HEATED_BED_PIN      12
#define HOTEND_PIN          13

/**
 * Termistor NTC-100k
 * Padrao: 25C = 100k Ohms
 * Mesmo sensor utilizado em impressoras RepRap e Creality Ender Series
 * Tabela de Temperatura vs Leitura Analogica (ADC): https://github.com/MarlinFirmware/Marlin/blob/bugfix-2.1.x/Marlin/src/module/thermistor/thermistor_1.h
 */

#define THERMISTOR_PIN      A7

//===========================================================================================================
//================================================== Motor ==================================================
//===========================================================================================================

/**
 * Motor de passo NEMA 17 - VZS1740-048-1004 (Vurtz Motors)
 * 4,8kgf.cm/1,0A 
 * Datasheet: https://curtocircuito.com.br/datasheet/VZS1740-048-1004.pdf, https://vurtzmotors.com/
 * 
 * Controlado por um driver A4988 (Allegro) soldado na placa controladora
 * Tensao de operacao: 3,3 a 5V
 * Configuracao microstepping: 1/16
 * Datasheet: https://www.allegromicro.com/-/media/files/datasheets/a4988-datasheet.pdf
 */ 

#define STEP_PIN            15
#define DIR_PIN             21
#define EN_PIN              14

//===========================================================================================================
//================================================= Display =================================================
//===========================================================================================================

/**
 * Display LCD 16x2 com interface serial I2C (Handson Technology)
 * Endereco I2C: 0x27
 * Tensao de Operacao: 5V
 * Como estamos utlizando a interface TWI (I2C) com a biblioteca 'LiquidCrystal_I2C', nao precisamos definir os 
 * pinos conectados aqui. A comunicacao e feita diretamente atraves dos pinos SDA e SCL do microcontrolador
 * SDA_PIN                  17 (pino fisico 20)
 * SCL_PIN                  16 (pino fisico 19)  
 * Datasheet: http://www.handsontec.com/dataspecs/module/I2C_1602_LCD.pdf
 */

#endif
