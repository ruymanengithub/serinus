# Copied from: http://www.penguintutor.com/news/electronics/rc-pico-ibus#google_vignette

#from pdb import set_trace as stop
import time
from ibus import IBus

ibus_in = IBus(1)


while True:
    res = ibus_in.read()
    #stop()
    # if signal then display immediately
    if (res[0] == 1):
        print ("Status {} CH 1 {} Ch 2 {} Ch 3 {} Ch 4 {} Ch 5 {} Ch 6 {}".format(
            res[0],    # Status
            IBus.normalize(res[1]),
            IBus.normalize(res[2]),
            IBus.normalize(res[3]),
            IBus.normalize(res[4]),
            IBus.normalize(res[5], type="dial"),
            IBus.normalize(res[6], type="dial")),
            end="")
        print (" - {}".format(time.ticks_ms()))
        time.sleep(0.25)
    else:
        print ("Status offline {}".format(res[0]))
        #time.sleep()