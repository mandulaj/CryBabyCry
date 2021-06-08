
import os
import numpy as np
import librosa
import csv
import tqdm

from tools import cached, zero_pad

import gzip


from scipy.io import wavfile
from scipy.signal import resample
from scipy.fftpack import dct
from scipy import signal

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


def load_data(categories, new_rate):
    cat_data = {}
    for cat, paths in categories.items():
        audio_data = []
        for path in tqdm.tqdm(paths):
            if os.path.splitext(path)[1] == ".wav":
                sampling_rate, data = wavfile.read(path)
            elif os.path.splitext(path)[1] == ".gz":
                with gzip.open(path, "rb") as f:
                    sampling_rate, data = wavfile.read(f)
            else:
                print("Unknown file type {}".format(path))
                continue

            l = len(data)

            data = np.array(data, dtype=np.float32)
            if sampling_rate !=new_rate:
                number_of_samples = round(len(data) * float(new_rate) / sampling_rate)
                data = resample(data, number_of_samples)
            
            
            data = np.array(data, dtype=np.float32) / 2**15

            #print(data.shape, sampling_rate, l/sampling_rate, len(data)/new_rate, path)

            # padd all to same length
            # data = zero_pad(data, 114688)
            # Keep only one channel
            if len(data.shape) > 1 and data.shape[1] > 1:
                data = data[:,0]
          
            
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


def get_audio_file_yt_cry(path):
    return [os.path.join(path, "youtube/cry16k.wav")]


def get_audio_file_yt_laugh(path):
    return [os.path.join(path, "youtube/laugh16k.wav")]

def get_adv_samp(path):
    return [os.path.join(path, "adversarial_samples.wav")]

def get_files_from_audioset(path):
    files = []
    path = os.path.join(path, "raw/audioset/babycry")
    for file in os.listdir(path):
        file_path = os.path.join(path, file)
        if os.path.isfile(file_path):
            files.append(file_path)
    return files




@cached('./cache/raw_data.pkl')
def get_data(path, sf):
    audios_esc50 = getESC50(os.path.join(path, "ESC-50"))

    audios_baby = get_audio_file_categories(os.path.join(path, "donateacry_corpus_cleaned_and_updated_data"))

    # audios_baby2 = get_audio_file_yt(path)
    # print(audios_baby2)

    

    merged = {"baby_cry": [], "other": []}

    for k in audios_esc50:
        if k == "crying_baby":
            merged["baby_cry"] += audios_esc50[k]
        else:
            merged["other"] += audios_esc50[k]

    for k in audios_baby:
        # print(audios_baby[k])
        merged["baby_cry"] += audios_baby[k]


    merged["other"] += get_audio_file_yt_laugh(path)
    merged["baby_cry"] += get_audio_file_yt_cry(path)
    merged["other"] += get_adv_samp(path)

    merged["baby_cry"] += get_files_from_audioset(path)

    # merged["baby_cry"] += [audios_baby2]
    # print(merged['other'][1]) 
    return load_data(merged, sf)


@cached('./cache/mfcc_lib.pkl')
def get_mfcc_lib(data, sf, n_fft, hop_length, n_mfcc):
    for k in data:
        for i, s in tqdm.tqdm(enumerate(data[k])):
            data[k][i] = librosa.feature.mfcc(data[k][i], n_fft=n_fft, hop_length=hop_length, sr=sf, n_mfcc=n_mfcc)[:,1:]

    return data


BIQUAD_A = np.array([1, -1.88901808, 0.89485930])
BIQUAD_B = np.array([0.94596935, -1.89193869, 0.94596935])

bins = np.array([0, 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 19, 20, 22, 24, 26, 28, 30, 32, 34, 36, 39, 41, 44, 46, 49, 52, 55, 58, 61, 64, 68, 71, 75, 79, 83, 87, 91, 96, 101, 106, 111, 116, 121, 127, 133, 139, 146, 152, 159, 166, 174, 182, 190, 198, 207, 216, 225, 235, 245, 256])
ceps_mat = np.zeros((256,64))
for m in range(1, 64+1):
    f_m_plus = bins[m+1]
    f_m = bins[m]
    f_m_minus = bins[m-1]
    for k in range(f_m_minus, f_m):
        ceps_mat[k, m-1] = (k - bins[m-1])/(bins[m] - bins[m-1])
    for k in range(f_m, f_m_plus):
        ceps_mat[k, m-1] = (bins[m+1]-k)/(bins[m+1]-bins[m])


def my_calc_mfs(data, config, gain=1):
    zeros = np.zeros(int(config["n_fft"]/2))
    data = np.concatenate((zeros, data)) ## Prepend zeros for procesing

    # Filter data with 2 stage biqud
    filt_data = signal.lfilter(BIQUAD_B, BIQUAD_A, data)
    
    # Segment data into 128 512 sample frams in overlapping strides
    strides = np.lib.stride_tricks.as_strided(
        filt_data,
        (config['n_filts'],config["n_fft"]),
        (int(filt_data.strides[0]*config["n_fft"]/2), filt_data.strides[0]), writeable=False)
    # print("Filt:")
    # print(strides[:1])
    strides_h = np.multiply(strides, np.hamming(config["n_fft"]))
    # print("Hamming:")
    # print(strides_h[:1])
    strides_fft = np.abs(np.fft.rfft(strides_h, axis=1))[:,:-1] ## Discard upper coef
    # print("Abs FFT:")
    # print(strides_fft[:1])
    ceps = strides_fft @ ceps_mat
    # print("Ceps:")
    # print(ceps[:1])


    # print("Log Ceps:")
    # print(librosa.power_to_db(ceps)[:1])
    res = dct(librosa.power_to_db(ceps)*gain, norm='ortho', axis=1, type=2)
    # res[:,32] = 0 # MCU can not calculate that coefficient
    return res





def calculate_mfs(data, config, gain=1/20):
    mfs = librosa.feature.melspectrogram(data, sr=sf, n_fft=config["n_fft"], hop_length=config["fft_skip"],n_mels=config["n_filts"])[:,1:]
    
    res = dct(librosa.power_to_db(mfs)*gain, norm='ortho',axis=0, type=2)[:]
    # res = mfs[6:-6]

    return res[:].T  # Transpose and flip in order to match the MCU






def get_mfcc(data, config):
    result = np.empty((len(data), config["n_frames"], config["n_ceps_raw"]), dtype=np.float32)
    for i, d in tqdm.tqdm(enumerate(data)):
        result[i] = my_calc_mfs(d, config)

    return result

# @cached('./cache/mfcc_train.pkl')
def get_mfcc_train(data, config):
    return get_mfcc(data, config)

# @cached('./cache/mfcc_valid.pkl')
def get_mfcc_valid(data, config):
    return get_mfcc(data,config)



# Cut data into 
@cached('./cache/segment.pkl')
def segment_data(data, sample_length):
    new_data = {}
    for k in data:
        new_data[k] = []
        for i, s in tqdm.tqdm(enumerate(data[k])):
            raw = data[k][i]

            num_segments = int(np.ceil(len(raw) / sample_length))
            padded = zero_pad(raw, num_segments * sample_length)
            segments = np.split(padded, num_segments)

            for s in segments:
                if np.mean(np.abs(s)) > 0.03:
                    new_data[k].append(s)

    return new_data


def apply_transformation(data, transform):
    if transform == "noise":
        std = np.std(data)
        return np.random.randn(len(data)) * std / 1000.0 + data
    if transform == "volume":
        volume = np.random.gamma(1,3)
        if volume > 10:
            volume = 10
        return volume * data
    if transform == "shift":
        return np.roll(data, np.random.randint(len(data)))




def augment_audio(inputs, labels, os):
    OVERSAMPLE=int(len(inputs)*os)
    

    new_x = np.empty((OVERSAMPLE, len(inputs[0])), dtype=np.float32)
    new_y = np.empty((OVERSAMPLE, 2), dtype=np.float32)

    indexes = np.random.choice(np.arange(len(inputs)), OVERSAMPLE, replace=False)

    for i , indx in tqdm.tqdm(enumerate(indexes)):

        transf = np.random.choice(["shift", "noise", "volume"], 2, replace=False, p=[0.5,0.1,0.4])
            
        new_audio = apply_transformation(inputs[indx], transf[0])
        new_audio = apply_transformation(new_audio, transf[1])

        new_x[i] = new_audio
        new_y[i] = labels[indx]
            
    

    return np.concatenate((inputs,new_x)), np.concatenate((labels,new_y))

@cached('./cache/augment_train.pkl')
def augment_audio_train(inputs,labels, config):
    return augment_audio(inputs, labels, config["audio_aug_os"])


@cached('./cache/augment_valid.pkl')
def augment_audio_valid(inputs,labels, config):
    return augment_audio(inputs, labels, config["audio_aug_os"])







def transform_mfcc(mfcc, config):


    i_low = np.random.randint(config["ceps_co_low"], config["ceps_co_high"])
    i_high = config["n_ceps_raw"] - config["n_ceps"] - i_low
    if i_high == 0:
        i_high = None
    else:
        i_high = -i_high

    scale = np.random.gamma(2,0.7)
    if scale >3:
        scale = 3

    offset = np.random.randn()*0.7

    res = mfcc[:,i_low:i_high] * scale + offset

    mean = np.mean(res)
    std = np.std(res)
    res = (res - mean) / std

    return res


def augment_mfcc(inputs, labels, config):
    # samples=int(len(inputs) * config['mfcc_aug_os'])
    out = np.empty((len(inputs), config["n_frames"], config["n_ceps"]), dtype=np.float32)
    # indexes = np.random.choice(np.arange(len(inputs)), samples, replace=False)

    for i in tqdm.tqdm(range(len(inputs))):
        out[i] = transform_mfcc(inputs[i], config)
        

    return out



def augment_mfcc_train(inputs, labels, config):
    return augment_mfcc(inputs, labels, config)


def augment_mfcc_valid(inputs, labels, config):
    return augment_mfcc(inputs, labels, config)



