#include "main.h"
#include "board_api.h"
#include "debug/utils.h"

#include "autotune/autotune.h"

int main(void)
{
  debug_dwt_enable();
  board_init();
  autotune_mode_init();
  autotune_mode_run();

  while (1)
  {
  }
}