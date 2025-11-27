#include "common_inc.h"
#include "board.hpp"

using namespace board;

EMPP_RAM_ITCM void Main()
{
    Com1::enable_irq_tx();

    while (true) {
    }
}