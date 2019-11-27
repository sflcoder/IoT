import RPi.GPIO as GPIO
def off():
	import sys
	sys.path.append('/storage/.kodi/addons/python.RPi.GPIO/lib/')
	import RPi.GPIO as GPIO
	GPIO.setmode(GPIO.BOARD)
	for x in range(0,41):
		try:
			GPIO.setup(x,GPIO.OUT)
			GPIO.output(x,0)
		except:
			continue
off()
GPIO.cleanup()
print "All GPIO pins are turned Off."
