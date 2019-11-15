import socket, traceback,tweepy,picamera
from time import sleep

consumer_key ="kVrZPunNty9GVvTxNxNoWdIZ0"
consumer_secret ="e7gW91gpPXmsP7oYj7J5D49u3Z9eVoMN7ALPbicrI5T4CTXw9h"
access_token ="111261196-dSuyVaRryrNh8t84gOaGjXMt0htTAHo5tmrZillu"
access_token_secret ="ym9De0yMCv24uiwypXHpipRyZeRQ0z1zzGtCt3xPudljz"
auth = tweepy.OAuthHandler(consumer_key, consumer_secret)   
auth.set_access_token(access_token, access_token_secret) 
api = tweepy.API(auth) 
  
first_run_var = 0
camera = picamera.PiCamera()
camera.resolution = (800, 600)
ip = '';port = 5005;BUFFER_SIZE = 50;s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);s.bind((ip, port));s.listen(3)
conn, addr = s.accept();

while (1):
    try:
        print "tcp server started on:",port
#        conn, addr = s.accept();
        if (first_run_var):
		conn, addr = s.accept();
                first_run_var = 0
#        print 'Connection address:', addr
        inData = conn.recv(BUFFER_SIZE)
        print (inData)
        if ("Tweet" in inData) or ("tweet" in inData):
           try:
                api.update_status(status =inData.replace("Tweet:","")) 
                print ("Tweet Successful")
                conn.send("Tweet_Sent")
                time.sleep(5)
           except:
                print (" ")
               # conn.send("Error: not posted")
        elif ("Img" in inData) or ("img" in inData):
             camera.capture('snapshot.jpg')
             tweet ="Tweet from RaspberryPi Camera" 
             image_path ="snapshot.jpg" 
             status = api.update_with_media(image_path, tweet)  
             api.update_status(status = tweet)
             print ("Image tweeted Successfully")
         #  conn.close()
    except KeyboardInterrupt:
        camera.close()
        conn.close()
        s.shutdown(socket.SHUT_RDWR)
        break

    except:
       # print ("Some error occured.. program continue")
      	conn.close()
        s.shutdown(socket.SHUT_RDWR)
        camera.close()
        break
#sendUdp("192.168.43.203",5555,"helloTest")
# twitter link https://twitter.com/Zeal18052398
print "ended"
