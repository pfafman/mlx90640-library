
from ctypes import *

mlxLib = CDLL('libMLX90640.so')

MLX_I2C_ADDR=0x33

print(mlxLib.initAll)

print("init")
print(mlxLib.initAll())

print("resolution", mlxLib.getResolution(MLX_I2C_ADDR))
print("refresh rate:", mlxLib.getRefreshRate(MLX_I2C_ADDR))

ta = c_float()
image = (c_float*768)()
print(list(image))

mlxLib.getTemperatureImage.argtypes = [c_float, POINTER(c_float), POINTER(c_float)]

while (True):
  code = mlxLib.getTemperatureImage(1.0, byref(ta), image)
  print("Code %d Ta %.2f" % (code,ta.value))
  print(list(image))