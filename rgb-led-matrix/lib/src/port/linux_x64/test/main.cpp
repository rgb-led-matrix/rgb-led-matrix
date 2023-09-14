#include <RGBMatrix.h>
#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <IO/Root/FT4232/FT4232.h>
#include <IO/Scheduler/Scheduler.h>
#include <Panel/MultiPanel_Mapper.h>
#include <Panel/Panel_Pixel_Mapper.h>
#include <Panel/Panel.h>
#include <CFG/CFG.h>

int main(int argc, char **argv) {
    FT4232 *root = new FT4232("000");
    Node *node = root->GetNode(0);
    CFG *cfg = new CFG(16, 32, node, rgb_matrix::Data_Format_ID::RGB48_ID, rgb_matrix::Panel_Type::HUB75, 8);
    Panel *panel = RGBMatrix::CreatePanel(cfg);
    MultiPanel_Mapper *frame[2] = { new MultiPanel_Mapper(32, 16), new MultiPanel_Mapper(32, 16) };
    Scheduler *scheduler[2] = { new Scheduler(), new Scheduler() };
    RP2040_UART *protocol = new RP2040_UART();

    frame[0]->map_panel(0, 0, panel);
    scheduler[0]->add_node(node);
    node->set_protocol(protocol);

    return 0;
}