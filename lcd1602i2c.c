/* LCD1602 with I2C converter driver */

#include <lcd1602i2c.h>

/*Place yout I2C HAL struct here */
extern I2C_HandleTypeDef hi2c1;

uint8_t BACKLIGHT = 0;

HAL_StatusTypeDef LCD_SendSignal(uint8_t addr, uint8_t data, uint8_t flags){
	HAL_StatusTypeDef res;

	uint8_t eld=data & 0xF0;
	uint8_t young=(data<<4)&0xF0;

	uint8_t data_arr[4];

	data_arr[0]= eld|flags|BACKLIGHT|PIN_EN;
	data_arr[1]= eld|flags|BACKLIGHT;
	data_arr[2]= young|flags|BACKLIGHT|PIN_EN;
	data_arr[3]= young|flags|BACKLIGHT;

	res=HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_arr, sizeof(data_arr), HAL_MAX_DELAY);

	HAL_Delay(LCD_DELAY_MS);
	return res;

}
/* Controls backlight of display */
void LCD_Backlight(bool state){
		if (state){
			BACKLIGHT=(1 << 3);
		}
		else{
			BACKLIGHT=0;
		}
}

void LCD_SendCommand(uint8_t addr, uint8_t cmd){
	LCD_SendSignal(addr, cmd, 0);
}

void LCD_SendSymbol(uint8_t addr, uint8_t symbol){
	LCD_SendSignal(addr, symbol, PIN_RS);
}

/* Sends array of symbols using Send_Symbol func */
void LCD_SendString(uint8_t addr, char *str){
	while(*str){
		LCD_SendSymbol(addr, (uint8_t)(*str));
		str++;
	}
}
void LCD_Init(uint8_t lcd_addr) {
	LCD_Backlight(true);
    /* Configure LCD in 4-bit mode and 2 line format */
    LCD_SendCommand(lcd_addr, 0b00110000);
    /* Place cursor to home */
    LCD_SendCommand(lcd_addr, 0b00000010);
    /* Turns display on */
    LCD_SendCommand(lcd_addr, 0b00001100);
    /* Clears display  */
    LCD_SendCommand(lcd_addr, 0b00000001);
}

void LCD_ClearDisplay(uint8_t lcd_addr){
	LCD_SendCommand(lcd_addr, 0b00000001);
}

/* Display string on a screen */
void LCD_WriteString(char *str, uint8_t string, uint8_t column){
	uint8_t position=0;
	if((string>1)||(column>15)){
		LCD_SendCommand(LCD_ADDR, 0b10000000);
		LCD_SendString(LCD_ADDR, "Invalid Values");
		LCD_SendCommand(LCD_ADDR, 0b11000000);
		LCD_SendString(LCD_ADDR, "0<y<1 0<x<15");
	}
	else if (!string){
		position=position|0x80|column;
		LCD_SendCommand(LCD_ADDR, position);
		LCD_SendString(LCD_ADDR, str);
	}
	else{
		position=position|0xC0|column;
		LCD_SendCommand(LCD_ADDR, position);
		LCD_SendString(LCD_ADDR, str);
	}
}

