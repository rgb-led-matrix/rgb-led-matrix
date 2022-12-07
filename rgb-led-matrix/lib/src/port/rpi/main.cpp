#include "canvas.h"
#include "led-matrix.h"
#include "CFG/RP2040_Multiplexed_Dual_UART_CFG.h"
using namespace rgb_matrix;

int main(int argc, char **argv) {
    RP2040_Multiplexed_Dual_UART_CFG *cfg = new RP2040_Multiplexed_Dual_UART_CFG(32, 32);
    Options options(Canvas_ID::RP2040_Multiplexed_Dual_UART_ID, cfg);
    RGBMatrix *matrix = RGBMatrix::CreateFromOptions(options);
    Canvas *canvas0 = matrix->CreateCanvas();
    Canvas *canvas1 = matrix->CreateCanvas();

    while (1) {
        canvas0->Fill(255, 255, 255);
        matrix->show(canvas0);
        canvas1->Fill(0, 0, 0);
        matrix->show(canvas1);
    }

    return 0;
}