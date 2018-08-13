#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include "headers/MLX90640_API.h"
#include "bcm2835.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_NONE    "\x1b[30m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//#define FMT_STRING "%+06.2f "
#define FMT_STRING "\u2588\u2588"

#define MLX_I2C_ADDR 0x33

void pulse(){
	bcm2835_gpio_write(RPI_BPLUS_GPIO_J8_07, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	bcm2835_gpio_write(RPI_BPLUS_GPIO_J8_07, 0);
}

int main(){
    int state = 0;
    printf("Starting...\n");
    static uint16_t eeMLX90640[832];
    float emissivity = 1;
    uint16_t frame[834];
    static float image[768];
    float eTa;
    static uint16_t data[768*sizeof(float)];

    std::fstream fs;

    bcm2835_init();
    bcm2835_gpio_fsel(RPI_BPLUS_GPIO_J8_07, BCM2835_GPIO_FSEL_OUTP);

    MLX90640_SetDeviceMode(MLX_I2C_ADDR, 0);
    MLX90640_SetSubPageRepeat(MLX_I2C_ADDR, 0);
    MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b110);
    MLX90640_SetChessMode(MLX_I2C_ADDR);
    //MLX90640_SetInterleavedMode(MLX_I2C_ADDR);
    //MLX90640_SetSubPage(MLX_I2C_ADDR, 0);
    printf("Configured...\n");

    paramsMLX90640 mlx90640;
    MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640);
    MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
    printf("kVdd %d\n", mlx90640.kVdd);
    int refresh = MLX90640_GetRefreshRate(MLX_I2C_ADDR);
    printf("Refresh rate %d\n", refresh);
    int resolution = MLX90640_GetCurResolution(MLX_I2C_ADDR);
    printf("Resolution %d\n", resolution);
    printf("EE Dumped...\n");

    int frames = 30;
    int subpage;
    static float mlx90640To[768];
    while (1){
    	//bcm2835_gpio_write(RPI_BPLUS_GPIO_J8_07, state);
    	state = !state;
    	printf("State: %d \n", state);
    	pulse();
        MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
	    pulse();
        eTa = MLX90640_GetTa(frame, &mlx90640);
        subpage = MLX90640_GetSubPageNumber(frame);
        MLX90640_CalculateTo(frame, &mlx90640, emissivity, eTa, mlx90640To);
        printf("Subpage: %d\n", subpage);
	    //MLX90640_SetSubPage(MLX_I2C_ADDR,!subpage);
        //printf("\n\n\n");
        for(int x = 0; x < 32; x++){
            for(int y = 0; y < 24; y++){
                //std::cout << image[32 * y + x] << ",";
                float val = mlx90640To[32 * (23-y) + x];
                if(val > 99.99) val = 99.99;
                if(val > 32.0){
                    printf(ANSI_COLOR_MAGENTA FMT_STRING ANSI_COLOR_RESET, val);
                }
                else if(val > 29.0){
                    printf(ANSI_COLOR_RED FMT_STRING ANSI_COLOR_RESET, val);
                }
                else if (val > 26.0){
                    printf(ANSI_COLOR_YELLOW FMT_STRING ANSI_COLOR_YELLOW, val);
                }
		        else if ( val > 20.0 ){
        		    printf(ANSI_COLOR_NONE FMT_STRING ANSI_COLOR_RESET, val);
        		}
        		else if (val > 17.0) {
                    printf(ANSI_COLOR_GREEN FMT_STRING ANSI_COLOR_RESET, val);
        		}
        		else if (val > 10.0) {
                    printf(ANSI_COLOR_CYAN FMT_STRING ANSI_COLOR_RESET, val);
        		}
        		else {
                    printf(ANSI_COLOR_BLUE FMT_STRING ANSI_COLOR_RESET, val);
                }
            }
            std::cout << std::endl;
        }
	      //std::this_thread::sleep_for(std::chrono::milliseconds(20));
        printf("\x1b[33A");
    }
    printf("Close\n");
    bcm2835_close();
    return 0;
}
