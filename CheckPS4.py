import pygame
import time

pygame.init()
pygame.joystick.init()

# ตรวจสอบว่ามีจอยต่ออยู่
if pygame.joystick.get_count() == 0:
    print("❌ ไม่พบ joystick")
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"🎮 Joystick name: {joystick.get_name()}")
print(f"🔘 Number of buttons: {joystick.get_numbuttons()}")

while True:
    pygame.event.pump()

    for i in range(joystick.get_numbuttons()):
        if joystick.get_button(i):
            print(f"🔘 Button {i} is pressed")

    time.sleep(0.1)