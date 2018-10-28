
import ctypes

libTest = ctypes.CDLL('libTest.so')


print(libTest.testFunc)

libTest.testFunc.argtypes = (ctypes.c_int, ctypes.POINTER(ctypes.c_int))


def doSum(numbers):
  global libTest
  num_numbers = len(numbers)
  array_type = ctypes.c_int * num_numbers
  result = libTest.testFunc(ctypes.c_int(num_numbers), array_type(*numbers))
  return int(result)


print("DoIt")

print(doSum([1,2,-3,4,-5,6]))