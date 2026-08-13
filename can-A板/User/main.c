#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "ADC.h"
#include "MyCAN.h"
#include "MyUART.h"

int main(void)
{
	uint8_t rx_data[8];
	uint32_t rx_id;
	uint8_t rx_len;
    uint8_t tx_data[8];
    uint8_t speed;
    
    OLED_Init();
    MyADC_Init();
    MyCAN_Init();
    MyUART_Init(115200);
    
    OLED_ShowString(1, 1, "A: Sending");
    OLED_ShowString(2, 1, "Speed:");
    
    printf("=== A Board CAN Sender Start ===\r\n");
    
    while (1)
    {
		// ===== A板接收：处理开窗指令 =====
		if (MyCAN_IsDataReceive())
		{
			MyCAN_Receive(&rx_id, rx_data, &rx_len);
			
			if (rx_id == 0x102)   // 开窗/关窗指令
			{
				if (rx_data[0] == 1) {
					OLED_ShowString(4, 1, "Window:Open  ");
					printf("A: Window OPEN\r\n");
				} else {
					OLED_ShowString(4, 1, "Window:Closed");
					printf("A: Window CLOSED\r\n");
				}
			}
		}
        uint16_t adc_val = ADC_GetValue();
        speed = (uint8_t)(adc_val * 100 / 4095);
        
        tx_data[0] = speed;
        MyCAN_Transmit(0x101, tx_data, 1);
        
        OLED_ShowNum(2, 7, speed, 3);
        printf("TX Speed: %d km/h\r\n", speed);
        
        Delay_ms(200);
    }
}
