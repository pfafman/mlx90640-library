
from ctypes import *

mlxLib = CDLL('libMLX90640.so')

MLX_I2C_ADDR=0x33

print(mlxLib.initAll)

print("init")
print(mlxLib.initAll())

print("resolution", mlxLib.getResolution(MLX_I2C_ADDR))
print("refresh rate:", mlxLib.getRefreshRate(MLX_I2C_ADDR))


image = (c_float*768)()
print(list(image))

mlxLib.getTemperatureImage.argtypes = [c_float, POINTER(c_float)]

while (True):
  code = mlxLib.getTemperatureImage(1.0, image)
  print("Code", code)
  print(list(image))