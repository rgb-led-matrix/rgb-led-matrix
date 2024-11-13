#include <iostream>
#include "RGBMatrix.h"
#include "IO/Protocol/RP2040_UART/Data_Protocol/RP2040_UART_Data.h"
#include "IO/Protocol/RP2040_UART/Control_Protocol/RP2040_UART_Control.h"
#include "IO/Node/FTDI_UART/FTDI_UART.h"
#include "Mapper/Outdoor/Outdoor.h"
#include "Frame/Frame_Manager/Frame_Manager.h"
#include "Exception/Null_Pointer.h"
#include "Exception/Illegal.h"
#include "Exception/Unknown_Type.h"
#include "CFG/HUB75.h"
using namespace rgb_matrix;
using namespace rgb_matrix::FTDI;
using namespace rgb_matrix::Mappers;
using namespace rgb_matrix::Protocol::RP2040_UART;

int main(int argc, char **argv) {
    try {
        // Setup IO
        Node *data_node = new FTDI_UART("000", 0);
        Node *control_node = new FTDI_UART("001", 0);
        RP2040_UART_Data *data_protocol = new RP2040_UART_Data(data_node, 0);
        Control_Protocol *control_protocol = new RP2040_UART_Control(control_node);

        // Setup config
        GAMMA gamma(2.2, 2.2, 2.2);
        Outdoor *od = new Outdoor();
        CFG *cfg = new HUB75(16, 32, rgb_matrix::Data_Format_ID::RGB48_ID, 4, gamma, od);
        cfg->get_dot().set(0, 10, 255, 128, 0, 0.5, 1.0, 0.9);

        // Create panels (Double Buffered)
        Panel *panel[2] = { RGBMatrix::Create_Panel(cfg), RGBMatrix::Create_Panel(cfg) };
        MultiPanel *frame[2] = { RGBMatrix::Create_MultiPanel(32, 16), RGBMatrix::Create_MultiPanel(32, 16) };
        frame[0]->map_panel(0, 0, MultiPanel::Direction::Right, panel[0], data_protocol);
        frame[1]->map_panel(0, 0, MultiPanel::Direction::Right, panel[1], data_protocol);

        // Create frame (Double Buffered)
        Frame *f[2] = { new Frame(frame[0], control_protocol), new Frame(frame[1], control_protocol) };
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