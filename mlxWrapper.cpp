

#include <stdint.h>

#include "headers/MLX90640_API.h"
#include "bcm2835.h"

/*

Notes:


*/


extern "C" {

  #define MLX_I2C_ADDR 0x33

  // Internals
  static int libInit = 0;
  static uint16_t eeMLX90640[832];
  //static uint16_t data[768*sizeof(float)];
  static paramsMLX90640 mlx90640;

  // Lower Lib call wrappers

  void bcmInit(void) {
    bcm2835_init();
    //bcm2835_i2c_begin();
    //bcm2835_i2c_set_baudrate(400000);
    //bcm2835_i2c_setSlaveAddress(0x33);
  }

  int setDeviceMode(uint8_t slaveAddr, uint8_t deviceMode) {
    MLX90640_SetDeviceMode(slaveAddr, deviceMode);
  }

  int dumpEE(unsigned char slaveAddr, unsigned short *eeData) {
    return MLX90640_DumpEE(slaveAddr, eeData);
  }

  int checkInterrupt(uint8_t slaveAddr) {
    return MLX90640_CheckInterrupt(slaveAddr);
  }

  void startMeasurement(uint8_t slaveAddr, uint8_t subPage) {
    MLX90640_StartMeasurement(slaveAddr, subPage);
  }

  int getData(uint8_t slaveAddr, uint16_t *frameData){
    return MLX90640_GetData(slaveAddr, frameData);
  }

  int getFrameData(uint8_t slaveAddr, uint16_t *frameData){
    return MLX90640_GetFrameData(slaveAddr, frameData);
  }

  int extractParameters(uint16_t *eeData, paramsMLX90640 *mlx90640){
    return MLX90640_ExtractParameters( eeData, mlx90640);
  }

  float getVdd(uint16_t *frameData, const paramsMLX90640 *params){
    return MLX90640_GetVdd(frameData, params);
  }

  float getTa(uint16_t *frameData, const paramsMLX90640 *params){
    return MLX90640_GetTa(frameData, params);
  }

  void getImage(uint16_t *frameData, const paramsMLX90640 *params, float *result){
    MLX90640_GetImage(frameData, params, result);
  }

  void calculateTo(uint16_t *frameData, const paramsMLX90640 *params, float emissivity, float tr, float *result){
    MLX90640_CalculateTo(frameData, params, emissivity, tr, result);
  }
  
  int setResolution(uint8_t slaveAddr, uint8_t resolution){
    return MLX90640_SetResolution(slaveAddr, resolution);
  }
  
  int getResolution(uint8_t slaveAddr){
    return MLX90640_GetCurResolution(slaveAddr);
  }
  
  int setRefreshRate(uint8_t slaveAddr, uint8_t refreshRate){
    return MLX90640_SetRefreshRate(slaveAddr, refreshRate);
  }
  
  int getRefreshRate(uint8_t slaveAddr){
    return MLX90640_GetRefreshRate(slaveAddr);
  }
  
  int setSubPage(uint8_t slaveAddr, uint8_t subPage){
    return MLX90640_SetSubPage(slaveAddr, subPage);
  
  }
  
  int setSubPageRepeat(uint8_t slaveAddr, uint8_t subPageRepeat){
    return MLX90640_SetSubPageRepeat(slaveAddr, subPageRepeat);
  }
  
  int getSubPageNumber(uint16_t *frameData){
    return MLX90640_GetSubPageNumber(frameData);
  }
  
  int getCurMode(uint8_t slaveAddr){
    return MLX90640_GetCurMode(slaveAddr);
  } 
  
  int setInterleavedMode(uint8_t slaveAddr){
    return MLX90640_SetInterleavedMode(slaveAddr);
  }
  
  int setChessMode(uint8_t slaveAddr){
    return MLX90640_SetChessMode(slaveAddr);
  }


  // Upper Calls to make things easier

  int initAll(void) {
    bcm2835_init();
    MLX90640_SetDeviceMode(MLX_I2C_ADDR, 0);
    MLX90640_SetSubPageRepeat(MLX_I2C_ADDR, 0);
    MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b110);
    MLX90640_SetResolution(MLX_I2C_ADDR, 0b011);
    MLX90640_SetChessMode(MLX_I2C_ADDR);

    MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640);
    MLX90640_ExtractParameters(eeMLX90640, &mlx90640);

    libInit = 1;

    return libInit;
  }

  int getTemperatureImage(float emissivity, float *image) {
    uint16_t frame[834];
    int code = MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
    float eTa = MLX90640_GetTa(frame, &mlx90640);
    
    int subpage = MLX90640_GetSubPageNumber(frame);
    
    MLX90640_CalculateTo(frame, &mlx90640, emissivity, eTa, image);
    
    MLX90640_SetSubPage(MLX_I2C_ADDR,!subpage);
    
    // int i;
    // for(i=0;i<768;i++)
    //   image[i] = frame[i];
    return code;
  }


}

