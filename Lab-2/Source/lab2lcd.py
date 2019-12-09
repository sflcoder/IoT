# -*- coding: utf-8 -*-
# Example: Scrolling text on display if the string length is major than columns in display.
# Created by Dídac García.

# Import necessary libraries for communication and display use
import lcddriver
import time

import speech_recognition as sr
r = sr.Recognizer()
file = sr.AudioFile('trail1.wav')

# Load the driver and set it to "display"
# If you use something from the driver library use the "display." prefix first
display = lcddriver.lcd()

# for transcripting audio
with file as source:
    audio = r.record(source)

# Main body of code
try:
	print("Press CTRL + C for stop this script!")
	recog = r.recognize_google(audio, language = 'en-US')
	print(recog)

	def long_string(display, text = '', num_line = 1, num_cols = 20):
		""" 
		Parameters: (driver, string to print, number of line to print, number of columns of your display)
		Return: This function send to display your scrolling string.
		"""
		if(len(text) > num_cols):
			display.lcd_display_string(text[:num_cols],num_line)
			time.sleep(1)
			for i in range(len(text) - num_cols + 1):
				text_to_print = text[i:i+num_cols]
				display.lcd_display_string(text_to_print,num_line)
				time.sleep(0.2)
			time.sleep(1)
		else:
			display.lcd_display_string(text,num_line)


	# Example of short string
	#long_string(display, "Wav to text:", 1)
	#time.sleep(1)

	# Example of long string
	#long_string(display, recog, 2)
	#display.lcd_clear()
	#time.sleep(1)

	while True:
		# An example of infinite scrolling text
		long_string(display, "Wav to text:", 1)
		long_string(display, recog, 2)

except KeyboardInterrupt: # If there is a KeyboardInterrupt (when you press ctrl+c), exit the program and cleanup
	print("Cleaning up!")
	display.lcd_clear()
except sr.UnknownValueError:
    print("Google Speech Recognition could not understand audio")
except sr.RequestError as e:
    print("Could not request results from Google Speech Recognition service; {0}".format(e))