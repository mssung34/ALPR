import cv2
import torch
import numpy as np
from time import time

class ObjectDetection:
    def __init__(self):
        self.cam = cv2.VideoCapture(0)   #웹캠
        self.model = torch.hub.load('ultralytics/yolov5', 'custom', path='best.pt')  #모델 불러오기
        self.device = 'cuda' if torch.cuda.is_available() else 'cpu'    #cudnn or cpu
        self.class_name = 'lisence_plate'   #객체명

    def score_frame(self, frame):
        # frame: 단일 프레임; numpy/list/tuple 형식
        # return: 프레임에서 모델이 감지한 객체의 레이블과 좌표
        self.model.to(self.device)
        frame = [frame]
        results = self.model(frame)
        labels, cord = results.xyxyn[0][:, -1].cpu().numpy(), results.xyxyn[0][:, :-1].cpu().numpy()
        return labels, cord

    def class_to_label(self):
        return self.class_name

    def plot_boxes(self, results, frame):
        # 경계상자와 레이블을 프레임에 플로팅
        # results: 프레임에서 모델이 감지한 객체의 레이블과 좌표
        # frame: 점수화된 프레임
        # return: 경계 상자와 레이블이 플로팅된 프레임
        labels, cord = results
        n = len(labels)
        x_shape, y_shape = frame.shape[1], frame.shape[0]
        for i in range(n):
            row = cord[i]
            if row[4] >= 0.2:
                x1, y1, x2, y2 = int(row[0]*x_shape), int(row[1]*y_shape), int(row[2]*x_shape), int(row[3]*y_shape)
                bgr = (0, 0, 255)
                cv2.rectangle(frame, (x1, y1), (x2, y2), bgr, 2)
                cv2.putText(frame, self.class_to_label()
                            + ': ' + str(x1) + ', ' + str(x2) + ', ' + str(y1) + ', ' + str(y2),
                            (x1, y1), cv2.FONT_HERSHEY_SIMPLEX, 0.9, bgr, 2)
        return frame

    def __call__(self):
        # 인스턴스 생성 시 호출; 프레임 단위로 비디오 로드
        while cv2.waitKey(33) < 0:
            start_time = time()
            ret, frame = self.cam.read()
            assert ret
            results = self.score_frame(frame)
            frame = self.plot_boxes(results, frame)
            end_time = time()
            fps = 1/np.round(end_time - start_time, 3)
            print(f"Frames Per Second : {fps}")
            cv2.imshow('test', frame)

if __name__ == "__main__":
    Video = ObjectDetection()
    Video()