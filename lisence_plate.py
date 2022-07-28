import sys
import socket
import cv2
import torch
import numpy as np
import matplotlib.pyplot as plt
import pytesseract
pytesseract.pytesseract.tesseract_cmd = R'C:\Program Files\Tesseract-OCR\tesseract'
plt.style.use('dark_background')

BUF_SIZE = 1024
IP = "10.10.20.231"
Port = 9022

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((IP, Port))

class ObjectDetection:
    def __init__(self): #생성자
        self.cam = cv2.VideoCapture(0)   #웹캠
        self.model = torch.hub.load('ultralytics/yolov5', 'custom', path='best.pt')  #모델 불러오기
        self.device = 'cuda' if torch.cuda.is_available() else 'cpu'    #cudnn or cpu
        self.class_name = 'lisence_plate'   #객체명
        self.save_data = ' '

    def preprocessing(self, img):   #이미지 전처리 후 읽기
        height, width, channel = img.shape

        #gray 처리
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        structuringElement = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))

        imgTopHat = cv2.morphologyEx(gray, cv2.MORPH_TOPHAT, structuringElement)
        imgBlackHat = cv2.morphologyEx(gray, cv2.MORPH_BLACKHAT, structuringElement)

        imgGrayscalePlusTopHat = cv2.add(gray, imgTopHat)
        gray = cv2.subtract(imgGrayscalePlusTopHat, imgBlackHat)

        #가우시안 블러 처리 (노이즈 제거)
        img_blurred = cv2.GaussianBlur(gray, ksize=(5, 5), sigmaX=0)

        #thresh hold (흑색, 백색)
        img_thresh = cv2.adaptiveThreshold(
            img_blurred, 
            maxValue=255.0, 
            adaptiveMethod=cv2.ADAPTIVE_THRESH_GAUSSIAN_C, 
            thresholdType=cv2.THRESH_BINARY_INV, 
            blockSize=19, 
            C=9
        )

        #윤곽선 따기
        contours, _ = cv2.findContours(
            img_thresh, 
            mode=cv2.RETR_LIST, 
            method=cv2.CHAIN_APPROX_SIMPLE
        )

        temp_result = np.zeros((height, width, channel), dtype=np.uint8)

        cv2.drawContours(temp_result, contours=contours, contourIdx=-1, color=(255, 255, 255))

        #사각형 검출
        temp_result = np.zeros((height, width, channel), dtype=np.uint8)

        contours_dict = []

        for contour in contours:
            x, y, w, h = cv2.boundingRect(contour)
            cv2.rectangle(temp_result, pt1=(x, y), pt2=(x+w, y+h), color=(255, 255, 255), thickness=2)

            # insert to dict
            contours_dict.append({
                'contour': contour,
                'x': x,
                'y': y,
                'w': w,
                'h': h,
                'cx': x + (w / 2),
                'cy': y + (h / 2)
            })


        #규칙성 활용해서 번호판 검출
        MIN_AREA = 80
        MIN_WIDTH, MIN_HEIGHT = 2, 8
        MIN_RATIO, MAX_RATIO = 0.25, 1.0

        possible_contours = []

        cnt = 0
        for d in contours_dict:
            area = d['w'] * d['h']
            ratio = d['w'] / d['h']

            if area > MIN_AREA \
            and d['w'] > MIN_WIDTH and d['h'] > MIN_HEIGHT \
            and MIN_RATIO < ratio < MAX_RATIO:
                d['idx'] = cnt
                cnt += 1
                possible_contours.append(d)

        temp_result = np.zeros((height, width, channel), dtype=np.uint8)

        for d in possible_contours:
            cv2.rectangle(temp_result, pt1=(d['x'], d['y']), pt2=(d['x']+d['w'], d['y']+d['h']), color=(255, 255, 255), thickness=2)

        MAX_DIAG_MULTIPLYER = 5 # 5
        MAX_ANGLE_DIFF = 12.0 # 12.0
        MAX_AREA_DIFF = 0.5 # 0.5
        MAX_WIDTH_DIFF = 0.8
        MAX_HEIGHT_DIFF = 0.2
        MIN_N_MATCHED = 3 # 3

        def find_chars(contour_list):
            matched_result_idx = []

            for d1 in contour_list:
                matched_contours_idx = []
                for d2 in contour_list:
                    if d1['idx'] == d2['idx']:
                        continue
                    
                    dx = abs(d1['cx'] - d2['cx'])
                    dy = abs(d1['cy'] - d2['cy'])

                    diagonal_length1 = np.sqrt(d1['w'] ** 2 + d1['h'] ** 2)

                    distance = np.linalg.norm(np.array([d1['cx'], d1['cy']]) - np.array([d2['cx'], d2['cy']]))
                    if dx == 0:
                        angle_diff = 90
                    else:
                        angle_diff = np.degrees(np.arctan(dy / dx))
                    area_diff = abs(d1['w'] * d1['h'] - d2['w'] * d2['h']) / (d1['w'] * d1['h'])
                    width_diff = abs(d1['w'] - d2['w']) / d1['w']
                    height_diff = abs(d1['h'] - d2['h']) / d1['h']

                    if distance < diagonal_length1 * MAX_DIAG_MULTIPLYER \
                    and angle_diff < MAX_ANGLE_DIFF and area_diff < MAX_AREA_DIFF \
                    and width_diff < MAX_WIDTH_DIFF and height_diff < MAX_HEIGHT_DIFF:
                        matched_contours_idx.append(d2['idx'])

                matched_contours_idx.append(d1['idx'])

                if len(matched_contours_idx) < MIN_N_MATCHED:
                    continue
                
                matched_result_idx.append(matched_contours_idx)

                unmatched_contour_idx = []
                for d4 in contour_list:
                    if d4['idx'] not in matched_contours_idx:
                        unmatched_contour_idx.append(d4['idx'])

                unmatched_contour = np.take(possible_contours, unmatched_contour_idx)

                recursive_contour_list = find_chars(unmatched_contour)

                for idx in recursive_contour_list:
                    matched_result_idx.append(idx)

                break
            
            return matched_result_idx

        result_idx = find_chars(possible_contours)

        matched_result = []
        for idx_list in result_idx:
            matched_result.append(np.take(possible_contours, idx_list))

        temp_result = np.zeros((height, width, channel), dtype=np.uint8)

        for r in matched_result:
            for d in r:
                cv2.rectangle(temp_result, pt1=(d['x'], d['y']), pt2=(d['x']+d['w'], d['y']+d['h']), color=(255, 255, 255), thickness=2)

        #기울어진 사진 수평 맞춰주기
        PLATE_WIDTH_PADDING = 1.3 # 1.3
        PLATE_HEIGHT_PADDING = 1.5 # 1.5
        MIN_PLATE_RATIO = 3
        MAX_PLATE_RATIO = 10

        plate_imgs = []
        plate_infos = []

        for i, matched_chars in enumerate(matched_result):
            sorted_chars = sorted(matched_chars, key=lambda x: x['cx'])

            plate_cx = (sorted_chars[0]['cx'] + sorted_chars[-1]['cx']) / 2
            plate_cy = (sorted_chars[0]['cy'] + sorted_chars[-1]['cy']) / 2

            plate_width = (sorted_chars[-1]['x'] + sorted_chars[-1]['w'] - sorted_chars[0]['x']) * PLATE_WIDTH_PADDING

            sum_height = 0
            for d in sorted_chars:
                sum_height += d['h']

            plate_height = int(sum_height / len(sorted_chars) * PLATE_HEIGHT_PADDING)

            triangle_height = sorted_chars[-1]['cy'] - sorted_chars[0]['cy']
            triangle_hypotenus = np.linalg.norm(
                np.array([sorted_chars[0]['cx'], sorted_chars[0]['cy']]) - 
                np.array([sorted_chars[-1]['cx'], sorted_chars[-1]['cy']])
            )

            angle = np.degrees(np.arcsin(triangle_height / triangle_hypotenus))

            rotation_matrix = cv2.getRotationMatrix2D(center=(plate_cx, plate_cy), angle=angle, scale=1.0)

            img_rotated = cv2.warpAffine(img_thresh, M=rotation_matrix, dsize=(width, height))

            img_cropped = cv2.getRectSubPix(
                img_rotated, 
                patchSize=(int(plate_width), int(plate_height)), 
                center=(int(plate_cx), int(plate_cy))
            )

            if img_cropped.shape[1] / img_cropped.shape[0] < MIN_PLATE_RATIO or img_cropped.shape[1] / img_cropped.shape[0] < MIN_PLATE_RATIO > MAX_PLATE_RATIO:
                continue
            
            plate_imgs.append(img_cropped)
            plate_infos.append({
                'x': int(plate_cx - plate_width / 2),
                'y': int(plate_cy - plate_height / 2),
                'w': int(plate_width),
                'h': int(plate_height)
            })


        #최종확인
        longest_idx, longest_text = -1, 0
        plate_chars = []

        for i, plate_img in enumerate(plate_imgs):
            plate_img = cv2.resize(plate_img, dsize=(0, 0), fx=1.6, fy=1.6)
            _, plate_img = cv2.threshold(plate_img, thresh=0.0, maxval=255.0, type=cv2.THRESH_BINARY | cv2.THRESH_OTSU)

            contours, _ = cv2.findContours(plate_img, mode=cv2.RETR_LIST, method=cv2.CHAIN_APPROX_SIMPLE)

            plate_min_x, plate_min_y = plate_img.shape[1], plate_img.shape[0]
            plate_max_x, plate_max_y = 0, 0

            for contour in contours:
                x, y, w, h = cv2.boundingRect(contour)

                area = w * h
                ratio = w / h

                if area > MIN_AREA \
                and w > MIN_WIDTH and h > MIN_HEIGHT \
                and MIN_RATIO < ratio < MAX_RATIO:
                    if x < plate_min_x:
                        plate_min_x = x
                    if y < plate_min_y:
                        plate_min_y = y
                    if x + w > plate_max_x:
                        plate_max_x = x + w
                    if y + h > plate_max_y:
                        plate_max_y = y + h

            img_result = plate_img[plate_min_y:plate_max_y, plate_min_x:plate_max_x]

            img_result = cv2.GaussianBlur(img_result, ksize=(3, 3), sigmaX=0)
            _, img_result = cv2.threshold(img_result, thresh=0.0, maxval=255.0, type=cv2.THRESH_BINARY | cv2.THRESH_OTSU)
            img_result = cv2.copyMakeBorder(img_result, top=10, bottom=10, left=10, right=10, borderType=cv2.BORDER_CONSTANT, value=(0,0,0))

            chars = pytesseract.image_to_string(img_result, lang='kor', config='--psm 7 --oem 0')

            result_chars = ''
            has_digit = False
            for c in chars:
                if ord('가') <= ord(c) <= ord('힣') or c.isdigit():
                    if c.isdigit():
                        has_digit = True
                    result_chars += c

            #print(result_chars)
            plate_chars.append(result_chars)

            if has_digit and len(result_chars) > longest_text:
                longest_idx = i

            # plt.subplot(len(plate_imgs), 1, i+1)
            # plt.imshow(img_result, cmap='gray')
            # plt.show()
            return plate_chars

    def score_frame(self, frame):
        # frame: 단일 프레임. numpy/list/tuple 형식
        self.model.to(self.device)
        frame = [frame]
        results = self.model(frame)
        labels, cord = results.xyxyn[0][:, -1].cpu().numpy(), results.xyxyn[0][:, :-1].cpu().numpy()
        return labels, cord #모델이 찾아낸 객체의 레이블, 좌표

    def class_to_label(self):
        return self.class_name

    def plot_boxes(self, results, frame):
        # results: 프레임에서 모델이 감지한 객체의 레이블과 좌표
        labels, cord = results
        n = len(labels)
        x_shape, y_shape = frame.shape[1], frame.shape[0]
        for i in range(n):
            row = cord[i]
            if row[4] >= 0.2:
                x1, y1, x2, y2 = int(row[0]*x_shape), int(row[1]*y_shape), int(row[2]*x_shape), int(row[3]*y_shape)
                bgr = (0, 0, 255)   #blue green red
                cv2.rectangle(frame, (x1, y1), (x2, y2), bgr, 2)    #좌표에 맞춰서 사각형 그리기
                cv2.putText(frame, self.class_to_label()    #라벨 붙이기
                            + ': ' + str(x1) + ', ' + str(x2) + ', ' + str(y1) + ', ' + str(y2),
                            (x1, y1), cv2.FONT_HERSHEY_SIMPLEX, 0.9, bgr, 2)
                cut_plate = frame[y1:y2, x1:x2] #번호판 자르기
                plate_data = self.preprocessing(cut_plate)
                if plate_data == None:
                    pass
                else:
                    if plate_data == self.save_data:
                        pass
                    else:
                        #통신 해야함
                        self.save_data = plate_data[0]
                        sock.send(self.save_data.encode())
                        print(self.save_data)
        return frame

    def __call__(self): #객체 instance 시 호출 함수
        while cv2.waitKey(33) < 0:  #키 입력 없을 경우 한 프레임 띄우고 33만큼 delay
            ret, frame = self.cam.read()
            assert ret  #ret 참일 경우
            results = self.score_frame(frame)   #객체 탐지
            frame = self.plot_boxes(results, frame) #사각형, 라벨 그리기
            cv2.imshow('test', frame)

if __name__ == "__main__":
    Video = ObjectDetection()
    Video()