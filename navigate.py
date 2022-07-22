import cv2
import numpy as np
import requests
import time 
urlarray = ['192.168.0.104','192.168.0.9','192.168.0.8','192.168.0.10']

# img1 = cv2.VideoCapture(0)
# img0 = img1[-300:0,:]

def countour_count(new_img):
    # istrue, new_img = img1.read()

    hsv_frame = cv2.cvtColor(new_img, cv2.COLOR_BGR2HSV)
    low_green=np.array([60,30,50])
    high_green=np.array([102,255,255])
    green_mask=cv2.inRange(hsv_frame,low_green,high_green)
    green=cv2.bitwise_and(new_img,new_img,mask=green_mask)

    kernel = np.ones((7, 7), np.uint8)
    img_erosion = cv2.erode(green_mask, kernel, iterations=2)

    contours, hierarchy = cv2.findContours(img_erosion,
                                           cv2.RETR_TREE,
                                           cv2.CHAIN_APPROX_SIMPLE)
    number_of_objects_in_image= len(contours)
    return number_of_objects_in_image



bot = True
origin1=[274,29]
origin2=[304,26]
origin3=[334,29]
origin4=[363,26]

origin = [origin1,origin2,origin3,origin4]

p1=[[272,294], [41,292]]
p2=[[303,325], [41,317]]
p3=[[331,300], [580,303]]
p4=[[357,326], [582,329]]

point = [p1,p2,p3,p4]

# origin1=[]
# origin2=[]
# origin3=[]
# origin4=[]

# p1 = []  # stores the clicked points
# p2 = []
# p3 = []
# p4 = []

# --------------------------------------- HELPER FUNCTION --------------------------------------------
# i = 0
# for i in range(4):

#     def click_event(event, x, y, flags, params):                           
#         if event == cv2.EVENT_LBUTTONDOWN:
#             cv2.circle(img, (x, y), 10, (0, 0, 0), -1)
#             cv2.circle(img, (x, y), 5, (255, 255, 255), -1)
#             if (i == 0):
#                 p1.append([x, y])
#                 arr = np.array(p1)
#                 print(p1)
#                 cv2.polylines(img, [arr], False, (255, 0, 0), 4)
#                 cv2.imshow('image', img)
#             elif (i == 1):
#                 p2.append([x, y])
#                 arr = np.array(p2)
#                 print(p2)
#                 cv2.polylines(img, [arr], False, (255, 0, 0), 4)
#                 cv2.imshow('image', img)

#             elif (i == 2):
#                 p3.append([x, y])
#                 arr = np.array(p3)
#                 print(p3)
#                 cv2.polylines(img, [arr], False, (255, 0, 0), 4)
#                 cv2.imshow('image', img)
#             elif (i == 3):
#                 p4.append([x, y])
#                 arr = np.array(p4)
#                 print(p4)
#                 cv2.polylines(img, [arr], False, (255, 0, 0), 4)
#                 cv2.imshow('image', img)


def cordinate(n):
    x_arr = n[::2]
    x= sum(x_arr)
    x = x / len(x_arr)

    y_arr = n[1::2]
    y = sum(y_arr)
    y = y / len(y_arr)
    print(x,y)
    return x,y


def getContours(img):
     
    

    contours, hierarchy = cv2.findContours(img,
                                           cv2.RETR_TREE,
                                           cv2.CHAIN_APPROX_SIMPLE)
    for contour in contours:
        area = cv2.contourArea(contour)
        if (area > 200):
            approx = cv2.approxPolyDP(contour, 0.009 * cv2.arcLength(contour, True), True)
            cv2.drawContours(new_img, [approx], 0, (0, 0, 255), 5)
            n = approx.ravel()
            x, y = cordinate(n)
            # print(x_cor, y_cor)
            return x,y
    return [-1, -1] 




# url="http://192.168.0.3:8080/video"
# Put 0 for webcam and link for IP webcam or enter 1 if using Droidcam via USB
capture = cv2.VideoCapture(1)
# URL = "http://192.168.0.106/Flip"

while True:

  
    # r=requests.get(url="http://" + urlarray[0]+"/Forward")
    # time.sleep(0.2)
    arr = []
    istrue, img = capture.read()
    # cropped_img=img[:50,:]
    new_img=img
    
    hsv_frame = cv2.cvtColor(new_img, cv2.COLOR_BGR2HSV)
    low_green=np.array([60,30,50])
    high_green=np.array([102,255,255])
    green_mask=cv2.inRange(hsv_frame,low_green,high_green)
    green=cv2.bitwise_and(new_img,new_img,mask=green_mask)

    kernel = np.ones((7, 7), np.uint8)
    img_erosion = cv2.erode(green_mask, kernel, iterations=1)
    img_dilation=cv2.dilate(img_erosion,kernel,iterations=2)

    x_cor,y_cor=getContours(img_dilation)
    cv2.circle(new_img, (int(x_cor), int(y_cor)), 3, (0, 0, 255), 3, cv2.FILLED)

    arr.append([int(x_cor), int(y_cor)])
    value = sorted(arr, key=lambda k: k[0])
    print(value)
    if int(x_cor) == int(y_cor) == -1 : bot = False
    bot0=value[0] 
    bot1=value[1]
    bot2=value[2]
    bot3=value[3]

    bot = [bot0,bot1,bot2,bot3]

    i=0
    

    for i in range(4):

        if (bot == False):
            r=requests.get(url="http://" + urlarray[i]+"/Forward")
            time.sleep(0.2)

        else:

            while (point[i][0][1]-bot[i][1]>20):  # y tolerance 

                diff=point[i][0][0]-bot[i][0]   # x tolerance
                if (diff>0):
                    if (diff>20):
                        r = requests.get(url = "http://" + urlarray[i]+"/sleft")
                elif (diff<0):
                    if (abs(diff)>20):
                        r = requests.get(url = "http://" + urlarray[i]+"/sright")  
            #   turn() for making it turn 90 degrees
            
            while (point[i][1][0]-bot[i][0]>10):  # x tolerance
                diff=point[i][0][1]-bot[i][1]     # y tolerance
                if (diff>0):
                    if (diff>20):
                        r = requests.get(url = "http://" + urlarray[i]+"/sright")
                elif (diff<0):
                    if (abs(diff)>20):
                        r = requests.get(url = "http://" + urlarray[i]+"/sleft")

            r = requests.get(url = "http://"+urlarray[i]+"/Flip")    # bot reched now flip to deliver the parcel

            while (point[i][0][0]-bot[i][0]>10):   
                diff=point[i][0][1]-bot[i][1]
                if (diff>0):
                    if (diff>20):
                        #anti
                        r = requests.get(url = "http://" + urlarray[i]+"/sright")
                elif (diff<0):
                    if (abs(diff)>20):
                        #clock
                        r = requests.get(url = "http://" + urlarray[i]+"/sleft")

            #   turn() for making it turn 180 degrees

            while (origin[i][1]-bot[i][1]>20):
                diff=origin[i][0]-bot[i][0]
                if (diff>0):
                    if (diff>20):
                        #anti
                        r = requests.get(url = "http://" + urlarray[i]+"/sleft")
                elif (diff<0):
                    if (abs(diff)>20):
                        #clock
                        r = requests.get(url = "http://" + urlarray[i]+"/sright")
            i=i+1

                    
#***********************************************************************************************************************************************************************************
            

    cv2.imshow("Video", new_img)
    cv2.imshow("Bots", green)
    # r = requests.get(url=URL)
    if cv2.waitKey(20) & 0xFF == ord('d'):
        break

capture.release()
cv2.destroyAllWindows()