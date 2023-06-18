#!/usr/bin/python3
from paho.mqtt.client import Client
from datetime import datetime
from sys import platform
import os.path
import random

pathFile=" "
dstDir=" "
dstFile=" "
pathData=" "
msgTopic=" "
msgPayload=" " 
prevT="20"
prevH= "50"
delta_T=1.0
delta_H=2.0

def ws_fileInit():
   global pathFile
   global dstFile
   global pathData
   
   fileName="wsData.txt"
   print(platform)
   if platform=="win32":
      print("Windows")
      pathData="C:\\Users\\setzi\\Documents\\www\\html\\mqtt\\"
   if platform == "linux" or platform == "linux2":
      print("Unix")
      pathData="/var/www/html/mqtt/"
   pathFile=pathData+fileName

def ws_fileWrite():
   if not os.path.exists(pathFile):
      print("no file")
      fileData=open(pathFile,'w')
      fileData.write("Date--")
      fileData.write("Time--")
      fileData.write("Temp(C)--")
      fileData.write("Hum (%)")
      fileData.write("\n")
      #fileData.write("\r")
   else:
      fileData=open(pathFile,'a')
      print("file OK")
   fileData.close()
   return fileData

def on_connect(client, userdata, flags, rc):
    print("Connesso con successo")

def on_message(client, userdata, message):
   global msgTopic
   global msgPayload
   global prevT
   global prevH
   
   fd=open(pathFile,'a')
   #print( message.payload.decode() )
   #print("Message received-> " + message.topic + " " + str(message.payload.decode())) 
   msgTopic=message.topic
   msgPayload=str(message.payload.decode())
   #current_date = str(datetime.date.today())

   now = datetime.now()
   dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
   #print("date and time =", dt_string)
   current_date = now.strftime("%d/%m/%Y")
   current_time = now.strftime("%H:%M:%S")
   if "temperature" in msgTopic :
      if abs(float(msgPayload)-float(prevT))>=delta_T:
         print(current_time + " " + "T= " + msgPayload)
         fd.write(current_date + "--" + current_time + "--" + msgPayload + "--" + prevH + "\n")
         prevT=msgPayload
   elif "humidity" in msgTopic:
      if abs(float(msgPayload)-float(prevH))>=delta_H:
         print(current_time + " " + "H= " + msgPayload)
         fd.write(current_date + "--" + current_time + "--" + prevT + "--" + msgPayload + "\n")
         prevH=msgPayload
   fd.close()
    
global fd

ws_fileInit()
fd=ws_fileWrite()

#client = Client(client_id = "Subscriber_test")
client = Client(f'python-mqtt-{random.randint(0, 1000)}')
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.104")
client.subscribe("sensor/temperature")
client.subscribe("sensor/humidity")
client.subscribe("sensor/relay")
client.subscribe("sensor/speed")
client.loop_forever()

