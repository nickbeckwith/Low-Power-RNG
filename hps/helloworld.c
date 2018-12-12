
/*****************************************************
Getting Started Guide for Zedboard

This demo displays the status of the switches on the
LEDs and prints a message to the serial communication
when a button is pressed.

Terminal Settings:
   -Baud: 115200
   -Data bits: 8
   -Parity: no
   -Stop bits: 1

7/25/16: Created by JonP
 ****************************************************

#include <stdio.h>
#include "platform.h"
#include <xgpio.h>
#include "xparameters.h"
#include "sleep.h"

int main()
{
   XGpio input, output;
   int button_data = 0;
   int switch_data = 0;

   XGpio_Initialize(&input, XPAR_AXI_GPIO_0_DEVICE_ID);	//initialize input XGpio variable
   XGpio_Initialize(&output, XPAR_AXI_GPIO_1_DEVICE_ID);	//initialize output XGpio variable

   XGpio_SetDataDirection(&input, 1, 0xF);			//set first channel tristate buffer to input
   XGpio_SetDataDirection(&input, 2, 0xF);			//set second channel tristate buffer to input

   XGpio_SetDataDirection(&output, 1, 0x0);		//set first channel tristate buffer to output

   init_platform();

   while(1){
      switch_data = XGpio_DiscreteRead(&input, 2);	//get switch data

      XGpio_DiscreteWrite(&output, 1, switch_data);	//write switch data to the LEDs

      button_data = XGpio_DiscreteRead(&input, 1);	//get button data

      //print message dependent on whether one or more buttons are pressed
      if(button_data == 0b00000){} //do nothing

      else if(button_data == 0b00001)
         xil_printf("button 0 pressed\n\r");

      else if(button_data == 0b00010)
         xil_printf("button 1 pressed\n\r");

      else if(button_data == 0b00100)
         xil_printf("button 2 pressed\n\r");

      else if(button_data == 0b01000)
         xil_printf("button 3 pressed\n\r");

      else if(button_data == 0b10000)
              xil_printf("button 4 pressed\n\r");

      else
         xil_printf("multiple buttons pressed\n\r");

      usleep(200000);			//delay

   }
   cleanup_platform();
   return 0;
}
 */
///////////////////////////////////////////////////////
/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "platform.h"
#include "xadcps.h"
#include "xil_types.h"
#define XPAR_AXI_XADC_0_DEVICE_ID 0



//normal code

static XAdcPs XADCMonInst;
int seed;
int main()
{

	XAdcPs_Config *ConfigPtr;
	XAdcPs *XADCInstPtr = &XADCMonInst;

	//status of initialisation
	int Status_ADC;

	//temperature readings
	u32 TempRawData;
	float TempData;
	u32 bit;

	//Vcc Int readings
	u32 VccIntRawData;
	float VccIntData;

	//Vcc Aux readings
	u32 VccAuxRawData;
	float VccAuxData;

	//Vbram readings
	u32 VBramRawData;
	float VBramData;

	//VccPInt readings
	u32 VccPIntRawData;
	float VccPIntData;

	//VccPAux readings
	u32 VccPAuxRawData;
	float VccPAuxData;

	//Vddr readings
	u32 VDDRRawData;
	float VDDRData;

	init_platform();

	printf("Adam Edition MicroZed Using Vivado How To Printf \n\r");

	//XADC initilization

	ConfigPtr = XAdcPs_LookupConfig(XPAR_AXI_XADC_0_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	Status_ADC = XAdcPs_CfgInitialize(XADCInstPtr,ConfigPtr,ConfigPtr->BaseAddress);
	if(XST_SUCCESS != Status_ADC){
		print("ADC INIT FAILED\n\r");
		return XST_FAILURE;
	}

	//self test
	Status_ADC = XAdcPs_SelfTest(XADCInstPtr);
	if (Status_ADC != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//stop sequencer
	XAdcPs_SetSequencerMode(XADCInstPtr,XADCPS_SEQ_MODE_SINGCHAN);

	//disable alarms
	XAdcPs_SetAlarmEnables(XADCInstPtr, 0x0);

	//configure sequencer to just sample internal on chip parameters
	XAdcPs_SetSeqInputMode(XADCInstPtr, XADCPS_SEQ_MODE_SAFE);

	//configure the channel enables we want to monitor
	XAdcPs_SetSeqChEnables(XADCInstPtr,XADCPS_CH_TEMP|XADCPS_CH_VCCINT|XADCPS_CH_VCCAUX|XADCPS_CH_VBRAM|XADCPS_CH_VCCPINT|
			XADCPS_CH_VCCPAUX|XADCPS_CH_VCCPDRO);

    seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
	init_lfsrs(seed);
	int reseed_counter = 0;
	while(1){
		reseed_counter++;
		if (reseed_counter > 10000){
			seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP) + seed;
			init_lfsrs(seed);
			reseed_counter = 0;
		}
		usleep(100);
		/*
		int result1 = 0;
		int result2 = 0;
		for (int i = 0; i < 2; i++){

		TempRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
		TempData = XAdcPs_RawToTemperature(TempRawData);
		int loops =0;
		int found = 0;
			while (found == 0){
				int TempRawData2 = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
				//int TempData2 = XAdcPs_RawToTemperature(TempRawData);
				if (TempRawData2 == TempRawData){
					found = 1;
				}
				else {
					loops++;
				}
				//printf("firstloop");
			}

			int loops3 =0;
			int found3 = 0;
			while (found3 == 0){
				int TempRawData3 = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
				//int TempData3 = XAdcPs_RawToTemperature(TempRawData);
				if (TempRawData3 == TempRawData){
					found3 = 1;
				}
				else {
					loops3++;
				}
				//printf("secondloop");
			}
		if (loops > loops3){
			result1 = 1;
		}
		else {
			result1 = 0;
		}

		}*/
		//printf("Raw Temp %lu Real Temp %f \n\r", TempRawData, TempData);
		if ((TempRawData & 0x8 >> 2)){
		// printf("0");
		}
		else{
			//printf("1");
		}


		printf("%d",get_random() >> 15);
		 seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
		printf("%d",get_random() >> 15);
		 seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
		printf("%d",get_random() >> 15);
		 seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
		printf("%d",get_random() >> 15);
		 seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
		printf("%d",get_random() >> 15);
		 seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
		printf("%d",get_random() >> 15);
		 seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
		printf("%d",get_random() >> 15);
		 seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);
		printf("%d\n",get_random() >> 15);
		 seed = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_TEMP);

		VccIntRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_VCCINT);
		VccIntData = XAdcPs_RawToVoltage(VccIntRawData);
		//printf("Raw VccInt %lu Real VccInt %f \n\r", VccIntRawData, VccIntData);

		VccAuxRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_VCCAUX);
		VccAuxData = XAdcPs_RawToVoltage(VccAuxRawData);
		//printf("Raw VccAux %lu Real VccAux %f \n\r", VccAuxRawData, VccAuxData);

		//    VrefPRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_VREFP);
		//    VrefPData = XAdcPs_RawToVoltage(VrefPRawData);
		//    printf("Raw VRefP %lu Real VRefP %f \n\r", VrefPRawData, VrefPData);

		//    VrefNRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_VREFN);
		//    VrefNData = XAdcPs_RawToVoltage(VrefNRawData);
		//   printf("Raw VRefN %lu Real VRefN %f \n\r", VrefNRawData, VrefNData);

		VBramRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_VBRAM);
		VBramData = XAdcPs_RawToVoltage(VBramRawData);
		//printf("Raw VccBram %lu Real VccBram %f \n\r", VBramRawData, VBramData);

		VccPIntRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_VCCPINT);
		VccPIntData = XAdcPs_RawToVoltage(VccPIntRawData);
		//printf("Raw VccPInt %lu Real VccPInt %f \n\r", VccPIntRawData, VccPIntData);

		VccPAuxRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_VCCPAUX);
		VccPAuxData = XAdcPs_RawToVoltage(VccPAuxRawData);
		//printf("Raw VccPAux %lu Real VccPAux %f \n\r", VccPAuxRawData, VccPAuxData);

		VDDRRawData = XAdcPs_GetAdcData(XADCInstPtr, XADCPS_CH_VCCPDRO);
		VDDRData = XAdcPs_RawToVoltage(VDDRRawData);
		//printf("Raw VccDDR %lu Real VccDDR %f \n\r", VDDRRawData, VDDRData);
	}

	return 0;
}


/*LFSR code
 *
 */


#define POLY_MASK_32 0xB4BCD35C
#define POLY_MASK_31 0x7A5BC2E3

typedef unsigned int uint;

uint lfsr32, lfsr31;

int shift_lfsr(uint *lfsr, uint polynomial_mask){
	int feedback;
	//

	feedback = *lfsr & 1;
	*lfsr >>= 1;
	if (feedback == 1){
		*lfsr ^= polynomial_mask;
		*lfsr ^= seed;
	}
	return *lfsr;
}



void init_lfsrs(int seed1){
	lfsr32 = seed1; //seed
	lfsr31 = 0x23456789;
}

int get_random(void){
	shift_lfsr(&lfsr32, POLY_MASK_32);

	//return (shift_lfsr(&lfsr32, POLY_MASK_32) ^ shift_lfsr(&lfsr31, POLY_MASK_31)) & 0xffff;
	return (shift_lfsr(&lfsr32, POLY_MASK_32) ^ shift_lfsr(&lfsr31, POLY_MASK_31)) & 0xffff;
}
