#include "ti_msp_dl_config.h"
#include "oled.h"

int main(void)
{
    SYSCFG_DL_init();
    OLED_Init();
    OLED_ShowString(8,2,(uint8_t *)"ZHONGJINGYUAN",16);
    while (1) {
    }
}
