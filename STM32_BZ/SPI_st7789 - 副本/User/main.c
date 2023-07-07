#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MySPI.h"

int main(void)
{
	 	    	 	  
	   initlcd();
	   while(1){
             fillScreen(0xf800);
             fillScreen(0xffff);
        }
}
