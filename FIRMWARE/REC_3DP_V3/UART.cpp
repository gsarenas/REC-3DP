#include "REC_3DP_Config.h"

namespace UART {
    // Auxiliar: tempo rodando o programa atual na placa
    unsigned long currMillis_Debug = 0;
    // Auxiliar: referencia de tempo para calcular deltaT em relacao a 'currMillis_'
    unsigned long prevMillis_Debug = 0;
    // Intervalo de tempo para printar mensagens de debug via portal serial
    const int debugTimeInterval = 1000;

    // Verifica se passou o intervalo de tempo para debug (padrao 1000 ms)
    bool one_sec_has_passed() {
        currMillis_Debug = millis();  // Atualiza o tempo rodando o programa atual na placa
        if (currMillis_Debug - prevMillis_Debug > debugTimeInterval) {
            return true;
        } else
            return false;
    }

    // Serial print debug
    void debug_handler() {
    #if DEBUG_ENABLED
            if (one_sec_has_passed()) {
                DEBUG_PRINT("Real T: ");
                DEBUG_PRINT(Temperature::tempValue);
                DEBUG_PRINT(" | ");
                DEBUG_PRINT("PID out: ");
                DEBUG_PRINT(Temperature::pid_output);
                DEBUG_PRINT(" | ");
                DEBUG_PRINT("Error: ");
                DEBUG_PRINT(Temperature::error);
                DEBUG_PRINT(" | ");
                DEBUG_PRINT("Proportional: ");
                DEBUG_PRINT(Temperature::proportional);
                DEBUG_PRINT(" | ");
                DEBUG_PRINT("Integral: ");
                DEBUG_PRINT(Temperature::integral);
                DEBUG_PRINT(" | ");
                DEBUG_PRINT("Derivative: ");
                DEBUG_PRINTLN(Temperature::derivative);

                // Atualiza o ultimo instante em que 'print_debug()' foi chamado
                prevMillis_Debug = currMillis_Debug;
            }
    #endif
    }

    /** 
     * Comandos via porta Serial
     * F___: Feed Rate, velocidade do motor [-100; 100]
     * M104S___ ou M104 S___: Define a temperatura do hotend ]-inf; 240[ 
     * Done: Fecha a porta serial e restaura valores iniciais
     */ 
    void serial_handler() {
    #if SERIAL_ENABLED
            // Verifica se ha algum dado disponivel para leitura da porta serial
            if (SERIAL_AVAILABLE()) {
                // Le o byte de entrada
                String command = SERIAL_READSTRINGUNTIL('\n');

                String firstChar = command.substring(0, 1);

                // F___: Feed Rate (controla a velocidade do motor)
                if (firstChar == "F") {
                    String numberString_ = command.substring(1);
                    int newMotorSpeed = numberString_.toInt();
                    if (newMotorSpeed >= -100 && newMotorSpeed <= 100) {
                        Motor::motSpeed_ = newMotorSpeed;
                        Display::print_menu();
                        SERIAL_PRINT("Nova velocidade: ");
                        SERIAL_PRINTLN(Motor::motSpeed_);
                    } else {
                        SERIAL_PRINT("Velocidade nao permitida: ");
                        SERIAL_PRINTLN(newMotorSpeed);
                    }
                } else if (firstChar == "M") {
                    // M___: Codigos diversos
                    String nextChars = command.substring(1, 5);
                    String nextChars_ = command.substring(1, 6);
                    // M104 S___: Define a temperatura do hotend
                    if (nextChars == "104S") {
                        String numberString = command.substring(5);
                        int newTargetTemp = numberString.toInt();
                        if (newTargetTemp < Temperature::maxTempAllowed) {
                            Temperature::targetTemp = newTargetTemp;
                            Display::print_menu();
                            SERIAL_PRINT("Nova temperatura: ");
                            SERIAL_PRINTLN(Temperature::targetTemp);
                        } else {
                            SERIAL_PRINT("Temperatura nao permitida: ");
                            SERIAL_PRINTLN(newTargetTemp);
                        }
                    } else if (nextChars_ == "104 S") {
                        String numberString = command.substring(6);
                        int newTargetTemp = numberString.toInt();
                        if (newTargetTemp < Temperature::maxTempAllowed) {
                            Temperature::targetTemp = newTargetTemp;
                            Display::print_menu();
                            SERIAL_PRINT("Nova temperatura: ");
                            SERIAL_PRINTLN(Temperature::targetTemp);
                        } else {
                            SERIAL_PRINT("Temperatura nao permitida: ");
                            SERIAL_PRINTLN(newTargetTemp);
                        }
                    }
                } else if (command == "Done") {
                    Motor::motSpeed_ = 0;
                    Temperature::targetTemp = 0;
                    SERIAL_PRINTLN("Serial closed");
                } else {
                    SERIAL_PRINT("Comando desconhecido: ");
                    SERIAL_PRINTLN(command);
                }
            }
    #endif
    }
}  // namespace UART
