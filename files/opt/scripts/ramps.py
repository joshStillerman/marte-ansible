#!/usr/bin/python3
import time
import numpy as np
import MDSplus
import json
import threading
class MyThread(threading.Thread):
    def __init__(self, event):
        self.data = [0.] * 300
        self.data += (np.linspace(0,1, num=300).tolist())
        self.data += ([1.] * 300)
        self.data += (np.linspace(1,0, num=300).tolist())
        self.data += ([0.] * 300)
        self.index = 0
        self.start_time = time.time()
        threading.Thread.__init__(self)
        self.stopped = event

    def run(self):
        while not self.stopped.wait(0.01):
            if (self.index == len(self.data)-1) :
                self.stopped.set()
            MDSplus.Event.stream(1, 'LIFT_COIL_DEMAND', MDSplus.Int64((time.time())*1000), MDSplus.Float64(self.data[self.index]))
#            data = json.dumps({u'absolute_time': 0, 
#                    u'shot': 1, 
#                    u'name': u'LIFT_COIL_DEMAND', 
#                    u'timestamp': int(time.time()), 
#                    u'times': [(time.time()-self.start_time)*1000], 
#                    u'seq_number': self.index, 
#                    u'samples': [self.data[self.index]]})
#            print(data.__class__)
#            print(np.array(data))
#            MDSplus.Event.setevent('LIFT_COIL_DEMAND',np.array(data))
            self.index += 1


stopFlag = threading.Event()
thread = MyThread(stopFlag)
thread.start()
