import serial
import numpy as np
import pickle
import tqdm

with open("./cache/augment_valid.pkl", "rb") as f:
    valid_x, valid_y = pickle.load(f)



# print(np.argmax(valid_y[0]).astype(np.int8).tobytes())

def label2bytes(label):
    return np.argmax(label).astype(np.int8).tobytes()

def np2bytes(array):
    return (array * 2**15).astype(np.int16).tobytes()


ser = serial.Serial("/dev/ttyUSB0", 1500000)


# print((valid_x[0, :512] * 2**15).astype(np.int16))
# print((valid_x[0, :512] * 2**15).astype(np.int16).tobytes())


N = len(valid_x)
N = 1000

print("Testing {} validation samples".format(N))
print("Start the MCU in Validation mode")
for i in tqdm.tqdm(range(N)):
    ser.readline()
    ser.write(label2bytes(valid_y[i]))

    segments = valid_x[i].reshape((-1,512))
    for j, s in enumerate(segments):
        ser.readline()
        # print("Writing:", np2bytes(s))
        ser.write(np2bytes(s))

print("Done")
# for i, v in enumerate(valid_x):
    # ser.write()
    # ser.write(b'\xfe\xdc')






ser.close()



