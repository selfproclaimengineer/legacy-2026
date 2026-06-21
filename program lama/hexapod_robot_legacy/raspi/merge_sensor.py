#!/usr/bin/python
import time
import RPi.GPIO as GPIO
# from time import sleep

infra_red = 5
led = 6
push_button = 15
i2c = [2,3]
srf = [17,27,22,10,9,11]
servo = [12,3]
# pwm = [0,0,0,0,0,0]

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(infra_red, GPIO.IN)
GPIO.setup(led, GPIO.OUT)
GPIO.setup(push_button, GPIO.IN, pull_up_down=GPIO.PUD_UP)
# for i in range(6):
#    print (srf[i])
   # GPIO.setup(servo[i], GPIO.OUT)
   # pwm[i] = GPIO.PWM(servo[i], 60)
# GPIO.setup(servo_check, GPIO.OUT)

def bacaIR() :
	return GPIO.input(infra_red)
	    
def bacaPB() :
	return GPIO.input(push_button)
	
def ledNyala() :
   GPIO.output(led, True)

def ledMati() :
   GPIO.output(led, False)
   
def bacaSrf(pin):
   GPIO.setup(srf[pin], GPIO.OUT)
   GPIO.output(srf[pin], 0) 
   time.sleep(0.000002)
   #send trigger signal
   GPIO.output(srf[pin], 1)
   time.sleep(0.000005)
   GPIO.output(srf[pin], 0)
   GPIO.setup(srf[pin], GPIO.IN)
   while GPIO.input(srf[pin])==0:
      starttime=time.time()
   while GPIO.input(srf[pin])==1:
      endtime=time.time()
   duration=endtime-starttime
   # Distance is defined as time/2 (there and back) * speed of sound 34000 cm/s 
   distance=duration*34000/2
   return distance

'''
pwm.start(0)
# ----------------------------------

def setAngle(pin, angle):
    duty = angle / 18 + 2
    GPIO.output(servo[pin], True)
    pwm.ChangeDutyCycle(duty)
    sleep(1)
    GPIO.output(servo[pin], False)
    pwm.ChangeDutyCycle(duty)

# def kepencet()

#GPIO.add_event_detect(button_pin, GPIO.FALLING, callback=button_callback, bouncetime=300)      
   '''
#try: 
while True:
    if (bacaPB() == False): #object is far away
        ledNyala()
    elif (bacaPB() == True): #object is close
        ledMati()
    jarak = bacaSrf(0)
    # SRF[3] TIDAK BISA 
    print (jarak," cm")
    time.sleep(.5)
#except KeyboardInterrupt:
#    print("\nExiting program")
#    GPIO.cleanup()
#    pi.stop()
   
   
