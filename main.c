#include "graphic_equalizer.h"
#include "msgeq7.h"
//#include <fsl_debug_console.h>

void barTest(void) {
	GraphicEq_t state;
	initGraphicEq(&state);
	beginGraphicEq(&state);
	MatrixColor_t purple;
    createColor(127, 0, 127, &purple);
	setBarHeight(&state, 0, 5, &purple);
	while (1);
}

void pixelTest(void) {
	MatrixState_t state;
	initMatrixState(&state);
	beginMatrix(&state);
		MatrixColor_t purple;
    createColor(255, 0, 255, &purple);
	setPixel(&state, 0, 0, &purple);
	setPixel(&state, 15, 31, &purple);
	while(1);
}

void rectTest(void) {
	MatrixState_t state;
	initMatrixState(&state);
	beginMatrix(&state);
		MatrixColor_t purple;
	createColor(255, 0, 255, &purple);
	setRect(&state, 0, 10, 10, 20, &purple);
	createColor(127, 0, 127, &purple);
	setRect(&state, 0, 0, 10, 10, &purple);
	while(1);
}

void shadingTest(void) {
	MatrixState_t state;
	initMatrixState(&state);
	beginMatrix(&state);
		MatrixColor_t purple;
	createColor(255, 0, 255, &purple);
	setRect(&state, 0, 0, 10, 8, &purple);
	createColor(191, 0, 191, &purple);
	setRect(&state, 0, 7, 10, 15, &purple);
	createColor(127, 0, 127, &purple);
	setRect(&state, 0, 15, 10, 23, &purple);
	createColor(63, 0, 63, &purple);
	setRect(&state, 0, 23, 10, 31, &purple);
	while(1);
}

void nothingTest(void) {
	MatrixState_t state;
	initMatrixState(&state);
	beginMatrix(&state);
	while(1);
}

void audioTest(void) {
	MSGEQ7_t chipState;
	initMSGEQ7(&chipState);
	beginMSGEQ7(&chipState);
	while(1);
}

void bigBarTest(void) {
	GraphicEq_t state;
	initGraphicEq(&state);
	beginGraphicEq(&state);
	MatrixColor_t purple;
    createColor(255, 0, 255, &purple);
	setBarHeight(&state, 0, 5, &purple);
	createColor(255, 0, 127, &purple);
	setBarHeight(&state, 1, 10, &purple);
	createColor(0, 0, 255, &purple);
	setBarHeight(&state, 2, 7, &purple);
	createColor(255, 0, 0, &purple);
	setBarHeight(&state, 3, 15, &purple);
	createColor(0, 255, 255, &purple);
	setBarHeight(&state, 4, 9, &purple);
	createColor(0, 255, 0, &purple);
	setBarHeight(&state, 5, 3, &purple);
	createColor(0, 127, 0, &purple);
	setBarHeight(&state, 6, 6, &purple);
	while (1);
}

void bigBarChangeTest(void) {
	GraphicEq_t state;
	initGraphicEq(&state);
	beginGraphicEq(&state);
	MatrixColor_t purple;
	int offset = 0;
	int dir = 1;
	while(1){
    createColor(255, 0, 255, &purple);
	setBarHeight(&state, 0, 5 + offset, &purple);
	createColor(255, 0, 127, &purple);
	setBarHeight(&state, 1, 10 + offset, &purple);
	createColor(0, 0, 255, &purple);
	setBarHeight(&state, 2, 7 + offset, &purple);
	createColor(255, 0, 0, &purple);
	setBarHeight(&state, 3, 15 + offset, &purple);
	createColor(0, 255, 255, &purple);
	setBarHeight(&state, 4, 9 + offset, &purple);
	createColor(0, 255, 0, &purple);
	setBarHeight(&state, 5, 3 + offset, &purple);
	createColor(0, 127, 0, &purple);
	setBarHeight(&state, 6, 6 + offset, &purple);
	for(int x=0;x<1000000;x++);
	offset += dir;
	if(offset == 10){
		dir = -1;
	}
	if(offset == 0){
		dir = 1;
	}
	}
}

void realMain(){
	MSGEQ7_t chipState;
	initMSGEQ7(&chipState);
	beginMSGEQ7(&chipState);
	
	GraphicEq_t state;
	initGraphicEq(&state);
	beginGraphicEq(&state);
	
	MatrixColor_t blue;
    createColor(0, 0, 255, &blue);
	
	while (1) {
		for (size_t i = 0; i < 7; i++) {
			uint16_t data = chipState.data[i];
			coord_t height = 15 * data / 0xFFF;
			//debug_printf("Index %d : %d", i, data);
			setBarHeight(&state, i, height, &blue);
		}
	}
}

int main(int count, char **args) {
   //bigBarChangeTest();
	realMain();
   return 0;
}
