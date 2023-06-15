import cv2
import numpy as np
import imutils
import pytesseract
from PIL import Image
from PIL import ImageEnhance
import string
from picamera import PiCamera
from time import sleep
import re

'''
import time
import pyrebase
import random

config = {
	"apiKey": "AIzaSyACMBFnW44JpZD3-4lrOEL99Hn6eBmORb8",
	"authDomain": "parckingapp-monitor.firebaseapp.com",
	"databaseURL": "https://parckingapp-default-rtdb.europe-west1.firebasedatabase.app/",
	"storageBucket": "parckingapp-monitor.appspot.com"
}

firebase = pyrebase.initialize_app(config)
db = firebase.database()
'''
  
def checkFormat(text):
	rightFormat = ''
	if re.findall('^B[0-9][0-9][0-9]?[A-Z][A-Z][A-Z]', text):
		rightFormat = re.findall('^B[0-9][0-9][0-9]?[A-Z][A-Z][A-Z]', text)
		return rightFormat
	elif re.findall('[A-Z][A-Z][0-9][0-9][0-9]?[A-Z][A-Z][A-Z]', text):
		rightFormat = re.findall('[A-Z][A-Z][0-9][0-9][0-9]?[A-Z][A-Z][A-Z]', text)
		return rightFormat
	elif re.findall('[A-Z][A-Z][0-9][0-9][0-9][0-9][0-9][0-9]', text):
		rightFormat = re.findall('[A-Z][A-Z][0-9][0-9][0-9][0-9][0-9][0-9]', text)
		return rightFormat
	elif re.findall('^B[0-9][0-9][0-9][0-9][0-9][0-9]', text):
		rightFormat = re.findall('^B[0-9][0-9][0-9][0-9][0-9][0-9]', text)
		return rightFormat
	return rightFormat
	
	
def process_function(param):
	for i in range(3, 14):
		if pytesseract.image_to_string(param,config=f'-l eng --psm {i}'):
			text = pytesseract.image_to_string(param,config=f'-l eng --psm {i}')
			
			allowed_chars = string.ascii_letters + string.digits
			text = ''.join(c for c in text if c in allowed_chars)
			
			text = checkFormat(text)
			if len(text) == 0:
				text = ''
			else:
				text = text[0]
			
			return text
			

'''
camera = PiCamera()
camera.start_preview()
sleep(10)
camera.capture('/home/team2/Desktop/iotca_project/car_image.jpg')
camera.stop_preview()
'''

image0 = cv2.imread('car1.jpg')
image0 = cv2.resize(image0, (1920, 1080))

image = cv2.cvtColor(image0, cv2.COLOR_BGR2GRAY)
#cv2.imshow('Car', image)
#cv2.waitKey()

image = cv2.bilateralFilter(image, 11, 17, 17)
#cv2.imshow('Car', image)
#cv2.waitKey()

	
if np.all(cv2.Canny(image, 10, 100, apertureSize=5) != None):
	edged = cv2.Canny(image, 10, 100, apertureSize=5)
	
	
elif np.all(cv2.Canny(image, 10, 100, apertureSize=7) != None):
	edged = cv2.Canny(image, 10, 100, apertureSize=7)
	

#cv2.imshow('Car', edged)
#cv2.waitKey()

#this lines extract the contours
cnts = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cnts = imutils.grab_contours(cnts)
cnts = sorted(cnts, key = cv2.contourArea, reverse = True)[:10]
screenCnt = None 

for c in cnts:
	peri = cv2.arcLength(c, True)
	approx = cv2.approxPolyDP(c, 0.018 * peri, True)
	if len(approx) == 4:
		screenCnt = approx
		break
	
mask  = np.zeros(image.shape, np.uint8)
new_image = cv2.drawContours(mask,[screenCnt], 0, 255, -1)
new_image = cv2.bitwise_and (image0, image0, mask = mask)
#cv2.imshow('Car', new_image)
#cv2.waitKey()
#cv2.destroyAllWindows()	
		
(x, y) = np.where(mask == 255)
(topx, topy) = (np.min(x), np.min(y))
(bottomx, bottomy) = (np.max(x), np.max(y))
Cropped = image[topx:bottomx+1, topy:bottomy+1]

#cv2.imshow('Car', Cropped)
#cv2.waitKey()
#cv2.destroyAllWindows()

print(process_function(Cropped))



