import serial, struct, json
from time import sleep
from urllib import urlopen
ser = serial.Serial('/dev/ttyUSB0', 9600)
ser.flushInput()
boulderurl = "https://maps.googleapis.com/maps/api/distancematrix/json?\
origins=Lafayette,+Colorado&destinations=Boulder,+Colorado&departure_time=now&\
traffic_model=best_guess&key=AIzaSyAUl3FNewt5gQKEzuOjBU6CTqCMcfmpuM4"

def send(code):
    print code
    ser.write(struct.pack('>B', code))

while True:
    message = ser.readline().strip()
    print(message)
    if message == "bd":
        print "lala"
        response = urlopen(boulderurl)
        data = json.loads(response.read())
        send(data["rows"][0]["elements"][0]["duration"]["value"] / 60 + 1)

    if message == "r":
        send(1)
    ser.flushInput()
    sleep(0.1)




