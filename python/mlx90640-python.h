
#ifndef _MLX90640_PYTHON_H_
#define _MLX90640_PYTHON_H_

#include <stdint.h>

extern int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data);
extern void MLX90640_I2CFreqSet(int freq);
extern int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data);

extern int setup(int fps);
extern void cleanup();
extern float * get_frame();


#endif