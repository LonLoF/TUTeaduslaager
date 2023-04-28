import matplotlib.pyplot as plt
import numpy as np
import wave
import sys
import helisalvestaja as vs
import keyboard

def plot(x, y):
    plt.figure(1)
    plt.title("signal1 Wave...")
    plt.plot(x, y)
    plt.show()

def find(time, signal1, level):
    times = [0]
    for i in range(len(signal1)):
        if signal1[i] > level:
            if (time[i]-times[-1]) >= 0.1:
                times.append(time[i])
    return times[1:]
def find_tdifs(times):
    difs = []
    for i in range(len(times)-1):
        difs.append(round(abs(times[i]-times[i+1])*10))
    return difs

def find_ratios(times):
    ratios = []
    for i in range(len(times)-1):
        ratios.append(times[i]/times[i+1])
    return ratios
            
def difs(l1, l2):
    difs = []
    for i in range(len(l1)):
        difs.append(abs(l1[i]- l2[i]))
    return difs

def approve(errors, level):
    elist = []
    for i in errors:
        if i < level:
            elist.append(True)
        else:
            elist.append(False)
    return (False in elist), elist

"""print(find_tdifs(find(Time, abs(signal1), 50)))
print(find_tdifs(find(Time2, abs(signal2), 50)))

print(find_ratios(find(Time, abs(signal1), 50)))
print(find_ratios(find(Time2, abs(signal2), 50)))

print()"""

def knocking(name, save = False):
    if save:
        vs.save(name)
    file = wave.open(name+".wav", "r")
    signal = file.readframes(-1)
    signal = np.frombuffer(signal, np.int16)
    fs = file.getframerate()
    Time = np.linspace(0, len(signal) / fs, num=len(signal))
    return Time, signal
    
if __name__ == "__main__":
    mcname = "muster1"
    cname = "output"
    mcode = knocking(mcname)
    code = knocking(cname)
    while True:  # making a loop
        try:  # used try so that if user pressed other than the given key error will not be shown
            if keyboard.is_pressed('q'):  # if key 'q' is pressed 
                print('Quiting!')
                break  # finishing the loop
            if keyboard.is_pressed('c'):  # if key 'q' is pressed 
                print('Prepear to reset knocking code')
                mcode = knocking(mcname, True)
            if keyboard.is_pressed('d'):  # if key 'q' is pressed 
                print('Debug info')
                print(find_ratios(find(mcode[0], abs(mcode[1]), 50)))
                print(find_ratios(find(code[0], abs(code[1]), 50)))
                print(approve(difs(find_ratios(find(mcode[0], abs(mcode[1]), 50)),find_ratios(find(code[0], abs(code[1]), 50))), 0.1))
            if keyboard.is_pressed('v'):  # if key 'q' is pressed 
                print('Knock the code')
                code = knocking(cname, True)
                if not approve(difs(find_ratios(find(mcode[0], abs(mcode[1]), 50)),find_ratios(find(code[0], abs(code[1]), 50))), 0.1)[0]:
                    print("Approved")
                else:
                    print("Kaboom")
                
        except:
            pass  # if user pressed a key other than the given key the loop will break
