import sys
import serial as ser
import serial.tools.list_ports
import time
import PySimpleGUI as sg
import os
import numpy as np

# this example implements a chat where the PC sends a message/character and receives a response
# arguments:  0 - read 1 byte each time from the buffer
#             1 - read from the buffer until the terminator sequence '\n' is received
COM = 'COM3'
baudrate = 9600

Rx_out = (1.70 / 3.3) * 0xFFFF  # spose to be ---> 34157 Rx out = 1.72(V)
Ry_out = (1.61 / 3.3) * 0xFFFF  # spose to be ---> 31973 Ry out = 1.61(V)


def motor_rotate_for_cal(x, s):
    bytetxMsg = bytes('1' + x + '\n', 'ascii')
    s.write(bytetxMsg)
    time.sleep(0.25)
    s.reset_output_buffer()

def serial_ports():
    ports = serial.tools.list_ports.comports()
    list = []
    for port, desc, hwid in sorted(ports):
        list.append(port + ' ' + desc.split('(')[0])
        #print("{}: {}".format(port, desc.split('(')[0]))
    return list


def change_COM(x):
    global COM
    COM = x

def change_baundrate(x):
    global baudrate
    baudrate = x

def currect_mod(x, s):
    bytetxMsg = bytes('4' + x + '\n', 'ascii')
    s.write(bytetxMsg)
    time.sleep(0.25)
    s.reset_output_buffer()

def baudrate_change_send(x):
    s = create_uart_connection()
    bytetxMsg = bytes('3' + str(x) + '\n', 'ascii')
    s.write(bytetxMsg)
    time.sleep(0.25)

def create_uart_connection():
    global COM
    global baudrate
    s = ser.Serial(COM, baudrate=baudrate, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)  # timeout of 1 sec so that the read and write operations are blocking,
    # when the timeout expires the program will continue
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    return s

def initial_Uart_avoid_cut(s):
    bytetxMsg = bytes('00' + '\n', 'ascii')
    s.write(bytetxMsg)
    time.sleep(0.25)
    s.reset_output_buffer()

def send_massge_mod1(s, x):
    enableTX = True
    # TX
    while (1):  # while the output buffer isn't empty
        txMsg = x
        bytetxMsg = bytes('1' + txMsg + '\n', 'ascii')
        s.write(bytetxMsg)
        time.sleep(0.25)  # delay for accurate read/write operations on both ends
        if s.out_waiting == 0:
            return
            #enableTX = False

def read_massge_mod1(s):
    # RX
    data = ''
    while (s.in_waiting > 0):  # while the input buffer isn't empty
        lineByte = s.read_until()    # read  from the buffer until the terminator is received,
        #time.sleep(0.25)  # delay for accurate read/write operations on both ends
        data += lineByte.decode("ascii")
        if s.in_waiting == 0:
            return data
    return ''

def recive_file_in_MCU(s):
    data = chr(2)
    bytetxMsg = bytes('53' + '\n', 'ascii')
    s.write(bytetxMsg)
    s.reset_output_buffer()
    while (True):  # while the input buffer isn't empty
        lineByte = s.read_until()  # read  from the buffer until the terminator is received,
        # time.sleep(0.25)  # delay for accurate read/write operations on both ends
        data += lineByte.decode("ascii")
        if (data[-1] == '\n'):
            if data[1] != chr(4) and data[1] != chr(3):
                print(data)
                data = chr(2)
                bytetxMsg = bytes('53' + '\n', 'ascii')
                s.write(bytetxMsg)
                s.reset_output_buffer()
            else:
                break

    if (data[1] == chr(3)):
        s.reset_input_buffer()
        s.reset_output_buffer()
        return []
    else:
        s.reset_input_buffer()
        s.reset_output_buffer()
        data = data[0] + data[2:]
        return data[:-1].split(chr(2))



def Run_file_from_MCU_RAM(x, s):
    bytetxMsg = bytes('54' + x + '\n', 'ascii')
    s.write(bytetxMsg)
    time.sleep(0.25)
    s.reset_output_buffer()



def file_send_mod2(x, s):
    path = x
    flag = 0
    file_name = path.split("/")[-1]
    file_read = open(path, "r")
    enableTX = True
    file_text = file_read.read()
    if len(file_text) >= 8192:
        return -1
    while (s.out_waiting > 0 or enableTX):
        bytetxMsg = bytes('51' + file_name + chr(2) + str(len(file_text) + 1)[::-1] + '\n', 'ascii') #+ file_text + chr(3), 'ascii')
        #print('2' + file_name + chr(2) + str(len(file_text) + 1) + '\n' + file_text + chr(3))
        s.write(bytetxMsg)
        time.sleep(0.25)  # delay for accurate read/write operations on both ends
        if s.out_waiting == 0:
            enableTX = False
            s.reset_output_buffer()

    wait_to_ACK(s)
    enableTX = True

    while (s.out_waiting > 0 or enableTX):
        bytetxMsg = bytes('52' + '\n' + file_text + chr(3), 'ascii')
        #print('2' + file_name + chr(2) + str(len(file_text) + 1) + '\n' + file_text + chr(3))
        s.write(bytetxMsg)
        time.sleep(0.25)  # delay for accurate read/write operations on both ends
        if s.out_waiting == 0:
            enableTX = False
            s.reset_output_buffer()

    return wait_to_ACK(s)

def file_recive_mod2(s):
    data = ''
    while (True):  # while the input buffer isn't empty
        lineByte = s.read_until(expected = b'\x03')  # read  from the buffer until the terminator is received,
        time.sleep(0.25)
        data += lineByte.decode("ascii")
        try:
            if data[-1] == chr(3):
                return data[:-1]
        except:
            pass


def file_name_recive_mod2(s):
    i = 0
    while(i < 20000):
        i += 1
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            lineByte = s.read_until(expected = b'\x02')  # read  from the buffer until the terminator is received,
            time.sleep(0.25)
            data = lineByte.decode("ascii")
            if s.in_waiting == 0:
                return data
    return ''


def send_ACK_to_MCU(s):
    enableTX = True
    while (s.out_waiting > 0 or enableTX):
        bytetxMsg = bytes('6' + '1' + '\n', 'ascii')
        s.write(bytetxMsg)
        #time.sleep(0.25)  # delay for accurate read/write operations on both ends
        if s.out_waiting == 0:
            enableTX = False
            return


def wait_to_ACK(s):
    while (1):
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            lineByte = s.read_until()  # read  from the buffer until the terminator is received,
            #time.sleep(0.25)
            # readline() can also be used if the terminator is '\n'
            if (ord(lineByte.decode("ascii")[-1]) == 6):
                s.reset_input_buffer()
                return lineByte.decode("ascii")[-1]

def wait_for_int(s):
    while (1):
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            lineByte = s.read_until()  # read  from the buffer until the terminator is received,
            time.sleep(0.25)
            # readline() can also be used if the terminator is '\n'
            if (lineByte.decode("ascii")[-1] == "\n"):
                s.reset_input_buffer()
                return lineByte.decode("ascii")[:-1]

def wait_for_xy(s):
    i = 0
    data = ''
    while (i < 20000):
        i += 1
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            while(True):
                lineByte = s.read_until()  # read  from the buffer until the terminator is received,
                #time.sleep(0.25)
                try:
                    data += lineByte.decode("ascii")
                except:
                    data += ''
                # readline() can also be used if the terminator is '\n'
                if (data[-1] == "\n"):
                    data = data[:-1]
                    try:
                        s.reset_input_buffer()
                        x, y = data.split("A")
                        if(len(data) != 11):
                            #print(data + "  Exeption SIZE")
                            return 0, 0
                        return cartez_to_polar(int(x), int(y))
                    except:
                        s.reset_input_buffer()
                        #print(data + "  Exeption FUCK")
                        return 0, 0
    return 0, 0

def wait_for_xy_and_sw(s):
    i = 0
    data = ''
    while (i < 20000):
        i += 1
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            while(True):
                lineByte = s.read_until()  # read  from the buffer until the terminator is received,
                #time.sleep(0.25)
                data += lineByte.decode("ascii")
                # readline() can also be used if the terminator is '\n'
                if (data[-1] == "\n"):
                    data = data[:-1]
                    try:
                        s.reset_input_buffer()
                        x, y, SW = data.split("A")
                        x, y = cartez_to_polar(int(x), int(y))
                        return x, y, int(SW)
                    except:
                        s.reset_input_buffer()
                        #print(data)
                        data = ''
                        #raise Exception("WRONG!")
                        break

    return 0, 0 ,0

def cartez_to_polar(x,y):
    global Rx_out
    global Ry_out
    x = x - Rx_out  # spose to be ---> 34157 Rx out = 1.72(V)
    y = y - Ry_out  # spose to be ---> 31973 Ry out = 1.61(V)
    rho = np.sqrt(x**2 + y**2)
    phi = np.arctan2(y, x)
    phi = np.degrees(phi) + 360 ##- 15 ##------------------------------------ Offset?!?!!?----------------------------
    return rho, phi


def send_angel(x, s):
    bytetxMsg = bytes('2' + x + '\n', 'ascii')
    s.write(bytetxMsg)
    #time.sleep(0.25)
    s.reset_output_buffer()


#if __name__ == '__main__':
    #initial_Uart_avoid_cut()
    #file_send_mod2("C:\\Users\\Bar-PC\\Desktop\\flie tests for Project\\file2.txt")
    #file_send_mod2("C:\\Users\\Bar-PC\\Desktop\\flie tests for Project\\file1.txt")
    #file_send_mod2("C:\\Users\\Bar-PC\\Desktop\\flie tests for Project\\file3.txt")
    #currect_mod('2')

    # sg.theme('Dark Grey 13')
    #
    # layout = [[sg.Text('Filename')],
    #           [sg.Input(), sg.FileBrowse()],
    #           [sg.OK(), sg.Cancel()]]
    #
    # window = sg.Window('Get filename example', layout)
    #
    # event, values = window.read()
    # window.close()


    #read_or_send_massge_mod1()
    # print(bytes('AaBb123yz' + '\n', 'ascii'))
    # for j in range(0,129):
    #     print('----------------')
    #     print(j)
    #     print(bytes('', 'utf-8'))
    #     print(len(bytes('\n', 'utf-8')))
    ##print('----------------')
    ##print(bytes('A', 'utf-8'))
    ##print(len(bytes('A', 'utf-8')))
    # for i in range(0,256):
    #     print('----------------')
    #     print(i)
    #     print(bytes(chr(i), 'utf-8'))
    #     print(len(bytes(chr(i), 'utf-8')))