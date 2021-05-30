import keras 

import sounddevice as sd
import numpy as np
import scipy.io.wavfile as wav

import librosa
import librosa.display
import matplotlib.pyplot as plt

from data_utils import my_calc_mfs


fs=16000
duration = 5  # seconds

CONFIG={
    "n_filts": 64,
    "n_ceps_raw": 64,
    "n_ceps": 40,
    "fft_skip": 256,
    "n_fft": 512,
    "n_frames": 64,
    "sf":16000,
    "sample_length":0,
    "split":0.7,
    "audio_aug_os":0.7,
    "ceps_co_low": 10,
    "ceps_co_high": 14

}

def eval_raw(mfcc):

    # print(mfcc.shape)
    model = keras.models.load_model("./data/model.save")

    pred = model.predict(mfcc.reshape((1, CONFIG['n_frames'],CONFIG['n_ceps'],1)))

    return np.argmax(pred, axis=1)



def evaluate(a):
    # d = librosa.feature.mfcc()[:,1:].reshape((1,60,128,1))
    
    d = my_calc_mfs(a, CONFIG)

    return eval_raw(d[:,12:52]), d



if __name__ == "__main__":
    print("Recording")
    myrecording = sd.rec(512*32, samplerate=fs, channels=1,dtype='float32')
    sd.wait()

    t = np.arange(0, 512*32)/fs

    print("Done")

    p, d= evaluate(myrecording[:,0])
    
    mfs = librosa.feature.melspectrogram(myrecording[:,0], sr=CONFIG['sf'],n_mels=2*CONFIG["n_filts"], n_fft=512, hop_length=128)
    mfs_log = librosa.amplitude_to_db(mfs)

    print(mfs_log.shape)
    f, (ax1, ax2) = plt.subplots(2,1)
    ax1.plot(t, myrecording[:,0])
    # ax2.imshow(mfs_log, origin="lower", interpolation="none") # , extent=[ 0, np.max(t), 1, 8000])
    librosa.display.specshow(mfs_log, x_axis='time', y_axis='mel',sr=CONFIG['sf'], fmax=8000, ax=ax2,  hop_length=128, cmap='inferno')
    


    plt.figure()
    plt.imshow(mfs_log)
    # N_xlabels=6 
    # ax2.set_xticks(np.linspace(0, mfs_log.shape[1], N_xlabels))
    # ax2.set_xticklabels(["{:.2f}".format(i) for i in np.linspace(0, np.max(t),N_xlabels)] )

    # N_ylabels = 5
    # ax2.set_yticks(np.linspace(0, mfs_log.shape[0], N_ylabels))
    # ax2.set_yticklabels(np.logspace(0, np.max(t),N_xlabels))
    # ax2.set_yticks(np.arange(data.shape[0]))
    # ax2.set_yscale('log')
    if p == 0:
        f.suptitle("Cry", fontsize=25)
        print("Cry")
    else:
        f.suptitle("Other",fontsize=25)
        print("Other")

    plt.show()



