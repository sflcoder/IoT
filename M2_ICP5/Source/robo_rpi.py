import paho.mqtt.client as paho
import time,picamera,off
import RPi.GPIO as GPIO
output = "stop";temp=""
def on_publish(client, userdata, mid):
    return mid

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_message(client, userdata, msg):
    global output
    output = str(msg.payload)
    #print(msg.topic+" "+str(msg.qos)+": "+str(msg.payload))    

#set GPIO Pins
GPIO_TRIGGER = 38
GPIO_ECHO = 40
rb=8;rf=10;lb=12;lf=16 #THIS ARE PIN FOR WHEEL DIRECTION i.e. right back,left forward
GPIO.setmode(GPIO.BOARD);GPIO.setup(rb, GPIO.OUT);GPIO.setup(rf, GPIO.OUT);GPIO.setup(lb, GPIO.OUT);GPIO.setup(lf, GPIO.OUT)
#set GPIO direction (IN / OUT)
GPIO.setup(GPIO_TRIGGER, GPIO.OUT)
GPIO.setup(GPIO_ECHO, GPIO.IN)

def distance():
    GPIO.output(GPIO_TRIGGER, True)
    time.sleep(0.00001)
    GPIO.output(GPIO_TRIGGER, False) 
    StartTime = time.time()
    StopTime = time.time()
    while GPIO.input(GPIO_ECHO) == 0:
        StartTime = time.time()
    while GPIO.input(GPIO_ECHO) == 1:
        StopTime = time.time()
    TimeElapsed = StopTime - StartTime
    distance = (TimeElapsed * 34300) / 2
    return int(distance)
 
client = paho.Client()
client.on_publish = on_publish
client.on_subscribe = on_subscribe
client.on_message = on_message
client.connect("broker.mqttdashboard.com", 1883)
client.subscribe("iot_class_icp4", qos=1)
client.loop_start()
#camera = picamera.PiCamera()
#camera.resolution = (800, 600)
img_count = 1
# https://tutorials-raspberrypi.com/raspberry-pi-ultrasonic-sensor-hc-sr04/
while True:
    dist = distance() #distance is in Centimeter
#    distance = 20
    if dist > 10:
        if temp != output: print output;temp = output; #camera.capture('imgs/snapshot'+str(img_count)+'.jpg');img_count += 1
        (rc, mid) = client.publish("iot_class_icp4/recv", "no obstacle", qos=1)
        if output == "forward":
#            print "forwaRD"
            GPIO.output(rb, 0)
            GPIO.output(rf, 1)
            GPIO.output(lb, 0)
            GPIO.output(lf, 1)
        elif output == "backward":
            GPIO.output(rb, 1)
            GPIO.output(rf, 0)
            GPIO.output(lb, 1)
            GPIO.output(lf, 0)
        elif output == "left":
            GPIO.output(rb, 0)
            GPIO.output(rf, 1)
            GPIO.output(lb, 0)
            GPIO.output(lf, 0)
            time.sleep(0.5)
            output = "stop"
        elif output == "right":
            GPIO.output(rb, 0)
            GPIO.output(rf, 0)
            GPIO.output(lb, 0)
            GPIO.output(lf, 1)
	    time.sleep(0.5)
	    output = "stop"
        elif output == "stop":
            GPIO.output(rb, 0)
            GPIO.output(rf, 0)
            GPIO.output(lb, 0)
            GPIO.output(lf, 0)
    else:
        print ("Obstacle within 10 cm")
        GPIO.output(rb, 1)
        GPIO.output(rf, 0)
        GPIO.output(lb, 0)
        GPIO.output(lf, 1)
	time.sleep(1)
        (rc, mid) = client.publish("iot_class_icp4/recv", "obstacle", qos=2)
    
