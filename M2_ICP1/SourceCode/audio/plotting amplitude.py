from scipy.io.wavfile import read
import matplotlib.pyplot as plt
from os import walk
import os
if not os.path.exists("dogPlots"):
    os.makedirs("dogPlots")
car_wavs = []
for (_,_,filenames) in walk('dog'):
    car_wavs.extend(filenames)
    break
for car_wav in car_wavs:
    # read audio samples
    input_data = read("dog/" + car_wav)
    audio = input_data[1]
    # plot the first 1024 samples
    plt.plot(audio)
    # label the axes
    plt.ylabel("Amplitude")
    plt.xlabel("Time")
    # set the title
    # plt.title("Sample Wav")
    # display the plot
    plt.savefig("dogPlots/" + car_wav.split('.')[0] + '.png')
    # plt.show()
    plt.close('all')


if not os.path.exists("catPlots"):
    os.makedirs("catPlots")
car_wavs = []
for (_,_,filenames) in walk('cat'):
    car_wavs.extend(filenames)
    break
for car_wav in car_wavs:
    # read audio samples
    input_data = read("cat/" + car_wav)
    audio = input_data[1]
    # plot the first 1024 samples
    plt.plot(audio)
    # label the axes
    plt.ylabel("Amplitude")
    plt.xlabel("Time")
    # set the title
    # plt.title("Sample Wav")
    # display the plot
    plt.savefig("catPlots/" + car_wav.split('.')[0] + '.png')
    # plt.show()
    plt.close('all')
