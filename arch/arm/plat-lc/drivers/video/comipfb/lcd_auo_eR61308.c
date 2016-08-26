#include "comipfb.h"
#include "comipfb_dev.h"
#include "mipi_cmd.h"
#include "mipi_interface.h"

static u8 backlight_cmds[][ROW_LINE] = {
//	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x04},
	{0x00, GEN_CMD, LW_PACK, 0x07, 0x05, 0x00, 0xBB, 0x7B, 0xFF, 0x0F, 0x02, 0x01, 0x00},
};

//[0]: delay after transfer; [1]:count of data; [2]: word count ls; [3]:word count ms; [4]...: data for transfering
static u8 lcd_cmds_init[][ROW_LINE] = {
/****Start Initial Sequence ***/
/*	{0xff, DCS_CMD, SW_PACK1, 0x01, 0x11},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x04}, 
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB3, 0x02},	
	{0x00, GEN_CMD, LW_PACK, 0x07, 0x05, 0x00, 0xB6, 0x53, 0x62, 0x85, 0x00},	//gen long
	{0x00, GEN_CMD, LW_PACK, 0x06, 0x04, 0x00, 0xB7, 0x00, 0x00, 0x01},	//gen long 
	{0x00, GEN_CMD, LW_PACK, 0x0B, 0x09, 0x00, 0xC0, 0x43, 0xFF, 0x04, 0x0B, 0x02, 0x07, 0x07, 0x40},
	{0x00, GEN_CMD, LW_PACK, 0x0A, 0x08, 0x00, 0xC1, 0x50, 0x02, 0x22, 0x00, 0x00, 0x61, 0x11},	
	{0x00, GEN_CMD, LW_PACK, 0x1B, 0x19, 0x00, 0xC3, 0x04, 0x00, 0x05, 0x14, 0x80, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x5C},	
	{0x00, GEN_CMD, LW_PACK, 0x0C, 0x0A, 0x00, 0xD0, 0x74, 0x29, 0xDD, 0x15, 0x09, 0x2B, 0x00, 0xC0, 0xCC}, //Power Setting
	{0x00, GEN_CMD, LW_PACK, 0x10, 0x0E, 0x00, 0xD1, 0x4D, 0x24, 0x34, 0x55, 0x55, 0x77, 0x77, 0x66, 0x31, 0x75, 0x42, 0x86, 0x06},	
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xD6, 0xA8},	//DSI Setting
	{0x00, GEN_CMD, LW_PACK, 0x05, 0x03, 0x00, 0xC8, 0x1C, 0x23, 0x27, 0x2B, 0x30, 0x34, 0x16, 0x15, 0x12, 0x0E, 0x0B, 0x06, 0x1C, 0x23, 0x27, 0x2B, 0x30, 0x34, 0x16, 0x15, 0x12, 0x0E, 0x0B, 0x06},
	{0x00, GEN_CMD, LW_PACK, 0x05, 0x03, 0x00, 0xD5, 0x18, 0x18},	
	{0x05, GEN_CMD, LW_PACK, 0x07, 0x05, 0x00, 0xBB, 0x0B, 0x00, 0x00, 0x02, 0x00, 0x00},*/
	{0xff, DCS_CMD, SW_PACK1, 0x01, 0x11},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x04},
	{0x00, GEN_CMD, LW_PACK, 0x07, 0x05, 0x00, 0xBB, 0x7B, 0xFF, 0x0F, 0x02, 0x01, 0x00},
	{0xff, DCS_CMD, SW_PACK1, 0x01, 0x29},		//dcs short no param
};

static u8 lcd_cmds_suspend[][ROW_LINE] = {
	{0x64, DCS_CMD, SW_PACK1, 0x01, 0x28},
	{0xff, DCS_CMD, SW_PACK1, 0x01, 0x10},	//TODO delay is 300ms.
	{0x10, GEN_CMD, SW_PACK2, 0x02, 0xB1, 0x01}, //4.5ma,but diaodian
};

static u8 lcd_cmds_resume[][ROW_LINE] = {
/*	{0xff, DCS_CMD, SW_PACK1, 0x01, 0x11},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x04}, 
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB3, 0x02},	
	{0x00, GEN_CMD, LW_PACK, 0x07, 0x05, 0x00, 0xB6, 0x52, 0x62, 0x85, 0x00},	//gen long
	{0x00, GEN_CMD, LW_PACK, 0x06, 0x04, 0x00, 0xB7, 0x00, 0x00, 0x01},	//gen long 
	{0x00, GEN_CMD, LW_PACK, 0x0B, 0x09, 0x00, 0xC0, 0x43, 0xFF, 0x04, 0x0B, 0x02, 0x07, 0x07, 0x40},
	{0x00, GEN_CMD, LW_PACK, 0x0A, 0x08, 0x00, 0xC1, 0x50, 0x02, 0x22, 0x00, 0x00, 0x61, 0x11},
	{0x00, GEN_CMD, LW_PACK, 0x1B, 0x19, 0x00, 0xC3, 0x04, 0x00, 0x05, 0x14, 0x80, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x5C},	//gen long
	{0x00, GEN_CMD, LW_PACK, 0x0C, 0x0A, 0x00, 0xD0, 0x74, 0x29, 0xDD, 0x15, 0x09, 0x2B, 0x00, 0xC0, 0xCC},		//Power Setting Charge Pump Setting
	{0x00, GEN_CMD, LW_PACK, 0x10, 0x0E, 0x00, 0xD1, 0x4D, 0x24, 0x34, 0x55, 0x55, 0x77, 0x77, 0x66, 0x31, 0x75, 0x42, 0x86, 0x06},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xD6, 0xA8},	//DSI Setting
	{0x00, GEN_CMD, LW_PACK, 0x05, 0x03, 0x00, 0xC8, 0x1C, 0x23, 0x27, 0x2B, 0x30, 0x34, 0x16, 0x15, 0x12, 0x0E, 0x0B, 0x06, 0x1C, 0x23, 0x27, 0x2B, 0x30, 0x34, 0x16, 0x15, 0x12, 0x0E, 0x0B, 0x06},	//gen long
	{0x00, GEN_CMD, LW_PACK, 0x05, 0x03, 0x00, 0xD5, 0x18, 0x18},	
	{0x05, GEN_CMD, LW_PACK, 0x07, 0x05, 0x00, 0xBB, 0x0B, 0xFF, 0x0F, 0x02, 0x00, 0x00},*/
	{0xff, DCS_CMD, SW_PACK1, 0x01, 0x11},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x04},
	{0x00, GEN_CMD, LW_PACK, 0x07, 0x05, 0x00, 0xBB, 0x7B, 0xFF, 0x0F, 0x02, 0x01, 0x00},
	{0xff, DCS_CMD, SW_PACK1, 0x01, 0x29},
};

static int lcd_auo_eR61308_reset(struct comipfb_info *fbi)
{
	int gpio_rst = fbi->pdata->gpio_rst;

	if (gpio_rst >= 0) {
		gpio_request(gpio_rst, "LCD Reset");
		gpio_direction_output(gpio_rst, 1);
		mdelay(10);
		gpio_direction_output(gpio_rst, 0);
		mdelay(10);
		gpio_direction_output(gpio_rst, 1);
		mdelay(20);
		gpio_free(gpio_rst);
	}
	return 0;
}

static int lcd_auo_eR61308_suspend(struct comipfb_info *fbi)
{
	int ret=0;
	if (!(fbi->pdata->flags & COMIPFB_SLEEP_POWEROFF))
		ret = comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_suspend);
	mdelay(20); /*20ms test find*/
	if(fbi->cdev->reset)
		fbi->cdev->reset(fbi);
	mipi_dsih_dphy_enter_ulps(fbi, ENTER_ULPS);
	return ret;
}
EXPORT_SYMBOL(lcd_auo_eR61308_suspend);

static int lcd_auo_eR61308_resume(struct comipfb_info *fbi)
{
	int ret=0;
	mipi_dsih_dphy_exit_ulps(fbi, EXIT_ULPS);
	mipi_dsih_cmd_mode(fbi, 1);
	if (fbi->pdata->flags & COMIPFB_SLEEP_POWEROFF) {
	if (fbi->cdev->reset)
    		fbi->cdev->reset(fbi);
    	ret = comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_init);
  	} else {
  	    if (fbi->cdev->reset)
    		fbi->cdev->reset(fbi);
  	}
#ifdef CONFIG_FBCON_DRAW_PANIC_TEXT
	if (unlikely(kpanic_in_progress == 1)) {
		ret = comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_init);
	}
	else
		ret = comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_resume);
#else
	ret = comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_resume);
#endif
	return ret;
}
EXPORT_SYMBOL(lcd_auo_eR61308_resume);

struct comipfb_dev lcd_auo_eR61308_dev = {
	.name = "lcd_auo_eR61308",
	.control_id = 0,
	.screen_num = 0,
	.interface_info = COMIPFB_MIPI_IF,
	.default_use = 1,
	.bpp = 32,
	.xres = 720,
	.yres = 1280,
	.flags = 0,
	.pclk = 62000000,//30M,62
	.timing = {
		.mipi = {
			.reference_freq = 26000,	//KHZ
			.output_freq = 71000,		//Kbyte,71M,40M->right,79
			.no_lanes = 3,
			.lcdc_mipi_if_mode = COLOR_CODE_24BIT,
			.hsync = 2,
			.hbp = 60,//60
			.hfp = 130,//130
			.vsync = 1,
			.vbp = 10,
			.vfp = 8,
		},
	},
	.cmds_init = {ARRAY_AND_SIZE(lcd_cmds_init)},
	.cmds_suspend = {ARRAY_AND_SIZE(lcd_cmds_suspend)},
	.cmds_resume = {ARRAY_AND_SIZE(lcd_cmds_resume)},
	.backlight_info = {ARRAY_AND_SIZE(backlight_cmds),
				.brightness_bit = 8,
				},
	.reset = lcd_auo_eR61308_reset,
};

static int __init lcd_auo_eR61308_init(void)
{
	return comipfb_dev_register(&lcd_auo_eR61308_dev);
}

subsys_initcall(lcd_auo_eR61308_init);
