import tkinter as tk
import serial
import threading
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

# Configuracao da porta serial
port = 'COM5'
baudrate = 115200

# Inicia a comunicacao serial
ser = serial.Serial(port, baudrate)

# Inicializa as listas para armazenar os dados de plotagem
timestamps = []
temperatures = []
pid_outputs = []
motor_speeds = []

# Funcao para ler dados da porta serial
def read_serial():
    while True:
        if ser.in_waiting > 0:
            received_data = ser.readline().decode().strip()
            timestamp = datetime.now()
            message_data = received_data.split(':')
            if len(message_data) == 3:
                temperature = float(message_data[0])
                pid_output = float(message_data[1])/10.625 # Converte analogico para tensao (255/24)
                motor_speed = float(message_data[2])

                # Adiciona os dados as respectivas listas
                timestamps.append(timestamp)
                temperatures.append(temperature)
                pid_outputs.append(pid_output)
                motor_speeds.append(abs(motor_speed) if motor_speed < 0 else motor_speed)

                # Atualiza o grafico
                update_plot()

            # Mostra a mensagem recebida na caixa de texto
            message_box.insert(tk.END, 'Recv: ' + received_data + '\n')
            message_box.see(tk.END)  # Auto Scroll

# Funcao para atualizar o grafico
def update_plot():
    # Limpa o grafico atual
    plot_ax.clear()

    # Calcula a diferenca de tempo entre timestamps consecutivos
    time_diffs = [(timestamps[i+1] - timestamps[i]).total_seconds() for i in range(len(timestamps) - 1)]
    x_values = [0] + list(np.cumsum(time_diffs))  # Acumula a diferenca de tempos

    # Plota os dados
    temperature_line, = plot_ax.plot(x_values, temperatures, label='Temperatura')
    pid_output_line, = plot_ax.plot(x_values, pid_outputs, label='Tensão')
    motor_speed_line, = plot_ax.plot(x_values, motor_speeds, label='Velocidade do motor')

    # Customizando o grafico
    plot_ax.set_xlabel('Tempo (s)', fontsize=12)
    plot_ax.set_ylabel('Temperatura (°C), Tensão (V), Velocidade (%)', fontsize=12)
    plot_ax.set_title('Temperatura, Tensão e Velocidade vs Tempo', fontsize=14)
    plot_ax.legend()

    # Adiciona anotacoes 'data tips' para cada linha do grafico
    def add_annotation(line, x, y):
        plot_ax.annotate(f'({x:.2f}, {y:.2f})', xy=(x, y), xytext=(-50, -20), textcoords='offset points',
                         arrowprops=dict(arrowstyle='->'))

    # Atualiza as anotacoes para cada linha
    if temperatures:
        add_annotation(temperature_line, x_values[-1], temperatures[-1])
    if pid_outputs:
        add_annotation(pid_output_line, x_values[-1], pid_outputs[-1])
    if motor_speeds:
        add_annotation(motor_speed_line, x_values[-1], motor_speeds[-1])

    # Atualiza o grafico na GUI (IHM)
    canvas.draw()

# Funcao para enviar mensagem pela porta serial
def send_serial(event=None):  # Atualizado para aceitar um parametro de evento
    message = entry.get()
    ser.write(message.upper().encode())
    entry.delete(0, tk.END)

    # Mostra a mensagem recebida na caixa de texto
    message_box.insert(tk.END, 'Send: ' + message + '\n')
    message_box.see(tk.END)  # Auto Scroll

# Funcao para vincular o evento de pressionamento da tecla 'Enter' a 'send_serial'
def bind_enter(event):
    send_serial()

# Cria a janela da GUI (IHM)
window = tk.Tk()
window.title("REC-3DP GUI")

# Configura a geometria da janela
window.geometry("915x915+300+20")

# Configura as dimensoes minimas da janela
min_height = 915
min_width = 915
window.minsize(width=min_width, height=min_height)

# Configura as cores do plano de fundo e primeiro plano da janela
window.configure(bg='#f3f3f3')
window.tk_setPalette(background='#f3f3f3', foreground='black')

# Cria a caixa de texto para mostrar as mensagens recebidas e enviadas (terminal)
message_box = tk.Text(window, height=40, width=50, font=('Calibri', 14))
message_box.grid(row=0, column=0, sticky='nsew', padx=10, ipadx=10, pady=1, ipady=10)

# Cria o quadro para enviar mensagens
entry_frame = tk.Frame(window)
entry_frame.grid(row=1, column=0, sticky='ew', padx=10, ipadx=10, ipady=10)

# Cria a caixa de texto de entrada para as mensagens a ser enviadas
entry = tk.Entry(entry_frame, font=('Calibri', 14), bg='white')
entry.pack(side=tk.LEFT, fill=tk.X, expand=True)

# Cria o botao 'Enviar'
send_button = tk.Button(entry_frame, text="Enviar", command=send_serial, bg='#dddddd', font=('Calibri', 12), height=1)
send_button.pack(side=tk.RIGHT)

# Cria a tela e figura de plotagem
fig, plot_ax = plt.subplots(figsize=(8, 4))
canvas = FigureCanvasTkAgg(fig, master=window)
canvas.get_tk_widget().grid(row=0, column=1, rowspan=2, sticky='nsew')

# Configura os pesos da grade
window.grid_rowconfigure(0, weight=1)
window.grid_columnconfigure(0, weight=1)
window.grid_columnconfigure(1, weight=1)

# Vincula o evento de pressionamento da tecla 'Enter' a funcao 'send_serial'
entry.bind("<Return>", bind_enter)

# Comeca a leitura da porta serial em um thread separado
threading.Thread(target=read_serial, daemon=True).start()

# Configura a cor the fundo da figura e plotagem (graficos)
fig.patch.set_facecolor('#f3f3f3')
plot_ax.set_facecolor('white')

# Comeca o evento de loop da GUI (IHM)
window.mainloop()