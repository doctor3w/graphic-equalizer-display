#include "led_matrix.h"

typedef struct GraphicEq {
  coord_t bars[7];
  MatrixState_t* mat;
} GraphicEq_t;


int main(){
  MatrixState_t matrix = {40, 40, 23};
  GraphicEq_t geq = {{0,0,0,0,0,0,0}, &matrix};

  graphicSetup(&geq);
  graphBarSet(&geq, 3, 40);

  beginMatrix(&matrix);
  setPixel(&matrix, 0, 10, {10,34,45});
  endMatrix(&matrix);
}
