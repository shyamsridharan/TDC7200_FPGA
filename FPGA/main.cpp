#include<xparameters.h>
#include "TDC7200.h"
#define PIN_TDC7200_INT       (2)
#define PIN_TDC7200_ENABLE    (4)
#define PIN_TDC7200_STOP      (5)
#define PIN_TDC7200_START     (6)

#define PIN_TDC7200_SPI_CS    (10)
#define TDC7200_CLOCK_FREQ_HZ (8000000)



#define NUM_STOPS (1)


int main()
{
	int status;
	TDC7200 tof;
	xil_printf("Hello \n \r");

	xil_printf("error in begin the tof \n \r");
	status=tof.begin();
    xil_printf("end the tof \n \r");


	return 0;
}
