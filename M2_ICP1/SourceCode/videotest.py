import cv2
import numpy as np


args = {'image': 'images/test.png', 'confidence': 0.2,
		'model': 'MobileNetSSD_deploy.caffemodel',
		'prototxt': 'MobileNetSSD_deploy.prototxt.txt'}
CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
	"bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
	"dog", "horse", "motorbike", "person", "pottedplant", "sheep",
	"sofa", "train", "tvmonitor"]
count = {"background":0, "aeroplane":0, "bicycle":0, "bird":0, "boat":0,
	"bottle":0, "bus":0, "car":0, "cat":0, "chair":0, "cow":0, "diningtable":0,
	"dog":0, "horse":0, "motorbike":0, "person":0, "pottedplant":0, "sheep":0,
	"sofa":0, "train":0, "tvmonitor":0}
COLORS = np.random.uniform(255, 255, size=(len(CLASSES), 3))

# load our serialized model from disk
print("[INFO] loading model...")
net = cv2.dnn.readNetFromCaffe(args["prototxt"], args["model"])

# Create a VideoCapture object and read from input file
# If the input is the camera, pass 0 instead of the video file name
cap = cv2.VideoCapture(0)
 
# Check if camera opened successfully
if (cap.isOpened()== False): 
  print("Error opening video stream or file")
 
# Read until video is completed
while(cap.isOpened()):
    # Capture frame-by-frame
    ret, image = cap.read()
    if ret == True:
        for key,value in count.items():
            count[key]=0
        (h, w) = image.shape[:2]
        blob = cv2.dnn.blobFromImage(cv2.resize(image, (300, 300)), 0.007843, (300, 300), 127.5)
        # pass the blob through the network and obtain the detections and
        # predictions
        #print("[INFO] computing object detections...")
        net.setInput(blob)
        detections = net.forward()
        for i in np.arange(0, detections.shape[2]):
            # extract the confidence (i.e., probability) associated with the
            # prediction
            confidence = detections[0, 0, i, 2]
            # filter out weak detections by ensuring the `confidence` is
            # greater than the minimum confidence
            if confidence > args["confidence"]:
                # extract the index of the class label from the `detections`,
                # then compute the (x, y)-coordinates of the bounding box for
                # the object
                idx = int(detections[0, 0, i, 1])
                box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
                (startX, startY, endX, endY) = box.astype("int")
                # display the prediction
                label = "{}: {:.2f}%".format(CLASSES[idx], confidence * 100)
                print("[INFO] {}".format(label))
                count[CLASSES[idx]]+=1
                cv2.rectangle(image, (startX, startY), (endX, endY),
                    COLORS[idx], 2)
                y = startY - 15 if startY - 15 > 15 else startY + 15
                cv2.putText(image, label, (startX, y),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, COLORS[idx], 2)
            # show the output image
            print (count)
            cv2.imshow("Output", image)
            # Display the resulting frame
            #cv2.imshow('Frame',frame)
            # Press Q on keyboard to  exit
            if cv2.waitKey(25) & 0xFF == ord('q'):
                break

        # Break the loop
    else:
        break
 
# When everything done, release the video capture object
cap.release()
 
# Closes all the frames
cv2.destroyAllWindows()
