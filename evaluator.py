import keras 

import sounddevice as sd
import numpy as np
import scipy.io.wavfile as wav

import librosa

import matplotlib.pyplot as plt

fs=8000
duration = 5  # seconds
print("Recording")
myrecording = sd.rec(56480, samplerate=fs, channels=1,dtype='float32')
sd.wait()

print("Done")



d = librosa.feature.mfcc(myrecording[:,0], sr=8000, n_mfcc=60).reshape((1,60,111,1))

model = keras.models.load_model("./data/model.save")

pred = model.predict(d)

p = np.argmax(pred, axis=1)





f, (ax1, ax2) = plt.subplots(2,1)
ax1.plot(myrecording[:,0])
ax2.imshow(d.reshape(60,111)[2:,])
if p == 0:
    f.suptitle("Cry", fontsize=25)
    print("Cry")
else:
    f.suptitle("Other",fontsize=25)
    print("Other")

plt.show()