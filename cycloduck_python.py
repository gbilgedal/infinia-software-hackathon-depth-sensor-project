import numpy as np
import matplotlib.pyplot as plt
import serial
from scipy.interpolate import interp2d
from scipy.ndimage import gaussian_filter

# Define serial communication port and baud rate
BAUD_RATE = 115200
COM_PORT = 'COM12'

# Define row and column values for sensitivity of the depth map
ROWS = 45
COLS = 45

# Augment row and column values for interpolation operation
ROWS_I = ROWS *4
COLS_I = ROWS*4

raw_data = np.full((ROWS, COLS), 400)

# bilinear interpolation implementation
def bilinear_interpolation(array, target_size):
    array = gaussian_filter(array, sigma=1)
    x = np.arange(array.shape[1])
    y = np.arange(array.shape[0])
    interp_func = interp2d(x, y, array, kind='linear')
    x_new = np.linspace(0, array.shape[1] - 1, target_size[1])
    y_new = np.linspace(0, array.shape[0] - 1, target_size[0])
    return interp_func(x_new, y_new)

array_interpolated = bilinear_interpolation(raw_data, (ROWS_I, COLS_I))
ser = serial.Serial(COM_PORT, BAUD_RATE)

# Create the plot
fig, (ax_raw, ax_enhanced) = plt.subplots(1, 2)
im_raw = ax_raw.imshow(raw_data, cmap='jet_r', vmin=0, vmax=400)
im_enhanced = ax_enhanced.imshow(array_interpolated, cmap='jet_r', vmin=0, vmax=400)
plt.colorbar(im_raw)

# Set axis labels
ax_raw.set_xlabel('Column')
ax_raw.set_ylabel('Row')

# Set title of the plot
ax_raw.set_title('Array Plot')

# Display the plot
plt.show(block=False)

import math

while True:
    data = ser.readline().split()
    try:
        if len(data) >= 6:
            print(data[0])
            if b'Distance:' in data:
                distance_index = data.index(b'Distance:')
                print("Data " , data)
                i_index = data[3]
                j_index = data[5]

                i = int(i_index)
                j = int(j_index)
                value = int(data[distance_index + 1])

                raw_data[i][j] = (value/10)

                print(f"Assigned value {value} to array[{i}][{j}]")

                array_interpolated = bilinear_interpolation(raw_data, (ROWS_I, COLS_I))

                # Update the plot with the new array values
                im_raw.set_array(raw_data)
                im_enhanced.set_array(array_interpolated)

                # Redraw the plot
                fig.canvas.draw()

                # Pause for a short interval to allow the plot to be updated
                plt.pause(0.001)

            else:
                print("Distance not found in data")
    except Exception as e:
        print("an error occured", e)
