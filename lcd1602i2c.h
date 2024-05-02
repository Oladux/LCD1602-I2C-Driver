

#ifndef INC_LCD1602I2C_H_
#define INC_LCD1602I2C_H_

#include <main.h>
#include <stdbool.h>
HAL_StatusTypeDef LCD_SendSignal(uint8_t addr, uint8_t data, uint8_t flags);
void LCD_SendCommand(uint8_t addr, uint8_t cmd);
void LCD_SendSymbol(uint8_t addr, uint8_t symbol);
void LCD_SendString(uint8_t addr, char *str);
void LCD_Init(uint8_t lcd_addr);
void I2C_Scan();
void LCD_ClearDisplay(uint8_t lcd_addr);
void LCD_WriteString(char *str, uint8_t string, uint8_t column);

#endif /* INC_LCD1602I2C_H_ */
