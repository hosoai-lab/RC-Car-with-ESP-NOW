import pygame
import serial
import time

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

ser = serial.Serial('COM8', 115200)
time.sleep(1)

gear = 1  # Default gear
turbo = False # Turbo mode off
current_dir = None  # "F", "B", or None
speed = 0  # ความเร็วปัจจุบัน

def send_serial(command):
    ser.write((command + "\n").encode())
    print("Sent:", command)

while True:
    pygame.event.pump()

    x = joystick.get_axis(0)
    angle = int((x + 1) * 90)
    angle = 180 - angle  # Invert for steering
    angle = min(angle, 150)
    angle = max(angle, 30) 
    send_serial(f"L{angle}")

    base_speed = 145 + (gear - 1) * 15

    if joystick.get_button(0):  # X button (เดินหน้า)
        current_dir = "F"
        speed = base_speed
        if joystick.get_button(10):  # Turbo
            speed = 255
        send_serial(f"F{speed}")
        print(f"Forward: {speed}")
    

    elif joystick.get_button(1):  # O button (ถอยหลัง)
        current_dir = "B"
        speed = base_speed
        if joystick.get_button(10):  # Turbo
            speed = 255
        send_serial(f"B{speed}")
        print(f"Backward: {speed}")

    else:
        # ถ้ายังมีทิศทาง เคยกดมาก่อน ให้ค่อย ๆ ลด speed
        if current_dir and speed > 0:
            speed -= 5
            if speed <= 0:
                speed = 0
                send_serial("S")
                print("Stop")
                current_dir = None
            else:
                send_serial(f"{current_dir}{speed}")
                print(f"{current_dir}, Slowing: {speed}")

    # เกียร์
    if joystick.get_button(13):
        gear = max(1, gear - 1)
        print(f"Gear: {gear}")
        time.sleep(0.2)
    if joystick.get_button(14):
        gear = min(6, gear + 1)
        print(f"Gear: {gear}")
        time.sleep(0.2)

    time.sleep(0.1)