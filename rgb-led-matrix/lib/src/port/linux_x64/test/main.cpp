#include <iostream>
#include <RGBMatrix.h>
#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <IO/Node/FTDI_UART/FTDI_UART.h>
#include <Mapper/Multiplex/Outdoor/Outdoor.h>
#include <Frame/Frame_Manager/Frame_Manager.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>
using namespace rgb_matrix;

int main(int argc, char **argv) {
    try {
        // Setup IO
        Node *node = new FTDI_UART("000", 0);
        RP2040_UART *protocol = new RP2040_UART(node);

        // Setup config
        GAMMA gamma(2.2, 2.2, 2.2);
        CFG *cfg = new CFG(16, 32, rgb_matrix::Data_Format_ID::RGB48_ID, gamma);
        cfg->get_dot().set(0, 10, 255, 128, 0, 0.5, 1.0, 0.9);

        // Create panels (Double Buffered)
        Single_Panel *panel[2] = { RGBMatrix::Create_Single_Panel(cfg), RGBMatrix::Create_Single_Panel(cfg) };
        Outdoor *od[2] = { new Outdoor(panel[0]), new Outdoor(panel[1]) };
        MultiPanel *frame[2] = { RGBMatrix::Create_MultiPanel(32, 16), RGBMatrix::Create_MultiPanel(32, 16) };
        frame[0]->map_panel(0, 0, MultiPanel::Direction::Right, od[0], protocol);
        frame[1]->map_panel(0, 0, MultiPanel::Direction::Right, od[1], protocol);

        // Create frame (Double Buffered)
        Frame *f[2] = { new Frame(frame[0]), new Frame(frame[1]) };
        Frame_Manager *manager = new Frame_Manager();

        // Draw (Double Buffered)
        uint16_t x = 0, y = 0, i = 0;
        while (1) {
            if (f[i]->isFree()) {
                f[i]->SetPixel(x, y, 255, 255, 255);
                manager->push_frame(f[i]);
                x = (x + 1) % 32;
                y = (y + 1) % 16;
                i = (i + 1) % 2;
            }
        }
    }
    catch (const Illegal &e) {
        std::cerr << std::string(e.what()) << std::endl;
    }
    catch (const Unknown_Type &e) {
        std::cerr << std::string(e.what()) << std::endl;
    }
    catch (const Null_Pointer &e) {
        std::cerr << std::string(e.what()) << std::endl;
    }
    catch (...) {
        // Something really bad happened
    }

    return 0;
}