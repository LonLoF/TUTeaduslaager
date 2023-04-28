"""Tükk mis tahab ehitamist:
1) Arduino kuulab analoogsisendit.
2) Kui sisendis on kolm mingile mustrile vastavat piiki, siis tõstetakse mingi
digiväljund olekusse ON. Ehk siis ehitada süsteem, mis avab õigesti koputamise
peale ukse ... võimaliku lisandusega, et kui koputatakse valesti, siis
lastakse koputaja õhku :-)"""

import pyaudio
import struct
import numpy as np
import matplotlib.pyplot as plt

CHUNK = 1024*4
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100

p = pyaudio.PyAudio()
stream = p.open(
    format=FORMAT,
    channels = CHANNELS,
    rate = RATE,
    input=True,
    #output=True,
    frames_per_buffer=CHUNK
    )

def get_input(stream):
    data = stream.read(CHUNK)
    return np.array(struct.unpack(str(2*CHUNK)+ 'b', data))[::2] + 127

def plot_data(stream):
    plt.ion()
    fig, ax = plt.subplots()
    x = np.arange(0,2 * CHUNK, 2)
    line, = ax.plot(x, np.random.rand(CHUNK))
    ax.set_ylim(0, 255)
    ax.set_xlim(0, CHUNK)
    while True:
        line.set_ydata(get_input(stream))
        fig.canvas.draw()
        fig.canvas.flush_events()

def analyze():
    pass
plot_data(stream)
