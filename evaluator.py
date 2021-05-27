import keras 

import sounddevice as sd
import numpy as np
import scipy.io.wavfile as wav

import librosa

import matplotlib.pyplot as plt

from data_utils import calculate_mfs


fs=16000
duration = 5  # seconds


def eval_raw(mfcc):

    print(mfcc.shape)
    model = keras.models.load_model("./data/model.save")

    pred = model.predict(mfcc.reshape((1, 128,52,1)))

    return np.argmax(pred, axis=1)



def evaluate(a):
    # d = librosa.feature.mfcc()[:,1:].reshape((1,60,128,1))

    d = calculate_mfs(a, sf=fs, n_mfs=64, n_ceps=52,n_fft=512,hop_length=256).reshape((1, 128,52,1))

    return eval_raw(d), d



if __name__ == "__main__":
    print("Recording")
    myrecording = sd.rec(512*64, samplerate=fs, channels=1,dtype='float32')
    sd.wait()

    print("Done")

    p, d= evaluate(myrecording[:,0])

    f, (ax1, ax2) = plt.subplots(2,1)
    ax1.plot(myrecording[:,0])
    ax2.imshow(d.reshape(128,52).T[2:,])
    if p == 0:
        f.suptitle("Cry", fontsize=25)
        print("Cry")
    else:
        f.suptitle("Other",fontsize=25)
        print("Other")

    plt.show()



