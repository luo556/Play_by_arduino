import cv2
import mediapipe as mp
import math
import time
import serial
import random

import numpy as np

ser = serial.Serial('COM6', 9600)
record_list = []
model_change = 0
x_tep = 0
model_change_2 = 0
def light_show(frame, num):
    if num == 1:
        cv2.putText(frame, 'Scissors', (int(frame.shape[0] * 0.75) + 20, 270), 0, 1.3, (0, 0, 255), 2)
    if num == 2:
        cv2.putText(frame, 'Rock', (int(frame.shape[0] * 0.75) + 20, 270), 0, 1.3, (0, 0, 255), 2)
    if num == 3:
        cv2.putText(frame, 'Paper', (int(frame.shape[0] * 0.75) + 20, 270), 0, 1.3, (0, 0, 255), 2)
    else:
        return 0


def ren_show(frame, num):
    if num == 1:
        cv2.putText(frame, 'Scissors', (int(frame.shape[0] / 4) + 20, 270), 0, 1.3, (0, 0, 255), 2)
    if num == 2:
        cv2.putText(frame, 'Rock', (int(frame.shape[0] / 4) + 20, 270), 0, 1.3, (0, 0, 255), 2)
    if num == 3:
        cv2.putText(frame, 'Paper', (int(frame.shape[0] / 4) + 20, 270), 0, 1.3, (0, 0, 255), 2)
    else:
        return 0


def game_input(frame, a, b):
    global x_tep
    global model_change_2

    ren_show(frame, a)
    # print("x_tep", x_tep)
    light_show(frame, x_tep)
    if a == x_tep:
        cv2.putText(frame, 'draw', (int(frame.shape[0] / 2) + 50, 400), 0, 1.3, (88, 0, 255), 2)
        the_state = 3

    elif x_tep ==0:
        cv2.putText(frame, 'wait', (int(frame.shape[0] / 2) + 50, 400), 0, 1.3, (88, 0, 255), 2)
    elif a == 1 and x_tep == 3:
        cv2.putText(frame, 'gamer win', (int(frame.shape[0] / 2) + 50, 400), 0, 1.3, (88, 0, 255), 2)
        the_state = 1
        # ser.write(b'1')

    elif a == 2 and x_tep == 1:
        cv2.putText(frame, 'gamer win', (int(frame.shape[0] / 2) + 50, 400), 0, 1.3, (88, 0, 255), 2)
        the_state = 1
        # ser.write(b'1')

    elif a == 3 and x_tep == 2:
        cv2.putText(frame, 'gamer win', (int(frame.shape[0] / 2) + 50, 400), 0, 1.3, (88, 0, 255), 2)
        the_state = 1

    else:
        cv2.putText(frame, 'light win', (int(frame.shape[0] / 2) + 50, 400), 0, 1.3, (88, 0, 255), 2)
        the_state = 2

    if model_change_2 == 1 and b == 1 :
        print("m2", model_change_2)
        print('x', x_tep)
        print('a', a)
        if x_tep == 1 and a == 1:
            ser.write(b'1')
            model_change_2 = 0

        elif x_tep ==1  and a == 2:
            ser.write(b'2')
            model_change_2 = 0
        elif x_tep == 1 and a == 3:
            ser.write(b'3')
            model_change_2 = 0
        elif x_tep == 2 and a == 1:
            ser.write(b'4')
            model_change_2 = 0
        elif x_tep == 2 and a == 2:
            ser.write(b'5')
            model_change_2 = 0
        elif x_tep == 2 and a == 3:
            ser.write(b'6')
            model_change_2 = 0
        elif x_tep == 3 and a == 1:
            ser.write(b'7')
            model_change_2 = 0
        elif x_tep == 3 and a == 2:
            ser.write(b'8')
            model_change_2 = 0
        elif x_tep == 3 and a == 3:
            ser.write(b'9')
            model_change_2 = 0

def vector_2d_angle(v1, v2):
    '''
        求解二维向量的角度
    '''
    v1_x = v1[0]
    v1_y = v1[1]
    v2_x = v2[0]
    v2_y = v2[1]
    try:
        angle_ = math.degrees(math.acos(
            (v1_x * v2_x + v1_y * v2_y) / (((v1_x ** 2 + v1_y ** 2) ** 0.5) * ((v2_x ** 2 + v2_y ** 2) ** 0.5))))
    except:
        angle_ = 65535.
    if angle_ > 180.:
        angle_ = 65535.
    return angle_


def hand_angle(hand_):
    '''
        获取对应手相关向量的二维角度,根据角度确定手势
    '''
    angle_list = []
    # ---------------------------- thumb 大拇指角度
    angle_ = vector_2d_angle(
        ((int(hand_[0][0]) - int(hand_[2][0])), (int(hand_[0][1]) - int(hand_[2][1]))),
        ((int(hand_[3][0]) - int(hand_[4][0])), (int(hand_[3][1]) - int(hand_[4][1])))
    )
    angle_list.append(angle_)
    # ---------------------------- index 食指角度
    angle_ = vector_2d_angle(
        ((int(hand_[0][0]) - int(hand_[6][0])), (int(hand_[0][1]) - int(hand_[6][1]))),
        ((int(hand_[7][0]) - int(hand_[8][0])), (int(hand_[7][1]) - int(hand_[8][1])))
    )
    angle_list.append(angle_)
    # ---------------------------- middle 中指角度
    angle_ = vector_2d_angle(
        ((int(hand_[0][0]) - int(hand_[10][0])), (int(hand_[0][1]) - int(hand_[10][1]))),
        ((int(hand_[11][0]) - int(hand_[12][0])), (int(hand_[11][1]) - int(hand_[12][1])))
    )
    angle_list.append(angle_)
    # ---------------------------- ring 无名指角度
    angle_ = vector_2d_angle(
        ((int(hand_[0][0]) - int(hand_[14][0])), (int(hand_[0][1]) - int(hand_[14][1]))),
        ((int(hand_[15][0]) - int(hand_[16][0])), (int(hand_[15][1]) - int(hand_[16][1])))
    )
    angle_list.append(angle_)
    # ---------------------------- pink 小拇指角度
    angle_ = vector_2d_angle(
        ((int(hand_[0][0]) - int(hand_[18][0])), (int(hand_[0][1]) - int(hand_[18][1]))),
        ((int(hand_[19][0]) - int(hand_[20][0])), (int(hand_[19][1]) - int(hand_[20][1])))
    )
    angle_list.append(angle_)
    return angle_list


def h_gesture(angle_list):
    '''
        # 二维约束的方法定义手势
        # fist five gun love one six three thumbup yeah
    '''
    thr_angle = 65.  # 手指闭合则大于这个值（大拇指除外）
    thr_angle_thumb = 53.  # 大拇指闭合则大于这个值
    thr_angle_s = 49.  # 手指张开则小于这个值
    gesture_str = "Unknown"
    if 65535. not in angle_list:
        if (angle_list[0] > thr_angle_thumb) and (angle_list[1] > thr_angle) and (angle_list[2] > thr_angle) and (
                angle_list[3] > thr_angle) and (angle_list[4] > thr_angle):
            gesture_str = "rock"


        elif (angle_list[0] > thr_angle_thumb) and (angle_list[1] < thr_angle_s) and (angle_list[2] < thr_angle_s) and (
                angle_list[3] > thr_angle) and (angle_list[4] > thr_angle):
            gesture_str = "scissor"



        elif (angle_list[0] < thr_angle_s) and (angle_list[1] < thr_angle_s) and (angle_list[2] < thr_angle_s) and (
                angle_list[3] < thr_angle_s) and (angle_list[4] < thr_angle_s):
            gesture_str = "paper"

    return gesture_str

list_jilu = []
def detect():
    global model_change_2
    global  record_list
    global x_tep

    model_change = 0
    the_zhuang = 0
    mp_drawing = mp.solutions.drawing_utils
    mp_hands = mp.solutions.hands
    hands = mp_hands.Hands(
        static_image_mode=False,
        max_num_hands=1,
        min_detection_confidence=0.75,
        min_tracking_confidence=0.75)
    cap = cv2.VideoCapture(0)
    while True:
        pppp = 0
        ret, img = cap.read()
        frame = cv2.resize(img, (int(img.shape[1] * 1.5), int(img.shape[0] * 1.5)), cv2.INTER_NEAREST)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        frame = cv2.flip(frame, 1)
        results = hands.process(frame)
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        # print(results.multi_handedness)

        if results.multi_handedness:
            for hand_label in results.multi_handedness:
                hand_jugg = str(hand_label).split('"')[1]
                # print(hand_jugg)

        if results.multi_hand_landmarks:

            for hand_landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)
                hand_local = []
                for i in range(21):
                    x = hand_landmarks.landmark[i].x * frame.shape[1]
                    y = hand_landmarks.landmark[i].y * frame.shape[0]
                    hand_local.append((x, y))
                if hand_local:
                    angle_list = hand_angle(hand_local)
                    gesture_str = h_gesture(angle_list)
                    # print(gesture_str)
                    if hand_jugg == 'Right':
                        model_change_2 =1
                    if hand_jugg == 'Left':
                        if gesture_str == "rock":
                            model_change = 2
                            pose_state = 1


                        if gesture_str == "scissor":
                            model_change = 1
                            pose_state = 1
                            list_jilu.append(model_change)

                        if gesture_str == "paper":
                            model_change = 3
                            pose_state = 1
                            list_jilu.append(model_change)


                    if model_change_2 == 1 and hand_jugg == "Left":
                            print('ioioui')
                                # ser.write(b'3')
                            if len(record_list) > 9:
                                record_list.append(model_change)
                                record_list.pop(0)
                            else:
                                record_list.append(model_change)
                                # print(sum(record_list))
                                # print(len(record_list))
                            if sum(record_list) >5 and len(record_list) == 10:
                                    # print('yes')
                                x_tep = random.randint(1, 3)

                                counts = np.bincount(record_list)

                                the_zhuang = np.argmax(counts)

                                pppp = 1
                                record_list = []




                            # print('mode2:',model_change_2)
        kkk = game_input(frame, the_zhuang, pppp)
        cv2.putText(frame, 'game', (int(frame.shape[0] / 2), 75), 0, 3, (255, 0, 255), 4)
        cv2.putText(frame, 'you:', (int(frame.shape[0] / 4), 230), 0, 2, (255, 0, 0), 2)
        cv2.putText(frame, 'light:', (int(frame.shape[0] * 0.75), 230), 0, 2, (0, 255, 255), 2)
        cv2.putText(frame, 'result:', (int(frame.shape[0] / 2), 350), 0, 1.3, (256, 59, 255), 2)
        # print('model_change:',model_change)
        cv2.imshow('MediaPipe Hands', frame)
        # print('model_change:',model_change)
        # print('model_change_2:', model_change_2)
        # ser.write(str(x_tep).encode('utf-8'))
        # print(x_tep)

        # print(y_tep)

        if cv2.waitKey(1) & 0xFF == 27:
            break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    detect()


