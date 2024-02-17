#include <RGBMatrix.h>
#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <IO/Root/FT4232/FT4232.h>
#include <Panel/MultiPanel.h>
#include <CFG/CFG.h>
#include <Frame/Frame.h>
#include <Frame/Frame_Manager/Frame_Manager.h>
using namespace rgb_matrix;

int main(int argc, char **argv) {
    // Setup IO
    FT4232 *root = new FT4232("000");
    Node *node = root->GetNode(0);
    RP2040_UART *protocol = new RP2040_UART(node);

    // Setup config
    GAMMA gamma(2.2, 2.2, 2.2);
    CFG *cfg = new CFG(16, 32, rgb_matrix::Data_Format_ID::RGB48_ID, gamma);
    cfg->get_dot().set(0, 10, 255, 128, 0, 0.5, 1.0, 0.9);

    // Create panels
    Single_Panel *panel = RGBMatrix::Create_Single_Panel(cfg);
    MultiPanel *frame[2] = { RGBMatrix::Create_MultiPanel(32, 16), RGBMatrix::Create_MultiPanel(32, 16) };
    frame[0]->map_panel(0, 0, panel, protocol);

    // Create frame
    Frame *f = new Frame(frame[0]);
    Frame_Manager *manager = new Frame_Manager();
    manager->push_frame(f);

    // Draw
    uint8_t x, y;
    while (1) {
        if (f->isFree()) {
            f->SetPixel(x, y, 255, 255, 255);
            manager->push_frame(f);
            x++;
            y++;
        }
    }

    return 0;
}