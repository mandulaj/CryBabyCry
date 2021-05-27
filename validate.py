
import numpy as np
import tensorflow as tf
from tensorflow import keras
import pickle
import matplotlib.pyplot as plt

def parse_2d_buf(lines):
    splits = [s.split(" ")[:-1] for s in lines]
    data = np.zeros((len(lines), len(splits[0])))


    for i, s in enumerate(splits):
        for j, n in enumerate(s):
            try:
                data[i][j] = float(n)
            except:
                print(i, j)
    return data


def load_from_file(name):
    with open(name, "r") as f:
        s = f.readlines()
        return parse_2d_buf(s)


def predict_quantized(x):
    tflite_interpreter = tf.lite.Interpreter(model_path="MFCCmodel_q8.tflite")
    input_details = tflite_interpreter.get_input_details()[0]
    output_details = tflite_interpreter.get_output_details()[0]
    
    input_shape = input_details['shape']
    scale = input_details["quantization"][0]
    offset = input_details["quantization"][1]
    
    # print("Scale {} Offset {}".format(scale, offset))
    x = x.reshape(-1,input_shape[1],input_shape[2],input_shape[3]) / scale + offset
    x = np.clip(x, -128, 127)
    
    tflite_interpreter.allocate_tensors()
    tflite_interpreter.set_tensor(input_details['index'], x.astype(np.int8))
    tflite_interpreter.allocate_tensors()
    tflite_interpreter.invoke()
    tflite_model_predictions = tflite_interpreter.get_tensor(output_details['index'])
    return tflite_model_predictions


def plot_both(mcu_mfcc, dataset_mfcc, dataset_true):

    if np.argmax(dataset_true) == 0:
        label = "Cry"
    else:
        label = "Other"
    dataset_mfcc = dataset_mfcc.reshape(64,40)
    
    # indx_min = 12
    
    # indx_max = -(20 - indx_min)
    
    # mcu_mfcc = mcu_mfcc[:,indx_min:indx_max]

    
    print("DS min: {} max: {}".format(np.min(dataset_mfcc), np.max(dataset_mfcc)))
    print("My min: {} max: {}".format(np.min(mcu_mfcc), np.max(mcu_mfcc)))
    print(np.mean(dataset_mfcc), np.std(dataset_mfcc), np.mean(mcu_mfcc), np.std(mcu_mfcc))
    
    combined = np.array([mcu_mfcc, dataset_mfcc])
    _min, _max = np.min(combined), np.max(combined)

    
    
    pred_raw = predict_quantized(dataset_mfcc)
    if np.argmax(pred_raw) == 0:
        pred = "Cry"
    else:
        pred = "Other"
        
    pred_mcu_raw = predict_quantized(mcu_mfcc)
    if np.argmax(pred_mcu_raw) == 0:
        pred_mcu = "Cry"
    else:
        pred_mcu = "Other"
    
    fig, (ax1, ax2) = plt.subplots(1,2)
    ax1.set_title("Dataset True:{} Pred:{} - {}".format(label,pred, pred_raw))
    ax2.set_title("MCU Pred:{} - {}".format(pred_mcu, pred_mcu_raw))
    ax1.imshow(dataset_mfcc, vmin=_min, vmax=_max)
    ax2.imshow(mcu_mfcc, vmin=_min, vmax=_max)



def main():

    offset = 8
    scale = 0.0747678205370903


    mcu_mfcc = (load_from_file("mcu_mfcc.txt") - offset ) * scale
    with open("./cache/dataset.pkl", "rb") as f:
        train_x, valid_x, train_y, valid_y = pickle.load(f)


    for index in range(10):

        plot_both(mcu_mfcc, valid_x[index], valid_y[index])
    plt.show()



    
if __name__ == "__main__":
    main()