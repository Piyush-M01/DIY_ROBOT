import cv2
import numpy as np
import pandas as pd

# define the mouse callback function
def get_input_points(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONUP:
        input_pts.append((x, y))
        cv2.circle(img, (x, y), 5, (0, 255, 0), -1)
        cv2.imshow('Input Image', img)

def get_output_points(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONUP:
        output_pts.append((x, y))
        cv2.circle(output_img, (x, y), 5, (0, 255, 0), -1)
        cv2.imshow('Output Image', output_img)

# load the input image
img = cv2.imread('/home/piyush/diy_bot/src/lane_mapping/src/lane.png')
cv2.imshow('Input Image', img)

# create an empty list to store the input points
input_pts = []

# set the mouse callback function for the input image window
cv2.setMouseCallback('Input Image', get_input_points)

# wait for the user to select four input points
while len(input_pts) < 4:
    cv2.waitKey(100)

# load the output image
output_img = cv2.imread('/home/piyush/diy_bot/src/lane_mapping/src/outputs.png')
cv2.imshow('Output Image', output_img)

# create an empty list to store the output points
output_pts = []

# set the mouse callback function for the output image window
cv2.setMouseCallback('Output Image', get_output_points)

# wait for the user to select four output points
while len(output_pts) < 4:
    cv2.waitKey(100)

# convert the input and output points to NumPy arrays
input_pts = np.float32(input_pts)
output_pts = np.float32(output_pts)

# calculate the perspective transform matrix
M = cv2.getPerspectiveTransform(input_pts, output_pts)
print(type(M))

data = pd.DataFrame(M)
data.to_csv('matrix.csv',index=False)

# apply the perspective transform
output_img = cv2.warpPerspective(img, M, (output_img.shape[1], output_img.shape[0]))

# display the output image
cv2.imshow('Output Image', output_img)
cv2.waitKey(0)
cv2.destroyAllWindows()