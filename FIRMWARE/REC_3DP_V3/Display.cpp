#include "REC_3DP_Config.h"

namespace Display {

    // Cria um objeto do display LCD
    LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

    // Define os itens do menu
    const char* menu[] = {"menu1", "menu2"};
    const int num_menu_items = sizeof(menu) / sizeof(menu[0]);

    // Define o indice do menu atual
    int current_menu_item = 0;

    // Auxiliar: tempo rodando o programa atual na placa para atualizar o Display
    long currMillis_Display = 0;
    // Auxiliar: referencia de tempo para calcular deltaT em relacao a
    // 'currMillis_Display'
    long prevMillis_Display = 0;
    // Intervalo de tempo para printar mensagens de debug via portal serial
    const int displayTimeInterval = 2000;

    // Define um ponteiro de funcao para o submenu
    typedef void (*submenu_func_t)();

    // Cria um caractere customizado para acao de selecionar
    byte selecionaChar[] = {B10000, B10000, B10000, B10100,
                            B10110, B11111, B00110, B00100};

    // Setup Display LCD 16x2 I2C
    void Init() {
        // Inicializa o display LCD
        lcd.init();
        lcd.backlight();

        print_menu();  // Printa o menu inicial

        lcd.createChar(0, selecionaChar);
    }

    void submenu1() {
        lcd.clear();
        Temperature::targetTemp = 0;  // Reinicia o SP
        analogWrite(HOTEND_PIN, 0);   // Desliga o aquecimento com o SP zerado
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print("Temp. Control");
        lcd.setCursor(1, 1);
        lcd.print("SP:");
        lcd.print(Temperature::targetTemp);
        lcd.setCursor(7, 1);
        lcd.write(0xDF);
        lcd.print("|PV:");
        lcd.print(int(Temperature::tempValue));
        lcd.setCursor(15, 1);
        lcd.write(0xDF);

        while (true) {
            delay(20);  // IMPORTANTE -> DEBOUNCE PARA O ENCODER
            int enc_value = Encoder::enc_counter;
            if (enc_value != Encoder::enc_last) {
                // lcd.clear();
                lcd.setCursor(0, 1);
                lcd.write(byte(0));
                lcd.setCursor(1, 0);
                lcd.print("Temp. Control");
                Temperature::targetTemp += enc_value;
                Temperature::targetTemp = constrain(Temperature::targetTemp, 0, Temperature::maxTempAllowed);
                lcd.setCursor(1, 1);
                lcd.print("SP:");
                lcd.print(Temperature::targetTemp);

                // Limpa caracteres "residuais" do display
                if (0 < Temperature::targetTemp && Temperature::targetTemp < 100) {
                    lcd.setCursor(6, 1);
                    lcd.print(" ");
                }
                if (0 < Temperature::targetTemp && Temperature::targetTemp < 10) {
                    lcd.setCursor(5, 1);
                    lcd.print("  ");
                }

                lcd.setCursor(7, 1);
                lcd.write(0xDF);
                lcd.print("|PV:");
                lcd.print(int(Temperature::tempValue));
                lcd.setCursor(15, 1);
                lcd.write(0xDF);

                Encoder::enc_counter = 0;
                Encoder::enc_last = enc_value;
            }
            if (!digitalRead(ENC_SW)) {
                delay(200);
                break;
            }
        }
        Temperature::targetTemp = constrain(Temperature::targetTemp, 0, Temperature::maxTempAllowed);
        print_menu();
    }

    void submenu2() {
        lcd.clear();
        // analogWrite(HOTEND_PIN, 0); // Garante que a temperatura nao vai continuar
        // subindo enquanto ajusta SP
        Motor::motSpeed_ = 0;
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print("Speed Control");
        lcd.setCursor(1, 1);
        lcd.print(Motor::motSpeed_);
        lcd.setCursor(5, 1);
        lcd.print("%");
        while (true) {
            delay(20);                     // IMPORTANTE -> DEBOUNCE PARA O ENCODER
            Temperature::manage_heater();  // Garante que a temperatura se mantenha
                                           // estavel dentro do loop do submenu
            int enc_value = Encoder::enc_counter;
            if (enc_value != Encoder::enc_last) {
                // lcd.clear();
                lcd.setCursor(0, 1);
                lcd.write(byte(0));
                lcd.setCursor(1, 0);
                lcd.print("Speed Control");
                Motor::motSpeed_ += enc_value;
                Motor::motSpeed_ = constrain(Motor::motSpeed_, -100, 100);
                lcd.setCursor(1, 1);
                lcd.print(Motor::motSpeed_);

                // Limpa caracteres "residuais" do display
                if (0 < Motor::motSpeed_ && Motor::motSpeed_ < 100) {
                    lcd.setCursor(3, 1);
                    lcd.print(" ");
                }
                if (0 < Motor::motSpeed_ && Motor::motSpeed_ < 10) {
                    lcd.setCursor(2, 1);
                    lcd.print("  ");
                }
                if (Motor::motSpeed_ == 0) {
                    lcd.setCursor(2, 1);
                    lcd.print("  ");
                }
                if (Motor::motSpeed_ < 0 && Motor::motSpeed_ > -100) {
                    lcd.setCursor(4, 1);
                    lcd.print(" ");
                }
                if (Motor::motSpeed_ < 0 && Motor::motSpeed_ > -10) {
                    lcd.setCursor(3, 1);
                    lcd.print("  ");
                }

                lcd.setCursor(5, 1);
                lcd.print("%");
                Encoder::enc_counter = 0;
                Encoder::enc_last = enc_value;
            }
            if (!digitalRead(ENC_SW)) {
                delay(200);
                break;
            }
        }
        Motor::motSpeed_ = constrain(Motor::motSpeed_, -100, 100);
        print_menu();
    }

    // Define a estrutura do menu
    struct MenuItem {
        const char* label;
        submenu_func_t submenu_func;
    };

    // Define os itens do menu com submenu
    MenuItem menu_items[] = {
        {"menu1", submenu1},
        {"menu2", submenu2},
    };

    // Funcao para printar o menu atual
    void print_menu() {
        lcd.clear();
        lcd.setCursor(1, 0);
        if (current_menu_item == 0) {
            /*lcd.print("SP:");
              lcd.print(targetTemp);
              lcd.setCursor(7, 0);
              lcd.write(0xDF);
              lcd.print("|PV:");
              lcd.print(int(tempValue));
              lcd.setCursor(15, 0);
              lcd.write(0xDF);*/
            lcd.print("Temp. Control");
            lcd.setCursor(1, 1);
            lcd.print("SP:");
            lcd.print(Temperature::targetTemp);
            lcd.setCursor(7, 1);
            lcd.write(0xDF);
            lcd.print("|PV:");
            lcd.print(int(Temperature::tempValue));
            lcd.setCursor(15, 1);
            lcd.write(0xDF);
        }
        if (current_menu_item == 1) {
            lcd.print("Speed Control");
            lcd.setCursor(1, 1);
            lcd.print(Motor::motSpeed_);
            lcd.setCursor(5, 1);
            lcd.print("%");
            /*lcd.print(motSpeed);
              lcd.print("%");*/
        }
        // lcd.print(menu_items[current_menu_item].label);
    }

    void update() {
        // Verifica se o botao e pressionado
        if (!digitalRead(ENC_SW)) {
            delay(200);
            // Chama a funcao de submenu para o item do menu atual
            menu_items[current_menu_item].submenu_func();
        }

        // Verifica se houve alteracao no encoder rotativo
        int enc_value = Encoder::enc_counter;
        if (enc_value != Encoder::enc_last) {
            // Atualiza o item do menu atual e printa o novo menu
            current_menu_item = (current_menu_item + enc_value + num_menu_items) % num_menu_items;
            print_menu();

            // Reseta o contador do encoder
            Encoder::enc_counter = 0;
            Encoder::enc_last = enc_value;
        }
        // Atualiza o tempo rodado
        currMillis_Display = millis();

        // Atualiza o display a cada 'displayTimeInterval' (2000ms)
        if (currMillis_Display - prevMillis_Display > displayTimeInterval && current_menu_item == 0) {
            print_menu();
            prevMillis_Display = currMillis_Display;
        }
    }
}  // namespace Display
