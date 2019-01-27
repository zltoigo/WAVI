import RPi.GPIO as GPIO
import time
from flask import Flask, render_template, url_for, redirect

# Set up flask
app = Flask(__name__)

# Set up IO pin out
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
    return redirect(url_for("getDistance"))

@app.route("/measure")
def getDistance():
    # send ultrasound ping
    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)

    pulse_start = time.time()
    while GPIO.input(ECHO) == 0:
        pulse_start = time.time()

    pulse_end = time.time(); 
    while GPIO.input(ECHO) == 1:
        pulse_end = time.time()

    # calculate distance
    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)
    print "Distance:", distance, "cm"

    #update web page
    templateData = {
       'distanceData': distance
    }
    return render_template('index.html', **templateData)

if __name__ == '__main__':
    app.run(debug=True, port=5000, host='0.0.0.0')
        
