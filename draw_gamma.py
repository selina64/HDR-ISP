import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
import math

# Gamma values
gammalut = np.array([
    0, 0.3504950718773984, 0.48243595264750255, 0.57750428843709, 
    0.6596458942714417, 0.731034378464739, 0.7925580792857235, 
    0.8509817015104557, 0.9029435754464383, 0.9534255851019492, 1.0
])

# Corresponding x values
x = np.linspace(0, 1, len(gammalut))

plt.plot(gammalut)
plt.plot(np.power(x, 1/2.2))

plt.show()