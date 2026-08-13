#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "ADC.h"
#include "MyCAN.h"
#include "MyUART.h"
#include "PWM.h"
#include "MyKey.h"

int main(void)
{
    uint8_t rx_data[8];
	uint32_t rx_id;
	uint8_t rx_len;
	uint8_t speed =0;
	uint8_t overspeed_flag = 0;
	uint8_t tx_data[8];
	static uint8_t window_state = 0;
	
    OLED_Init();
	PWM_Init();
    MyCAN_Init();
    MyUART_Init(115200);
    MyKey_Init();
	
	OLED_ShowString(1, 1, "B: Receiving");
    OLED_ShowString(2, 1, "Speed:");
    OLED_ShowString(3, 1, "Status:OK");
    OLED_ShowString(4, 1, "Window:Closed");
	
	printf("=== B Board CAN Receive Start ===\r\n");
	
    while (1)
    {
        if(MyCAN_IsDataReceive() == 1)
		{
			MyCAN_Receive(&rx_id, rx_data, &rx_len);
			
			if(rx_id == 0x101)
			{
				speed = rx_data[0];
				OLED_ShowNum(2,7,speed,3);
				printf("RX Speed: %d km/h\r\n",speed);
				
				if(speed > 80)
				{
					overspeed_flag = 1;
					PWM_SetDuty(100);
					OLED_ShowString(3,8,"OVERS ");
					printf("!!! OVERSPEED ALARM !!!\r\n");
					
					tx_data[0] = speed;
					MyCAN_Transmit(0x200,tx_data,1);
				}
				else
				{
					overspeed_flag = 0;
					PWM_SetDuty(0);
					OLED_ShowString(3,8,"OK    ");
				}
			}
		}
			if (Key_Get() == 1)
			{
				if (overspeed_flag == 0)
				{
					// 切换窗户状态
					window_state = !window_state;
					tx_data[0] = window_state; // 0=关，1=开
					MyCAN_Transmit(0x102, tx_data, 1);
					
					if (window_state) {
						OLED_ShowString(4, 8, "Open  ");
						printf("Window: OPEN\r\n");
					} else {
						OLED_ShowString(4, 8, "Closed");
						printf("Window: CLOSED\r\n");
					}
				}
				else
				{
					// 超速锁定，禁止操作
					OLED_ShowString(4, 8, "Locked");
					printf("Window: LOCKED (overspeed)\r\n");
				}
		}
		Delay_ms(50);
    }
}
