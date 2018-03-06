import serial, struct, json
from time import sleep
from urllib import urlopen
ser = serial.Serial('/dev/ttyUSB0', 9600)
ser.flushInput()
lcurl = "https://maps.googleapis.com/maps/api/distancematrix/json?origin\
s=202+West+Chester+Street,+Lafayette,+CO&destinations=Laughing+Coyote+Project,+North+65th+Street\
,+Longmont,+CO&departure_time=now&traffic_model=best_guess&key=AIzaSyAUl3FNewt5gQKEzuOjBU6CTqCMcfmpuM4"

apexurl = "https://maps.googleapis.com/maps/api/distancematrix/json?origins=202+West+Chester+S\
treet,+Lafayette,+CO&destinations=LRocky+Mountain+Christian+Church,+Frederick+Campus,+Majestic+Street,+Frederick\
,+CO&departure_time=now&traffic_model=best_guess&key=AIzaSyAUl3FNewt5gQKEzuOjBU6CTqCMcfmpuM4"

def send(code):
    ser.write(struct.pack('>B', code))

while True:
    message = ser.readline().strip()
    if message == "lt":
        response = urlopen(lcurl)
        data = json.loads(response.read())
        send(data["rows"][0]["elements"][0]["duration_in_traffic"]["value"] / 60 + 1)

    if message == "at":
        response = urlopen(apexurl)
        data = json.loads(response.read())
        send(data["rows"][0]["elements"][0]["duration_in_traffic"]["value"] / 60 + 1)

    if message == "r":
        send(1)
    ser.flushInput()
    sleep(0.1)




