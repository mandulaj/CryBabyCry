import tensorflow as tf 


from tensorflow.compat.v1 import ConfigProto
from tensorflow.compat.v1 import InteractiveSession

config = ConfigProto()
config.gpu_options.allow_growth = True
session = InteractiveSession(config=config)

import keras 

from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D, Dense, Dropout, Input, Flatten, SeparableConv1D, ReLU, Bidirectional, \
    LSTM, Activation, GlobalAveragePooling2D
from keras.layers import GlobalMaxPooling1D
from keras.layers import BatchNormalization
from keras.layers.merge import Concatenate
from keras.models import Model
from keras import regularizers


def NN_model():

    l2 = 0.001

    input1= Input(shape=(60,111,1),name='input')

    x = Conv2D(filters=64,
               kernel_size=(3,3),
               padding='same',
               strides=(2,2),
               kernel_initializer='he_normal',
               kernel_regularizer=keras.regularizers.l2(l2))(input1)
    x = BatchNormalization()(x)
    x = Activation('relu')(x)


    x = Conv2D(filters=64,
               kernel_size=(3,3),
               padding='same',
               strides=(2,2),
               kernel_initializer='he_normal',
               kernel_regularizer=keras.regularizers.l2(l2))(x)
    x = BatchNormalization()(x)
    x = Activation('relu')(x)

    x = MaxPooling2D(pool_size=(1,5), padding='same')(x)

    x = Conv2D(filters=64,
               kernel_size=(3,3),
               padding='same',
               strides=(2,2),
               kernel_initializer='he_normal',
               kernel_regularizer=keras.regularizers.l2(l2))(x)
    x = BatchNormalization()(x)
    x = Activation('relu')(x)

    x = Conv2D(filters=64,
               kernel_size=(3,3),
               padding='same',
               strides=(2,2),
               kernel_initializer='he_normal',
               kernel_regularizer=keras.regularizers.l2(l2))(x)
    x = BatchNormalization()(x)
    x = Activation('relu')(x)



    # x = MaxPooling2D(pool_size=(1,2), padding='same')(x)

    x = GlobalAveragePooling2D()(x)

    x = Flatten()(x)

    x = Dense(10, kernel_regularizer=regularizers.l1(0))(x)
    x = Activation('relu')(x)

    x = Dense(2)(x)
    output = Activation('softmax')(x)


    model = Model(inputs=input1, outputs=output)
    model.compile(optimizer='adam', loss='categorical_crossentropy',
                  metrics=['accuracy'])

    return model