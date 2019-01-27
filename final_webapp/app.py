import RPi.GPIO as GPIO
import time
from flask import Flask, render_template, url_for, redirect

app = Flask(__name__)

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
    
TRIG = 19
ECHO = 16
BUZZ = 20

GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)
#GPIO.setup(BUZZ, GPIO.OUT)

GPIO.output(TRIG, False)
time.sleep(2)

@app.route("/")
def index():
   ##    ##GPIO.cleanup()
    return redirect(url_for("getDistance"))

@app.route("/measure")
def getDistance():
    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)
    pulse_start = time.time()
    while GPIO.input(ECHO) == 0:
        pulse_start = time.time()
        print("echo = 0")
    pulse_end = time.time()
    while GPIO.input(ECHO) == 1:
        pulse_end = time.time()
        print("echo 1")


    pulse_duration = pulse_end - pulse_start

    distance = pulse_duration * 17150
    distance = round(distance, 2)
    print "Distance:", distance, "cm"

##    Buzzer = GPIO.PWM(BUZZ, 1100)
##    Buzzer.start(50)
##    
##    if distance > 20 and distance < 70:
##        Buzzer.ChangeFrequency(1500)
##        time.sleep(0.5)
##        Buzzer.stop()
##        
##    elif distance > 70 and distance < 150:
##        Buzzer.ChangeFrequency(1300)
##        time.sleep(0.5)
##        Buzzer.stop()
##
##        
##    if distance > 150 and distance < 300:
##        Buzzer.ChangeFrequency(1000)
##        time.sleep(0.5)
##        Buzzer.stop()
##
##    time.sleep(0.5)
    
    templateData = {
       'distanceData': distance
    }
    return render_template('index.html', **templateData)

if __name__ == '__main__':
    app.run(debug=True, port=80, host='0.0.0.0')
        
