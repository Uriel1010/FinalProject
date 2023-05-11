import sys
import serial as ser
import time
import PySimpleGUI as sg
import os
from os.path import exists
import Final_project as tp
import Painter as paint
import glob
import gui_util as gui
import numpy as np


ACK_flag = True
steps_full_cycle = 0
nominal_angel = 0
s = tp.create_uart_connection()

def menu_GUI():
    global s
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Button('Manual control of motor', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size = (25,2))],
              [sg.Button('joystick paiter', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size = (25,2))],
              [sg.Button('calibrition motor', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size = (25,2))],
              [sg.Button('script mod', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True,size=(25, 2))],
              [sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    window = sg.Window('Main MENU', layout, element_justification='c',font=('Helvetica', ' 13'), default_button_element_size=(8, 2),
                       use_default_focus=False)

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            return
        elif event == 'Manual control of motor':
            s.reset_input_buffer()
            s.reset_output_buffer()
            tp.currect_mod('1', s)
            window.close()
            manual_cotrol_motor_GUI()
            menu_GUI()
        elif event == 'joystick paiter':
            s.reset_input_buffer()
            s.reset_output_buffer()
            tp.currect_mod('2', s)
            window.close()
            paiter()
            menu_GUI()
        elif event == 'calibrition motor':
            s.reset_input_buffer()
            s.reset_output_buffer()
            tp.currect_mod('3', s)
            window.close()
            calibrition_motor_GUI_step_1()
            menu_GUI()
        elif event == 'script mod':
            s.reset_input_buffer()
            s.reset_output_buffer()
            tp.currect_mod('4', s)
            window.close()
            script_mod_GUI()
            menu_GUI()

def paiter():
    global s
    s.reset_input_buffer()
    s.reset_output_buffer()
    sg.theme('GreenTan')
    canvas_size = 800
    current_Point_Y = canvas_size/2
    current_Point_X = canvas_size/2
    cursor_state = 0

    painter_layout = [[sg.Graph(canvas_size=(canvas_size, canvas_size), graph_bottom_left=(0,0), graph_top_right=(canvas_size, canvas_size), background_color='white', key='painter_graph')],
                     [sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    painter_window = sg.Window('PC Painter', painter_layout, finalize=True)
    graph = painter_window['painter_graph']

    cursor = graph.DrawPoint((canvas_size/2, canvas_size/2), 10, color='green')


    while True:
        event, values = painter_window.read(timeout=2)
        if event in (sg.WIN_CLOSED, 'EXIT'):
            painter_window.close()
            tp.currect_mod('0', s)
            return
        R, phi, SW = tp.wait_for_xy_and_sw(s)
        phi += 180
        if SW == 1:
            cursor_state += 1
            cursor_state = cursor_state % 3
        if R > 0.5*32676:
            current_Point_Y += float(np.sin(np.deg2rad(phi)))
            current_Point_X += float(np.cos(np.deg2rad(phi)))
            current_Point_Y %= canvas_size
            current_Point_X %= canvas_size


        # cursor_photo = tk.PhotoImage(file=cursor_img)
        graph.TKCanvas.itemconfig(cursor)
        graph.TKCanvas.imgref = cursor


        if (cursor_state == 0):
            graph.DrawPoint((current_Point_Y, current_Point_X), 1, color = 'black')
        elif (cursor_state == 1):
            graph.DrawCircle((current_Point_Y, current_Point_X), 5, fill_color='white', line_color='white')

        #graph.MoveFigure(cursor, current_Point_Y-8, current_Point_X+6)
        graph.RelocateFigure(cursor, current_Point_Y, current_Point_X)
        graph.bring_figure_to_front(cursor)





def script_mod_GUI():
    global s
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Button('Send File to MCU', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True,
                         size=(30, 2))],
              [sg.Button('Run specific File From MCU RAM', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True,
                         size=(30, 2))],
              [sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    window = sg.Window('Script_mod', layout, element_justification='c', font=('Helvetica', ' 13'),
                       default_button_element_size=(8, 2),
                       use_default_focus=False)

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            tp.currect_mod('0', s)
            window.close()
            return
        elif event == 'Send File to MCU':
            window.close()
            script_mod_send_file_GUI()
            script_mod_GUI()
        elif event == 'Run specific File From MCU RAM':
            s.reset_input_buffer()
            s.reset_output_buffer()
            window.close()
            script_mod_RUN_file_GUI()
            script_mod_GUI()

def script_mod_send_file_GUI():
    global ACK_flag
    global s
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Text('Path of File:'), sg.Push()],
              [sg.Input(key='Path'), sg.FileBrowse()],
              [sg.Button('OK', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True),
               sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True)]]

    window = sg.Window('send File', layout, element_justification='c')

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            return
        elif event == 'OK':
            if not exists(value['Path']):
                sg.popup("Error: file not exists")
            else:
                ACK = tp.file_send_mod2(value['Path'], s)
                if ACK == -1:
                    sg.popup("Error! file is bigger than 8KB!!!")
                elif ord(ACK) == 6 and ACK_flag == True:
                    sg.popup("file send complete")
                    tp.send_ACK_to_MCU(s)
                    window.close()
                    script_mod_send_file_while_run_GUI()
                    return

def script_mod_send_file_while_run_GUI():
    global s
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Text('output:', size=(40, 1))],
              [sg.Output(size=(110, 20), font=('Helvetica 10'), key='-chat-')],
              [sg.Button('EXIT', disabled=True, button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    window = sg.Window('Chat window', layout, font=('Helvetica', ' 13'), default_button_element_size=(8, 2),
                       use_default_focus=False)

    while True:  # The Event Loop
        event, value = window.read(timeout=2)
        if event in (sg.WIN_CLOSED, 'EXIT'):  # quit if exit button or X
            window.close()
            return
        responed = tp.read_massge_mod1(s)
        s.reset_input_buffer()
        if responed != '':
            print('MCU: {}'.format(responed), flush=True)
            if 'completed' in responed:
                window['EXIT'].update(disabled=False)
            # window.refresh()
            time.sleep(0.25)

def script_mod_RUN_file_GUI():
    global s
    layout = []
    result_of_files = []
    result_of_files_temp = tp.recive_file_in_MCU(s)

    sg.theme('GreenTan')  # give our window a spiffy set of colors
    if result_of_files_temp != []:
        for x in result_of_files_temp:
            if x != '':
                result_of_files.append(x)

    for i in range(len(result_of_files)):
        layout.append([sg.Button(result_of_files[i], button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(25, 2), key=i)])

    if len(result_of_files) == 0:
        sg.popup("There is no file in MCU RAM")
        return

    layout.append([sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))])

    window = sg.Window('Script_mod', layout, element_justification='c', font=('Helvetica', ' 13'),
                       default_button_element_size=(8, 2),
                       use_default_focus=False)

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            return
        elif event == 0:
            tp.Run_file_from_MCU_RAM('1', s)
            script_mod_send_file_while_run_GUI()
        elif event == 1:
            tp.Run_file_from_MCU_RAM('2', s)
            script_mod_send_file_while_run_GUI()
        elif event == 2:
            tp.Run_file_from_MCU_RAM('3', s)
            script_mod_send_file_while_run_GUI()

def manual_cotrol_motor_GUI():
    global s
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Button('Finish', button_color=(sg.YELLOWS[0], sg.GREENS[0]), size = (30,2))]]

    window = sg.Window('manual_cotrol_motor', layout, element_justification='c', font=('Helvetica', ' 13'),
                       default_button_element_size=(8, 2),
                       use_default_focus=False)
    phi = 0
    while True:  # The Event Loop
        event, value = window.read(timeout = 2)
        phi_past = phi
        R, phi = tp.wait_for_xy(s)
        if R > 0.5*32676 and (phi < phi_past - 1 or phi > phi_past + 1):
            tp.send_angel(str(int(phi))[::-1], s)
        if event in (sg.WIN_CLOSED, 'Finish'):
            tp.currect_mod('0', s)
            window.close()
            return

def calibrition_motor_GUI_step_1():
    global s
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [
        [sg.Button('Start rotate', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(30, 2))],
        [sg.Button('One step rotate clockwise', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(30, 2))],
        [sg.Button('One step rotate counter clockwise', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(30, 2))],
        [sg.Button('Finish', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(30, 2))],
        [sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    window = sg.Window('calibration step 1', layout, element_justification='c', font=('Helvetica', ' 13'),
                       default_button_element_size=(8, 2),
                       use_default_focus=False)

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            tp.currect_mod('0', s)
            return
        elif event == 'Start rotate':
            tp.motor_rotate_for_cal('0', s)
            sg.popup("press ok to stop motor")
            tp.motor_rotate_for_cal('1', s)
        elif event == 'One step rotate clockwise':
            tp.motor_rotate_for_cal('2', s)
        elif event == 'One step rotate counter clockwise':
            tp.motor_rotate_for_cal('3', s)
        elif event == 'Finish':
            sg.popup("The motor now is sets to 0 degrees\nin the following windows you'll be needed\nto complete full "
                     "circle for calibrating the motor.")
            tp.motor_rotate_for_cal('4', s)
            window.close()
            calibrition_motor_GUI_step_2()

def calibrition_motor_GUI_step_2():
    global s
    global steps_full_cycle
    global nominal_angel
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Button('Start rotate', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(30, 2))],
              [sg.Button('One step rotate clockwise', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(30, 2))],
              [sg.Button('One step rotate counter clockwise', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(30, 2))],
              [sg.Button('Finish', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size=(30, 2))],
              [sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    window = sg.Window('calibration step 2', layout, element_justification='c', font=('Helvetica', ' 13'),
                       default_button_element_size=(8, 2),
                       use_default_focus=False)
    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            tp.currect_mod('0', s)
            window.close()
            return
        elif event == 'Start rotate':
            tp.motor_rotate_for_cal('0', s)
            sg.popup("press ok to stop motor")
            tp.motor_rotate_for_cal('1', s)
        elif event == 'One step rotate clockwise':
            tp.motor_rotate_for_cal('2', s)
        elif event == 'One step rotate counter clockwise':
            tp.motor_rotate_for_cal('3', s)
        elif event == 'Finish':
            tp.motor_rotate_for_cal('5', s)
            steps_full_cycle = int(tp.wait_for_int(s))
            nominal_angel = 360/steps_full_cycle
            tp.currect_mod('0', s)
            sg.popup("amount of step in Full cycle is: {} \n The nominal angel is: {}".format(steps_full_cycle,nominal_angel))
            tp.currect_mod('0', s)
            window.close()
##-------------------------------------------------------------------------------------------------------------------
##-------------------------------------------------------------------------------------------------------------------
##-------------------------------------------------------------------------------------------------------------------
##-------------------------------------------------------------------------------------------------------------------
def chat_GUI():
    s = tp.create_uart_connection()
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Text('Your output will go here:', size=(40, 1))],
              [sg.Output(size=(110, 20), font=('Helvetica 10'), key ='-chat-')],
              [sg.Multiline(size=(70, 5), enter_submits=False, key='-QUERY-', do_not_clear=True),
               sg.Button('SEND', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True),
               sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    window = sg.Window('Chat window', layout, font=('Helvetica', ' 13'), default_button_element_size=(8, 2),
                       use_default_focus=False)

    while True:  # The Event Loop
        event, value = window.read(timeout = 2)
        if event in (sg.WIN_CLOSED, 'EXIT'):  # quit if exit button or X
            window.close()
            return
            #menu_GUI()
        if event == 'SEND':
            query = value['-QUERY-'].rstrip()
            window['-QUERY-'].update(value = '')
            # EXECUTE YOUR COMMAND HERE
            print('PC: {}'.format(query), flush=True)
            window.refresh()
            time.sleep(0.25)
            tp.send_massge_mod1(s, query)
            s.reset_output_buffer()
        responed = tp.read_massge_mod1(s)
        s.reset_input_buffer()
        if responed != '':
            print('MCU: {}'.format(responed), flush=True)
            #window.refresh()
            time.sleep(0.25)

def menu_file_mod2_GUI():
    global ACK_flag
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Text('Please select an option:')],
              [sg.Button('send file', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size = (15,2)),
               sg.Button('receive file', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True, size = (15,2))],
              [sg.Checkbox('ACK?',key='ack', default = ACK_flag, enable_events = True) , sg.Push(), sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    window = sg.Window('MENU - File RX/TX', layout, element_justification='c', font=('Helvetica', ' 13'), default_button_element_size=(8, 2),
                       use_default_focus=False)

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            return
        elif event == 'send file':
            window.close()
            file_GUI_send()
            menu_file_mod2_GUI()
        elif event == 'receive file':
            window.close()
            file_GUI_receive()
            menu_file_mod2_GUI()
        elif event == 'ack':
            if value['ack'] == True:
                ACK_flag = True
                tp.currect_mod('4')

            elif value['ack'] == False:
                ACK_flag = False
                tp.currect_mod('5')


def file_GUI_send():
    global ACK_flag
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Text('Path of File:'), sg.Push()],
              [sg.Input(key = 'Path'), sg.FileBrowse()],
              [sg.Button('OK', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True), sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True)]]

    window = sg.Window('send File', layout, element_justification='c')

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            return
        elif event == 'OK':
            if not exists(value['Path']):
                sg.popup("Error: file not exists")
            else:
                ACK = tp.file_send_mod2(value['Path'])
                tp.currect_mod('2')
                if ACK == -1:
                    sg.popup("Error! file is bigger than 8KB!!!")
                elif ord(ACK) == 6 and ACK_flag == True:
                    sg.popup("file send complete")
                window.close()
                return
def file_GUI_receive():
    data =''
    name_of_file =''
    name_of_file, data = file_GUI_receive_wait_for_file()
    #s = tp.create_uart_connection()
    sg.theme('GreenTan')  # give our window a spiffy set of colors

    layout = [[sg.Text('Path to save to:')],
              [sg.Input(key = 'Path'), sg.FolderBrowse()],
              [sg.Button('OK', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True), sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True)]]
    if data == '' or name_of_file == '':
        return
    window = sg.Window('reciving to...', layout)

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            return
        elif event == 'OK':
            if not exists(value['Path']):
                sg.popup("Error: folder not exists")
            else:
                if data != '':
                    flie_name = name_of_file[0:-1]
                    text_file = open(value['Path'] + '/' + flie_name, 'w')
                    text_file.write(data)
                    text_file.close()
                    tp.currect_mod('2')
                    window.close()
                    return
                else:
                    sg.popup("Error something went worng")

def file_GUI_receive_wait_for_file():
    file_ACK = False
    name_ACK = False
    sg.theme('GreenTan')  # give our window a spiffy set of colors
    layout = [[sg.Text('Waiting for file to be send....')],
              [sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True)]]

    window = sg.Window('Waiting for file', layout, element_justification='c', size = (400,80))
    string_of_file = ''
    name_of_file = ''
    s = tp.create_uart_connection()
    while True:  # The Event Loop
        event, value = window.read(timeout = 1)
        if name_of_file == '':
            name_of_file = tp.file_name_recive_mod2(s)
        if name_of_file != '' and name_ACK == False:
            tp.send_ACK_to_MCU(s, 'n')
            name_ACK = True
        if name_ACK == True:
            if string_of_file == '':
                string_of_file = tp.file_recive_mod2(s)
            if string_of_file != '' and file_ACK == False:
                tp.send_ACK_to_MCU(s, 'f')
                file_ACK = True
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            return '', ''
        if file_ACK == True and name_ACK == True:
            window.close()
            return name_of_file, string_of_file

def Terminal_config():
    sg.theme('GreenTan')  # give our window a spiffy set of colors
    Ports = tp.serial_ports()
    SDA = [1 if 'SDA' in x else 0 for x in Ports].index(1)
    list_of_baund = [2400, 9600, 19200, 38400]
    layout = [[sg.Combo(Ports, default_value=Ports[SDA], s=(30,22), enable_events=True, readonly=True, k='-COMBO1-')],
              [sg.Combo(list_of_baund, default_value=list_of_baund[list_of_baund.index(tp.baudrate)], s=(30,22), enable_events=True, readonly=True, k='-COMBO2-')],
              [sg.Button('Apply', button_color=(sg.YELLOWS[0], sg.BLUES[0]), bind_return_key=True),sg.Push(), sg.Button('EXIT', button_color=(sg.YELLOWS[0], sg.GREENS[0]))]]

    window = sg.Window('Terminal_config', layout,element_justification='c', font=('Helvetica', ' 13'), default_button_element_size=(8, 2),
                       use_default_focus=False)

    while True:  # The Event Loop
        event, value = window.read()
        if event in (sg.WIN_CLOSED, 'EXIT'):
            window.close()
            return
        elif event == 'Apply':
            window.close()
            tp.baudrate_change_send(list_of_baund.index(value['-COMBO2-']) + 1)
            tp.change_COM(value['-COMBO1-'].split(' ')[0])
            tp.change_baundrate(value['-COMBO2-'])
            return



if __name__ == '__main__':

    #print(tp.serial_ports())
    tp.initial_Uart_avoid_cut(s)
    menu_GUI()