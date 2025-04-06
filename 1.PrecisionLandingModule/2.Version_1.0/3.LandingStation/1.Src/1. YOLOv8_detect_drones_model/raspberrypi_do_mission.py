import cv2
from picamera2 import Picamera2
from ultralytics import YOLO

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

    cv2.imshow("Camera", frame)

    if cv2.waitKey(1) == ord("q"):
        break

cv2.destroyAllWindows()
picam2.stop()
