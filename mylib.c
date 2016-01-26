#include <stdlib.h>
#include "mylib.h"

u16* videoBuffer = (u16*) 0x6000000;

int boundsCheck(int *var, int upperBound, int lowerBound, int *delta, int size)
{
		if(*var < upperBound)
		{
			*var = upperBound;
			*delta = -*delta;
			return 1;
		}
		if(*var > lowerBound-size+1)
		{
			*var = lowerBound-size+1;
			*delta = -*delta;
		}
		return 0;

}

void setPixel(int row, int col, u16 color)
{
   videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, volatile u16 color) {
    int r;
    for(r=0; r<height; r++)
    {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET((row + r), col, 240)];
        DMA[3].cnt = width | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_ON;
        
    }
}

/* drawimage3
* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	// @todo implement :)
	for(int y = 0; y < height; y++) {
		DMA[3].src = image + (width * y);
		DMA[3].dst = videoBuffer + (OFFSET((r + y), c, 240));
		DMA[3].cnt = width | DMA_ON;	
	}
}

void fillScreen(vu16 color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON;	
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
