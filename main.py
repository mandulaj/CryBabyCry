import numpy as np
import matplotlib.pyplot as plt 

from scipy.io import wavfile
from scipy.signal import resample

import csv
import os
import pickle
import tqdm

import librosa


from models import nn_tf

from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix

import matplotlib

DATA_FOLDER="./data"

SF=8000

SPLIT=0.8


def zero_pad(a, length):
    z = np.zeros(length)
    offset = len(z)//2 - len(a)//2
    if offset < 0:
        offset = 0
    z[offset:offset+len(a)] = a
    return z
 


def cached(cachefile):
    """
    A function that creates a decorator which will use "cachefile" for caching the results of the decorated function "fn".
    """
    def decorator(fn):  # define a decorator for a function "fn"
        def wrapped(*args, **kwargs):   # define a wrapper that will finally call "fn" with all arguments            
            # if cache exists -> load it and return its content
            if os.path.exists(cachefile):
                    with open(cachefile, 'rb') as cachehandle:
                        print("using cached result from '%s'" % cachefile)
                        return pickle.load(cachehandle)

            # execute the function with all arguments passed
            res = fn(*args, **kwargs)

            # write to cache file
            with open(cachefile, 'wb') as cachehandle:
                print("saving result to cache '%s'" % cachefile)
                pickle.dump(res, cachehandle)

            return res

        return wrapped

    return decorator   # return this "customized" decorator that uses "cachefile"



def get_files_from_cat(path):
    files = []
    for file in os.listdir(path):
        file_path = os.path.join(path, file)
        if os.path.isfile(file_path):
            files.append(file_path)
    return files


def get_audio_file_categories(path):
    categories ={}
    for dir in os.listdir(path):
        cat_path = os.path.join(path,dir)
        if os.path.isdir(cat_path):
            categories[dir] = get_files_from_cat(cat_path)

    return categories


def load_data(categories, new_rate=8000):
    cat_data = {}
    for cat, paths in categories.items():
        audio_data = []
        for path in tqdm.tqdm(paths):
            # raw = librosa.load(path, sr=8000)
            sampling_rate, data = wavfile.read(path)
            
            data = np.array(data, dtype=float)
            if sampling_rate !=new_rate:
                number_of_samples = round(len(data) * float(new_rate) / sampling_rate)
                data = resample(data, number_of_samples)
            data /= 2**16

            # padd all to same length
            data = zero_pad(data, 56480)
          
            
            audio_data.append(data)
        cat_data[cat] = audio_data
    return cat_data


def getESC50(path):
    categories = {}
    audio_path = os.path.join(path, "audio")
    with open(os.path.join(path, "meta/esc50.csv"), newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        next(reader) # skip first row
        for i,row in enumerate(reader):
            cat = row[3]
            name = os.path.join(audio_path, row[0])
            if cat not in categories:
                categories[cat] = [name]
            else:
                categories[cat].append(name)

    return categories




def plot_confusion_matrix_raw(cm, title="", path=None, fileName='confusion_matrix.png'):
    colormap = "viridis"

    plt.figure()
    plt.title(title)
    plt.imshow(cm, cmap=colormap)
    plt.xlabel("Predicted Label")
    plt.ylabel("True Label")
    plt.colorbar()
    plt.xticks(np.arange(len(cm)))
    plt.yticks(np.arange(len(cm)))

    cmap = matplotlib.cm.get_cmap(colormap)
    for x in range(len(cm)):
        for y in range(len(cm)):
            plt.text(x, y,  "{:.2f}".format(cm[y, x]), 
                horizontalalignment='center', 
                verticalalignment='center',
                fontsize=10,
                c=cmap(1-np.round(cm[y, x])))
    



@cached('./data/mfcc.pkl')
def get_mfcc():
    audios_esc50 = getESC50(os.path.join(DATA_FOLDER, "ESC-50"))

    audios_baby = get_audio_file_categories(os.path.join(DATA_FOLDER, "donateacry_corpus_cleaned_and_updated_data"))

    merged = {"baby_cry": [], "other": []}

    for k in audios_esc50:
        if k == "crying_baby":
            merged["baby_cry"] += audios_esc50[k]
        else:
            merged["other"] += audios_esc50[k]

    for k in audios_baby:
        merged["baby_cry"] += audios_baby[k]

    # print(merged['other'][1]) 
    # return
    data = load_data(merged, SF)

    for k in data:
        for i, s in tqdm.tqdm(enumerate(data[k])):
            data[k][i] = librosa.feature.mfcc(data[k][i], sr=SF, n_mfcc=60)


    return data
    

def main():
    data_raw = get_mfcc()

    data_inputs = []
    data_labels = []

    labels = {"baby_cry": np.array([1,0]), "other": np.array([0,1])}

    lim = 0

    for k in data_raw:
        for s in data_raw[k]:
            # if k == "other":
            #     if lim > 492:
            #         continue
            #     lim += 1
            data_inputs.append(s)
            data_labels.append(labels[k])

    data_inputs = np.array(data_inputs).reshape((-1,60,111,1))
    data_labels = np.array(data_labels)

    print(data_inputs.shape)
    print(data_labels.shape)

    train_inp,  valid_inp, train_lab, valid_lab = train_test_split(data_inputs, data_labels, train_size=SPLIT)
    


    model = nn_tf.NN_model()
    model.summary()

    print(train_inp.shape)
    print(len(valid_inp))

  

    for e in range(10):
        model.fit(x=train_inp, y=train_lab, validation_data=(valid_inp, valid_lab), epochs=1)

    model.save("./data/model.save")
    pred = model.predict(x=valid_inp)
    print(pred)
    y_pred = np.argmax(pred, axis=1)
    y_true = np.argmax(valid_lab, axis=1)

    cm = confusion_matrix(y_true, y_pred)
    plot_confusion_matrix_raw(cm)
    plt.show()
    pass
    # sample = data['belly_pain'][0]
    # plt.plot(sample)
    # plt.show()
    
    
    
    # return data



if __name__ == "__main__":
    main()