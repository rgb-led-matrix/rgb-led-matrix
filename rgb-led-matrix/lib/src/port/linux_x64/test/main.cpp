#include <RGBMatrix.h>
#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <IO/Root/FT4232/FT4232.h>
#include <Panel/MultiPanel.h>
#include <CFG/CFG.h>
using namespace rgb_matrix;

int main(int argc, char **argv) {
    GAMMA gamma(2.2, 2.2, 2.2);
    FT4232 *root = new FT4232("000");
    Node *node = root->GetNode(0);
    RP2040_UART *protocol = new RP2040_UART(node);
    CFG *cfg = new CFG(16, 32, rgb_matrix::Data_Format_ID::RGB48_ID, gamma);
    Single_Panel *panel = RGBMatrix::CreatePanel(cfg);
    MultiPanel *frame[2] = { new MultiPanel(32, 16), new MultiPanel(32, 16) };

    cfg->get_dot().set(0, 10, 255, 128, 0, 0.5, 1.0, 0.9);

    panel->set_brightness(50);
    frame[0]->map_panel(0, 0, panel, protocol);

    uint8_t x, y;
    while (1) {
        frame[0]->SetPixel(x, y, 255, 255, 255);
        frame[0]->show();
        x++;
        y++;
    }

    return 0;
}