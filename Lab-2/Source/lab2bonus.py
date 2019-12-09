import speech_recognition as sr 

mic_name = "USB Device 0x46d:0x825: Audio (hw:1, 0)"
sample_rate = 48000
chunk_size = 2048
r = sr.Recognizer() 
mic_list = sr.Microphone.list_microphone_names() 

keywords = ['disgusting','awesome','hello']
'''for i, microphone_name in enumerate(mic_list):
    print i, microphone_name
    if microphone_name == mic_name: 
        device_id = i 
'''
device_id = 1
#above code is for check your device id for mic.. change as per your wish
while (1):
    with sr.Microphone(device_index = device_id, sample_rate = sample_rate, chunk_size = chunk_size) as source: 
        print "\n\nSay Something"
        audio = r.listen(source)
        try: 
            text = r.recognize_google(audio) 
            print "Text: " + text                    
            print("Text: "+text)
            count = [text.count('disgusting'),text.count('awesome'),text.count('hello'),]
            for i,j in zip(count,keywords):
                print j,':',i
        except sr.UnknownValueError: 
            continue
            print("Speech Recognition could not understand audio")    
        except sr.RequestError as e:
            print("Could not request results from Speech Recognition service; {0}".format(e))


