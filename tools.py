
import os
import numpy as np
import pickle
import matplotlib.pyplot as plt
import matplotlib



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



def plot_confusion_matrix_raw(cm, title="", path=None, fileName='confusion_matrix.png'):
    colormap = "viridis"

    sc = 1/np.sum(cm, axis=1)
    cm_norm = sc[None].T * cm

    plt.figure()
    plt.title(title)
    plt.imshow(cm_norm, cmap=colormap)
    plt.xlabel("Predicted Label")
    plt.ylabel("True Label")
    plt.colorbar()
    plt.xticks(np.arange(len(cm_norm)))
    plt.yticks(np.arange(len(cm_norm)))

    cmap = matplotlib.cm.get_cmap(colormap)
    for x in range(len(cm_norm)):
        for y in range(len(cm_norm)):
            plt.text(x, y,  "{:.2f}".format(cm_norm[y, x]), 
                horizontalalignment='center', 
                verticalalignment='center',
                fontsize=10,
                c=cmap(1-np.round(cm_norm[y, x])))
    