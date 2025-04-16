import cv2
from picamera2 import Picamera2
from ultralytics import YOLO
import smbus
import RPi.GPIO as GPIO
import time

# I2C 설정
bus = smbus.SMBus(1)  # I2C 버스 번호
address = 0x08  # 아두이노 마이크로의 슬레이브 주소

# 명령 정의
COMMAND_FORWARD = 0x01
COMMAND_BACKWARD = 0x02
COMMAND_LEFT = 0x03
COMMAND_RIGHT = 0x04

def send_command(command):
    bus.write_byte(address, command)

# 인터럽트 핀 설정
interruptPin = 17  # GPIO 17을 사용
GPIO.setmode(GPIO.BCM)
GPIO.setup(interruptPin, GPIO.OUT)

def send_stop_signal():
    GPIO.output(interruptPin, GPIO.HIGH)  # 인터럽트 신호 전송
    time.sleep(0.1)
    GPIO.output(interruptPin, GPIO.LOW)

picam2 = Picamera2()
picam2.preview_configuration.main.size = (1280, 1280)
picam2.preview_configuration.main.format = "RGB888"
picam2.preview_configuration.align()
picam2.configure("preview")
picam2.start()

model = YOLO("/home/pi/best100.onnx")

screen_width = 1280
screen_height = 1280
screen_center_x = screen_width // 2
screen_center_y = screen_height // 2

margin = 35

camera_on = True

while camera_on:
    frame = picam2.capture_array()
    
    results = model(frame)
    
    inference_time = results[0].speed['inference']
    fps = 10000 / inference_time  
    text = f'FPS: {fps:.1f}'

    font = cv2.FONT_HERSHEY_SIMPLEX
    text_size = cv2.getTextSize(text, font, 1, 2)[0]
    text_x = frame.shape[1] - text_size[0] - 10  
    text_y = text_size[1] + 10  

    cv2.putText(frame, text, (text_x, text_y), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

    boxes = results[0].boxes.xyxy
    for box in boxes:
        x_min, y_min, x_max, y_max = box[0].item(), box[1].item(), box[2].item(), box[3].item()
        
        cv2.rectangle(frame, (int(x_min), int(y_min)), (int(x_max), int(y_max)), (0, 255, 0), 2)
        
        bbox_center_x = (x_min + x_max) // 2
        bbox_center_y = (y_min + y_max) // 2
        
        label = f'{results[0].names[int(results[0].boxes.cls[0].item())]} {results[0].boxes.conf[0].item():.2f}'
        cv2.putText(frame, label, (int(x_min), int(y_min - 10)), font, 0.9, (0, 255, 0), 2)

        
        cv2.circle(frame, (int(bbox_center_x), int(bbox_center_y)), 5, (0, 0, 255), -1)
        
        cv2.putText(frame, f"({int(bbox_center_x)}, {int(bbox_center_y)})", (int(bbox_center_x) + 10, int(bbox_center_y)), font, 0.5, (0, 255, 0), 1)

        diff_x = abs(bbox_center_x - screen_center_x)
        diff_y = abs(bbox_center_y - screen_center_y)

        if diff_x < margin and diff_y < margin:
            cv2.putText(frame, "Bounding box is centered within margin. Turning off the camera...", (10, 60), font, 0.5, (0, 255, 0), 1)
            cv2.waitKey(2000)  
            camera_on = False  
            send_stop_signal()  # 정지 명령 전송
        else:
            if diff_x < margin:
                horizontal_direction = "center"
            elif diff_x > margin:
                if bbox_center_x < screen_center_x:
                    horizontal_direction = "right"
                else:
                    horizontal_direction = "left"

            if diff_y < margin:
                vertical_direction = "center"
            elif diff_y > margin:
                if bbox_center_y < screen_center_y:
                    vertical_direction = "down"
                else:
                    vertical_direction = "up"

            cv2.putText(frame, f"Horizontal: {horizontal_direction}", (10, 20), font, 0.5, (0, 0, 255), 1)
            cv2.putText(frame, f"Vertical: {vertical_direction}", (10, 40), font, 0.5, (0, 0, 255), 1)
            cv2.putText(frame, "Bounding box is not centered. Camera remains on.", (10, 60), font, 0.5, (0, 255, 0), 1)

            # 명령 전송
            if vertical_direction == "down":
                send_command(COMMAND_FORWARD)
                if diff_y < 10:  # 목표 위치에 도달했을 때 정지
                    send_stop_signal()
                    vertical_direction = "center"
            elif vertical_direction == "up":
                send_command(COMMAND_BACKWARD)
                if diff_y < 10:  # 목표 위치에 도달했을 때 정지
                    send_stop_signal()
                    vertical_direction = "center"
            if horizontal_direction == "right":
                send_command(COMMAND_RIGHT)
                if diff_x < 10:  # 목표 위치에 도달했을 때 정지
                    send_stop_signal()
                    horizontal_direction = "center"
            elif horizontal_direction == "left":
                send_command(COMMAND_LEFT)
                if diff_x < 10:  # 목표 위치에 도달했을 때 정지
                    send_stop_signal()
                    horizontal_direction = "center"

    cv2.imshow("Camera", frame)

    if cv2.waitKey(1) == ord("q"):
        send_stop_signal()  # 정지 명령 전송
        break

cv2.destroyAllWindows()
picam2.stop()
