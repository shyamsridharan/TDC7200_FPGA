#include<xparameters.h>
#include "TDC7200.h"
#define PIN_TDC7200_INT       (2)
#define PIN_TDC7200_ENABLE    (4)
#define PIN_TDC7200_STOP      (5)
#define PIN_TDC7200_START     (6)

#define PIN_TDC7200_SPI_CS    (10)
#define TDC7200_CLOCK_FREQ_HZ (8000000)

static TDC7200 tof(PIN_TDC7200_ENABLE, PIN_TDC7200_SPI_CS, TDC7200_CLOCK_FREQ_HZ);

#define NUM_STOPS (1)


int main()
{
	xil_printf("Hello");
	int i=0;
    while (not tof.begin())
    {
        xil_printf("error in begin the tof \n \r");
        i=i+1;
        xil_printf("%d \n\r",i);

    }
    xil_printf("end the tof \n \r");
	return 0;
}
