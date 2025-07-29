#include "main.h"
#include "board_setup.h"


int main(void)
{
  init_board();
  while (1)
  {
    toggle_and_wait_led(100);
  }

}
