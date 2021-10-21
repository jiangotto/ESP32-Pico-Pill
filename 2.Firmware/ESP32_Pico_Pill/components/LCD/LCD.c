#include "LCD.h"

void lcd_spi_pre_transfer_callback(spi_transaction_t *t);

//定义spi设备句柄
spi_device_handle_t spi;
//设置spi总线引脚配置
spi_bus_config_t buscfg={
        .miso_io_num = PIN_NUM_MISO,                // MISO信号线
        .mosi_io_num = PIN_NUM_MOSI,                // MOSI信号线
        .sclk_io_num = PIN_NUM_CLK,                 // SCLK信号线
        .quadwp_io_num = -1,                        // WP信号线，专用于QSPI的D2
        .quadhd_io_num = -1,                        // HD信号线，专用于QSPI的D3
        .max_transfer_sz = 172*320*8,                    // 最大传输数据大小
};
//设置spi总线软件配置
spi_device_interface_config_t devcfg={
        .clock_speed_hz = SPI_MASTER_FREQ_40M,      // Clock out at 40 MHz,
        .mode = 0,                                  // SPI mode 0
        /*
        * The timing requirements to read the busy signal from the EEPROM cannot be easily emulated
        * by SPI transactions. We need to control CS pin by SW to check the busy signal manually.
        */
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 128,                            // 传输队列大小，决定了等待传输数据的数量
        .pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
        //.flags = SPI_DEVICE_TXBIT_LSBFIRST
};

void SPI_init()
{
    //Initialize the SPI bus
    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    //Initialize the RES
    gpio_pad_select_gpio(PIN_NUM_RES);
    gpio_set_direction(PIN_NUM_RES, GPIO_MODE_OUTPUT);
    //Initialize the DC
    gpio_pad_select_gpio(PIN_NUM_DC);
    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    //Initialize the BLK
    gpio_pad_select_gpio(PIN_NUM_BLK);
    gpio_set_direction(PIN_NUM_BLK, GPIO_MODE_OUTPUT);
}

void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc=(int)t->user;
    gpio_set_level(PIN_NUM_DC, dc);
}

esp_err_t SPI_write_reg(uint8_t data)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       

    t.length = 1*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer = &data;               //Data
    t.user=(void*)0;                //D/C needs to be set to 0
    ret = spi_device_polling_transmit(spi, &t);  //Transmit!

    return ret;
}

esp_err_t SPI_write_data8(uint8_t data)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction

    t.length = 1*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer = &data;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    ret = spi_device_polling_transmit(spi, &t);  //Transmit!

    return ret;
}

esp_err_t SPI_write_data16(uint16_t data)
{
    esp_err_t ret;
    uint16_t rev_data = (data>>8) + (data<<8);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       

    t.length = 2*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer = &rev_data;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    ret = spi_device_polling_transmit(spi, &t);  //Transmit!

    return ret;
}

void LCD_set_add(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    SPI_write_reg(0x2a);
    SPI_write_data16(x1+34);
    SPI_write_data16(x2+34);
    SPI_write_reg(0x2b);
    SPI_write_data16(y1);
    SPI_write_data16(y2);
    SPI_write_reg(0x2c);
}

void LCD_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    LCD_set_add(xsta, ysta, xend-1, yend-1);
	for(uint16_t i=ysta;i<yend;i++)
	{													   	 	
		for(uint16_t j=xsta;j<xend;j++)
		{
			SPI_write_data16(color);
		}
	} 	
}

void LCD_draw_point(uint16_t x,uint16_t y,uint16_t color)
{
    LCD_set_add(x, y, x, y);
    SPI_write_data16(color);
}


/*已优化显示部分的代码，可以快速刷新屏幕*/
void LCD_lvgl_cb(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint16_t *color, unsigned int size)
{
    LCD_set_add(x1, y1, x2, y2);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       

    t.length = size*2*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer = color;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    spi_device_polling_transmit(spi, &t);  //Transmit!
}

void LCD_init()
{
    SPI_init();
    gpio_set_level(PIN_NUM_RES, 0);
    vTaskDelay(30);
    gpio_set_level(PIN_NUM_RES, 1);
    vTaskDelay(100);
    gpio_set_level(PIN_NUM_BLK, 1);
    vTaskDelay(100);

    SPI_write_reg(0x11);
    SPI_write_reg(0x36);

    SPI_write_data8(0x00);

    SPI_write_reg(0x3A);
	SPI_write_data8(0x05);

	SPI_write_reg(0xB2);
	SPI_write_data8(0x0C);
	SPI_write_data8(0x0C);
	SPI_write_data8(0x00);
	SPI_write_data8(0x33);
	SPI_write_data8(0x33);

	SPI_write_reg(0xB7); 
	SPI_write_data8(0x35); 

	SPI_write_reg(0xBB);
	SPI_write_data8(0x35);

	SPI_write_reg(0xC0);
	SPI_write_data8(0x2C);

	SPI_write_reg(0xC2);
	SPI_write_data8(0x01);

	SPI_write_reg(0xC3);
	SPI_write_data8(0x13); 

	SPI_write_reg(0xC4);
	SPI_write_data8(0x20);

	SPI_write_reg(0xC6); 
	SPI_write_data8(0x0F); 

	SPI_write_reg(0xD0); 
	SPI_write_data8(0xA4);
	SPI_write_data8(0xA1);

	SPI_write_reg(0xD6); 
	SPI_write_data8(0xA1);

	SPI_write_reg(0xE0);
	SPI_write_data8(0xF0);
	SPI_write_data8(0x00);
	SPI_write_data8(0x04);
	SPI_write_data8(0x04);
	SPI_write_data8(0x04);
	SPI_write_data8(0x05);
	SPI_write_data8(0x29);
	SPI_write_data8(0x33);
	SPI_write_data8(0x3E);
	SPI_write_data8(0x38);
	SPI_write_data8(0x12);
	SPI_write_data8(0x12);
	SPI_write_data8(0x28);
	SPI_write_data8(0x30);

	SPI_write_reg(0xE1);
	SPI_write_data8(0xF0);
	SPI_write_data8(0x07);
	SPI_write_data8(0x0A);
	SPI_write_data8(0x0D);
	SPI_write_data8(0x0B);
	SPI_write_data8(0x07);
	SPI_write_data8(0x28);
	SPI_write_data8(0x33);
	SPI_write_data8(0x3E);
	SPI_write_data8(0x36);
	SPI_write_data8(0x14);
	SPI_write_data8(0x14);
	SPI_write_data8(0x29);
	SPI_write_data8(0x32);

	SPI_write_reg(0x21); 
  
    SPI_write_reg(0x11);
    vTaskDelay(120);	
	SPI_write_reg(0x29); 

    //LCD_fill(0, 0, LCD_W, LCD_H, WHITE);
}
