import cv2
import numpy as np

font = cv2.FONT_HERSHEY_SIMPLEX

def process_frame(frame):
    # Ubah frame menjadi mode warna HSV
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Buat mask dengan mengambil warna oranye saja
    mask = cv2.inRange(hsv_frame, low_orange, high_orange)

    # Temukan kontur objek-oranye
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    if len(contours) > 0:
        # Temukan kontur dengan luas terbesar (warna terbanyak)
        largest_contour = max(contours, key=cv2.contourArea)
        # Gambar kotak pada kontur terbesar
        x, y, w, h = cv2.boundingRect(largest_contour)
        #posisi =int(x+(w/2))
        frame_center = frame.shape[1] // 2
        #tampilkan garis
        # cv2.line(frame,(frame_center,220),(x,y),(300,100,5),2)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # Tentukan posisi korban (kanan, kiri, atau tengah)
        text_position = (10,30)
        text_position2 = (10,60)
        if x > frame_center + w // 2:
            text = "Posisi : Kanan"
        elif x + w < frame_center - w // 2:
            text = "Posisi    : Kiri"
        else:
            text = "Posisi    : Tengah"
        if x>y:
            text2 ="Keadaan : Normal"
        else:
            text2 ="Keadaan : Buntung"
        # Tambahkan teks pada gambar
        cv2.putText(frame, text, text_position, font, 0.7, (0, 255, 0), 2)
        cv2.putText(frame, text2, text_position2, font, 0.7, (0, 255, 0), 2)

    return frame

# Batas bawah dan atas warna oren dalam format HSV
low_orange = np.array([2, 155, 150])
high_orange = np.array([12, 255, 255])

cap = cv2.VideoCapture(0)

while True:
    #Menampilkan fps counter 
    timer = cv2.getTickCount()
    success, frame = cap.read()
    fps_counter = cv2.getTickFrequency()/(cv2.getTickCount()-timer)
    processed_frame = process_frame(frame)
    cv2.putText(processed_frame, "FPS :" + str(int(fps_counter)), (480,40), font, 1, (0,255,0), 2, cv2.LINE_AA)
    cv2.imshow("Softeng 622020016", processed_frame)
    key = cv2.waitKey(1)
    # Perncet Tombol ESC buat keluar
    if key == 27:  
        break

cap.release()
cv2.destroyAllWindows()