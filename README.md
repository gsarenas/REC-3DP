# REC-3DP  
Recicladora de Garrafa PET para Impressão 3D  
  
> **Apresentado a Faculdade Engenheiro Salvador Arena (FESA)**  
> **Apresentacao de artigo no SIMAC**
>
> **Desenvolvido por:**
>
> Adolfo Dias <061200020@faculdade.cefsa.edu.br>  
> Camila Tvardoski <062200002@faculdade.cefsa.edu.br>  
> Gustavo Arenas <061200006@faculdade.cefsa.edu.br>  
> Pedro Scapin <061200008@faculdade.cefsa.edu.br>  
> Rebeca Vitoretti <062200020@faculdade.cefsa.edu.br>  

## Funcionamento do projeto

### Protótipo funcionando
[Recicladora PET - Teste 01](https://youtu.be/Kp44HoyRCVU)

![](IMG/PROTOTIPO-FUNCIONANDO.png)

### Placa protótipo
Reaproveitando uma placa Creality3D V1.1.4 (Ender 3 Series)

![](IMG/P_20230319_201323.jpg)

## Interface visual - UART

Implementação de uma interface visual para controle do dispositivo via porta serial. Além do terminal para comunicação bidirecional entre o dispositivo e um guest, foi implementada também uma plotagem que é atualizada em tempo real das principais variáveis de processo (temperatura do Hotend, tensão fornecida e velocidade do motor)

Funções disponívels (G-CODE):
* M104 S___: Configura o novo setpoint para temperatura do Hotend (com feedback, controlador PID)
* F___: Configura o novo setpoint para velocidade do motor (sem feedback)

![](IMG/REC-3DP-GUI.png)
