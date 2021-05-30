import numpy as np
import matplotlib.pyplot as plt 


import tensorflow as tf

from models import nn_tf
from data_utils import get_data, segment_data, get_mfcc_train, get_mfcc_valid, augment_audio_train, augment_audio_valid,augment_mfcc_train,augment_mfcc_valid

from tools import plot_confusion_matrix_raw, cached

from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix
from tensorflow import keras


import wandb
from wandb.keras import WandbCallback






wandb.init(project='cbc', entity='mandula')

config = wandb.config







DATA_FOLDER="./data"


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

CONFIG["sample_length"] = int(CONFIG["n_frames"]/2 * CONFIG["n_fft"])

SPLIT_TEST = 0.99

EPOCHS=20

def tflite_convert_model(model, train_inp, name, quantize=True, config=CONFIG):

    print("TFLite conversion")
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    
    if quantize:
        def representative_dataset():
            for t in train_inp:
                yield[ np.array(t.reshape(1,config["n_frames"], config["n_ceps"],1),dtype=np.float32)]

        converter.optimizations = [tf.lite.Optimize.DEFAULT]
        converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
        converter.inference_input_type = tf.int8  # or tf.uint8
        converter.inference_output_type = tf.int8  # or tf.uint8
        converter.representative_dataset = representative_dataset
    tflite_model = converter.convert()
    with open(name,"wb") as f:
        f.write(tflite_model)

def validate_tflite_model(name, valid_inp, valid_label):

    tflite_interpreter = tf.lite.Interpreter(model_path=name)
    tflite_interpreter.allocate_tensors()


    predictions = np.zeros((len(valid_inp),), dtype=int)
    input_details = tflite_interpreter.get_input_details()
    output_details = tflite_interpreter.get_output_details()

    print("== Input details ==")
    print("name:", input_details[0]['name'])
    print("shape:", input_details[0]['shape'])
    print("type:", input_details[0]['dtype'])

    print("\n== Output details ==")
    print("name:", output_details[0]['name'])
    print("shape:", output_details[0]['shape'])
    print("type:", output_details[0]['dtype'])

    predictions = np.zeros((len(valid_inp),), dtype=int)
    input_scale, input_zero_point = input_details[0]["quantization"]
    for i in range(len(valid_inp)):
        val_batch = np.array(valid_inp[i],dtype=np.float32)
        # print(np.min(val_batch), np.max(val_batch))

        val_batch = val_batch / input_scale + input_zero_point
        # print(np.min(val_batch), np.max(val_batch))

        val_batch = np.expand_dims(val_batch, axis=0).astype(input_details[0]["dtype"])
        tflite_interpreter.set_tensor(input_details[0]['index'], val_batch)
        tflite_interpreter.allocate_tensors()
        tflite_interpreter.invoke()

        tflite_model_predictions = tflite_interpreter.get_tensor(output_details[0]['index'])
        #print("Prediction results shape:", tflite_model_predictions.shape)
        output = tflite_interpreter.get_tensor(output_details[0]['index'])
        predictions[i] = output.argmax()

    s = 0
    for i in range(len(predictions)):
        if (predictions[i] == valid_label[i]):
            s += 1
    accuracy_score = s / len(predictions)


    return predictions, valid_label, accuracy_score

@cached("./cache/dataset.pkl")
def load_dataset(config=CONFIG):
    print("Loading Data")
    data = get_data(DATA_FOLDER,config["sf"])

    print("Segmenting audio")
    data = segment_data(data, config["sample_length"])
    
    print("Raw number of Cry: {}, Other: {}".format(len(data['baby_cry']), len(data['other'])))

    # Cap the number of samples
    if len(data['baby_cry']) <  len(data['other']):
        cap = len(data['other'])
        indexes = np.arange(len(data['other']))
        indexes = np.random.choice( indexes, size=len( data["baby_cry"] ), replace=False)
        data["other"] = np.array(data["other"])[indexes]
    else:
        cap = len(data['baby_cry'])
        indexes = np.arange(len(data['baby_cry']))
        indexes = np.random.choice( indexes, size=len( data["other"] ), replace=False)
        data["baby_cry"] = np.array(data["baby_cry"])[indexes]
        
    
    print("Balanced number of Cry: {}, Other: {}".format(len(data['baby_cry']), len(data['other'])))



    data_inputs = []
    data_labels = []

    labels = {"baby_cry": np.array([1,0]), "other": np.array([0,1])}

    print("Merge")
    for k in data:
        count = 0
        for s in data[k]:
            count+=1
            data_inputs.append(s)
            data_labels.append(labels[k])
        print("{} {}".format(count, k))


    data_labels = np.array(data_labels)
    data_inputs = np.array(data_inputs)
    print("Split")
    train_x,  valid_x, train_y, valid_y = train_test_split(data_inputs, data_labels, train_size=config["split"])
    print("   Train set {} Validation set: {}".format(len(train_x), len(valid_x)))

    print("Augmenting training data")
    train_x, train_y = augment_audio_train(train_x, train_y, config)
    print("Augmenting validation data")
    valid_x, valid_y = augment_audio_valid(valid_x, valid_y, config)

    print("   Train set {} Validation set: {}".format(len(train_x), len(valid_x)))


    print("Generating MFCC")
    train_x = get_mfcc_train(train_x, config)    
    valid_x = get_mfcc_valid(valid_x, config)
    
    print("Augment MFCC")
    train_x = augment_mfcc_train(train_x, train_y, config)
    valid_x = augment_mfcc_valid(valid_x, valid_y, config)


    train_x = train_x.reshape(-1, config["n_frames"], config["n_ceps"], 1)
    valid_x = valid_x.reshape(-1, config["n_frames"], config["n_ceps"], 1)



    return train_x, valid_x, train_y, valid_y




def main():
   
    print("Load Dataset:")
    train_x, valid_x, train_y, valid_y = load_dataset()

    valid_x,  test_x, valid_y, test_y = train_test_split(valid_x, valid_y, train_size=SPLIT_TEST)


    model = nn_tf.NN_model(CONFIG["n_frames"],CONFIG["n_ceps"])
    model.summary()

    print(train_x.shape)
    print(valid_x.shape)
    print(test_x.shape)

  
    print("Start Training")
    best_val_ac = 0
    best_val_e = 0
    for e in range(EPOCHS):
        print("Epoch", e)
        history = model.fit(x=train_x, y=train_y, validation_data=(valid_x, valid_y), epochs=1, callbacks=[WandbCallback()])
        val_ac = history.history['val_accuracy'][0]

        if val_ac > best_val_ac:
            best_val_ac = val_ac
            best_val_e = e
        
            print("Saving model with validation {} /tmp/{}_{}_model.save".format(best_val_ac, e, best_val_ac))
            model.save("/tmp/{}_{}_model.save".format(e, best_val_ac))


    model = keras.models.load_model("/tmp/{}_{}_model.save".format(best_val_e, best_val_ac))
    print("Saving model")
    model.save("./data/model.save")
    y_pred = model.predict(x=valid_x)

    y_pred = np.argmax(y_pred, axis=1)
    y_true = np.argmax(valid_y, axis=1)


    print("Save quantized model")
    tflite_convert_model(model, train_x, "MFCCmodel_full.tflite", quantize=False)
    tflite_convert_model(model, train_x, "MFCCmodel_q8.tflite", quantize=True)


    y_pred_q8, _, accuracy_score = validate_tflite_model("MFCCmodel_q8.tflite", valid_x, y_true)


    print("Accuracy of quantized to int8 model is {}%".format(accuracy_score*100))
    # //print("Compared to float32 accuracy of {}%".format(score[1]*100))
    # print("We have a change of {}%".format((accuracy_score-score[1])*100))




    cmf32 = confusion_matrix(y_true, y_pred)
    cmi8 = confusion_matrix(y_true, y_pred_q8)
    plot_confusion_matrix_raw(cmf32, title="float32_model")
    plot_confusion_matrix_raw(cmi8, title='i8_model')
    plt.show()




if __name__ == "__main__":
    main()