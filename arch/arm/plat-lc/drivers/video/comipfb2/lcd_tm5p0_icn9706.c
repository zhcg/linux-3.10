#include "comipfb.h"
#include "comipfb_dev.h"
#include "mipi_cmd.h"
#include "mipi_interface.h"

static u8 lcd_cmds_init[][ROW_LINE] = {
/****Start Initial Sequence ***/
	{0x05, GEN_CMD, LW_PACK,  0x05, 0x03, 0x00,  0xf0, 0x5a, 0x5a},
	{0x05, GEN_CMD, LW_PACK,  0x05, 0x03, 0x00,  0xf1, 0xa5, 0xa5},
	{0x05, GEN_CMD, LW_PACK,  0x05, 0x03, 0x00,  0xf0, 0xb4, 0x4b},                                        
	{0x00, GEN_CMD, LW_PACK,  0x05, 0x03, 0x00,  0xb6, 0x49, 0x49},
	{0x00, GEN_CMD, LW_PACK,  0x08, 0x06, 0x00,  0xb8, 0x24, 0x01, 0x30, 0x44, 0x54},                                        
	{0x05, GEN_CMD, LW_PACK,  0x0d, 0x0b, 0x00,  0xbd, 0x4e, 0x0e, 0x64, 0x64, 0x20, 0x1e, 0x00, 0x14, 0x43, 0x03},                                        
	{0x05, GEN_CMD, LW_PACK,  0x05, 0x03, 0x00,  0xba, 0x30, 0x63},                                        
	{0x05, GEN_CMD, LW_PACK,  0x0b, 0x09, 0x00,  0xc6, 0x02, 0x02, 0x62, 0x02, 0x16, 0x52, 0x34, 0x00}, 
	{0x00, GEN_CMD, LW_PACK,  0x08, 0x06, 0x00,  0xc7, 0x05, 0x21, 0x6b, 0x41, 0x04},
	{0x05, GEN_CMD, LW_PACK,  0x0b, 0x09, 0x00,  0xb1, 0x94, 0x94, 0x02, 0x09, 0x40, 0x04, 0x40, 0x04},
	{0x05, GEN_CMD, LW_PACK,  0x0f, 0x0d, 0x00,  0xb2, 0x37, 0x09, 0x08, 0x89, 0x08, 0x11, 0x22, 0x33, 0x44, 0x81, 0x18, 0x00},                                         
	{0x00, GEN_CMD, LW_PACK,  0x16, 0x14, 0x00,  0xb3, 0x1c, 0x1d, 0x1e, 0x1f, 0x10, 0x12, 0x0c, 0x0e, 0x04, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x06, 0x00, 0x00, 0x00},                                         
	{0x00, GEN_CMD, LW_PACK,  0x16, 0x14, 0x00,  0xb4, 0x1c, 0x1d, 0x1e, 0x1f, 0x11, 0x13, 0x0d, 0x0f, 0x05, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x00, 0x00, 0x00},
                                         
	{0x00, GEN_CMD, LW_PACK,  0x14, 0x12, 0x00,  0xb7, 0x01, 0x01, 0x09, 0x11, 0x0d, 0x15, 0x19, 0x1d, 0x21, 0x1d, 0x00, 0x00, 0x20, 0x00, 0x02, 0xff, 0x3c},
                                         
	{0x00, GEN_CMD, LW_PACK,  0x09, 0x07, 0x00,  0xc1, 0x0e, 0x10, 0x04, 0x0c, 0x10, 0x04},
	{0x05, GEN_CMD, LW_PACK,  0x05, 0x03, 0x00,  0xc2, 0x83, 0x10},
	{0x05, GEN_CMD, LW_PACK,  0x04, 0x02, 0x00,  0x36, 0x0c},
                                         
	{0x00, GEN_CMD, LW_PACK,  0x29, 0x27, 0x00,  0xc8, 0x7c, 0x6d, 0x5d, 0x51, 0x4e, 0x40, 0x44, 0x2d, 0x45, 0x41, 0x3f, 0x5a, 0x46, 0x4c, 0x3f, 0x40, 0x36, 0x28, 0x06, 0x7c, 0x6d, 0x5d, 0x51, 0x4e, 0x40, 0x44, 0x2d, 0x45, 0x41, 0x3f, 0x5a, 0x46, 0x4c, 0x3f, 0x40, 0x36, 0x28, 0x06}, //gamma 
//  {0x00, DCS_CMD, SW_PACK2, 0x02, 0xC0, 0x11},
//	{0x00, DCS_CMD, SW_PACK2, 0x02, 0x51, 0x80},
//	{0x00, DCS_CMD, SW_PACK2, 0x02, 0x53, 0x24},
//	{0x05, DCS_CMD, SW_PACK2, 0x02, 0x55, 0x01},
//	{0x05, DCS_CMD, SW_PACK2, 0x02, 0x36, 0x0c},
	{0x32, DCS_CMD, SW_PACK1, 0x01, 0x11},
	{0x32, DCS_CMD, SW_PACK1, 0x01, 0x29},

};

static u8 lcd_cmds_suspend[][ROW_LINE] = {
	{0x64, DCS_CMD, SW_PACK1, 0x01, 0x28},
	{0xff, DCS_CMD, SW_PACK1, 0x01, 0x10},
};

/*
static u8 lcd_cmds_resume[][ROW_LINE] = {

	{0x00, DCS_CMD, LW_PACK, 0x03, 0xf0, 0x5a, 0x5a},
	{0x00, DCS_CMD, LW_PACK, 0x03, 0xf1, 0xa5, 0xa5},
	{0x00, DCS_CMD, LW_PACK, 0x03, 0xf0, 0xb4, 0x4b},
	{0x00, DCS_CMD, LW_PACK, 0x28, 0xc8, 0x7c, 0x6d, 0x5f, 0x54, 0x50, 0x3f, 0x42, 0x29, 0x3f, 0x3a, 0x36, 0x4f, 0x3c, 0x43, 0x35, 0x31, 0x25, 0x16, 0x06,\
	                                     0x7c, 0x6d, 0x5f, 0x54, 0x50, 0x3f, 0x42, 0x29, 0x3f, 0x3a, 0x36, 0x4f, 0x3c, 0x43, 0x35, 0x31, 0x25, 0x16, 0x06}, //gamma 
  {0x00, DCS_CMD, LW_PACK, 0x06, 0xb8, 0x24, 0x01, 0x30, 0x34, 0x53},
	{0x32, DCS_CMD, LW_PACK, 0x0c, 0xbd, 0x4e, 0x0e, 0x4b, 0x4b, 0x20, 0x1f, 0x00, 0x14, 0x43, 0x03},
	{0x00, DCS_CMD, LW_PACK, 0x18, 0xb3, 0x0e, 0x0c, 0x12, 0x10, 0x04, 0x06, 0x03, 0x00, 0x03, 0x03,\
	                                     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x08, 0x1c, 0x1d},
   {0x00, DCS_CMD, LW_PACK, 0x18, 0xb4, 0x0f, 0x0d, 0x13, 0x11, 0x05, 0x07, 0x03, 0x00, 0x03, 0x03,\
	                                     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x09, 0x1c, 0x1d},
	
	{0x00, DCS_CMD, SW_PACK2, 0x02, 0x51, 0x80},
	{0x00, DCS_CMD, SW_PACK2, 0x02, 0x53, 0x24},
	{0x05, DCS_CMD, SW_PACK2, 0x02, 0x55, 0x01},
	{0x00, DCS_CMD, LW_PACK,  0x05, 0x03, 0x00, 0xb6, 0x2f, 0x2f}, //vcom
	{0x96, DCS_CMD, SW_PACK1, 0x01, 0x11},
	{0x32, DCS_CMD, SW_PACK1, 0x01, 0x29},
	
};

static u8 lcd_pre_read_id[][ROW_LINE] = {
	{0x00, DCS_CMD, LW_PACK, 0x04, 0x02, 0X00, 0x0a, 0x9c},
	//{0x00, DCS_CMD, LW_PACK, 0x10, 0x0E, 0x00, 0xBA, 0x33, 0x83, 0xA8, 0xAD, 0xB6, 0x00, 0x00, 0x40, 0x10, 0xFF, 0x0F, 0x00, 0x80},
};
*/
static struct common_id_info lcd_common_id_info[] = {
	{DCS_CMD, {0x00}, 1, 0x0a},
};
/*
static struct common_id_info lcd_common_esd_info[] = {
	{DCS_CMD, {0x9c}, 1, 0x0a},   
	
};
*/

static int lcd_tm5p0_icn9706_power(struct comipfb_info *fbi, int onoff)
{
	int gpio_rst = fbi->pdata->gpio_rst;

	if (gpio_rst < 0) {
		pr_err("no reset pin found\n");
		return -ENXIO;
	}

	gpio_request(gpio_rst, "LCD Reset");

	if (onoff) {
		gpio_direction_output(gpio_rst, 0);
		pmic_voltage_set(PMIC_POWER_LCD_IO, 0, PMIC_POWER_VOLTAGE_ENABLE);
		mdelay(50);
		pmic_voltage_set(PMIC_POWER_LCD_CORE, 0, PMIC_POWER_VOLTAGE_ENABLE);
		mdelay(50);
		gpio_direction_output(gpio_rst, 1);
		mdelay(10);
		gpio_direction_output(gpio_rst, 0);
		mdelay(10);
		gpio_direction_output(gpio_rst, 1);
		mdelay(180);
	} else {
		gpio_direction_output(gpio_rst, 0);
		pmic_voltage_set(PMIC_POWER_LCD_CORE, 0, PMIC_POWER_VOLTAGE_DISABLE);
		mdelay(10);
		pmic_voltage_set(PMIC_POWER_LCD_IO, 0, PMIC_POWER_VOLTAGE_DISABLE);
		mdelay(10);
	}

	gpio_free(gpio_rst);

	return 0;
}

static int lcd_tm5p0_icn9706_reset(struct comipfb_info *fbi)
{
	int gpio_rst = fbi->pdata->gpio_rst;

	if (gpio_rst >= 0) {
		gpio_request(gpio_rst, "LCD Reset");
		gpio_direction_output(gpio_rst, 1);
		mdelay(10);
		gpio_direction_output(gpio_rst, 0);
		mdelay(10);
		gpio_direction_output(gpio_rst, 1);
		mdelay(180);
		gpio_free(gpio_rst);
	}
	return 0;
}

static int lcd_tm5p0_icn9706_suspend(struct comipfb_info *fbi)
{
	int ret=0;
	struct comipfb_dev_timing_mipi *mipi;

	mipi = &(fbi->cdev->timing.mipi);

	if (mipi->display_mode == MIPI_VIDEO_MODE) {
		mipi_dsih_hal_mode_config(fbi, 1);
	}

	comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_suspend);

	mdelay(20);
	mipi_dsih_dphy_enable_hs_clk(fbi, 0);

	mipi_dsih_dphy_reset(fbi, 0);
	mipi_dsih_dphy_shutdown(fbi, 0);

	return ret;
}

static int lcd_tm5p0_icn9706_resume(struct comipfb_info *fbi)
{
	int ret=0;
	struct comipfb_dev_timing_mipi *mipi;

	mipi = &(fbi->cdev->timing.mipi);

	mipi_dsih_dphy_shutdown(fbi, 1);
	mipi_dsih_dphy_reset(fbi, 1);

	//if (fbi->cdev->reset)
	//	fbi->cdev->reset(fbi);

	if (!(fbi->pdata->flags & COMIPFB_SLEEP_POWEROFF))
		ret = comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_resume);
	else
		ret = comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_init);

	 msleep(20);
	if (mipi->display_mode == MIPI_VIDEO_MODE) {
		mipi_dsih_hal_mode_config(fbi, 0);
	}
	mipi_dsih_dphy_enable_hs_clk(fbi, 1);

	return ret;
}

struct comipfb_dev lcd_tm5p0_icn9706_dev = {
	.name = "lcd_tm5p0_icn9706",
	.interface_info = COMIPFB_MIPI_IF,
	.lcd_id = 0,
	.refresh_en = 1,
	.bpp = 32,
	.xres = 800,
	.yres = 1280,
	.flags = 0,
	.pclk = 65000000,
	.timing = {
		.mipi = {
			.hs_freq = 52000,
			.lp_freq = 13000,		//KHZ
			.no_lanes = 4,
			.display_mode = MIPI_VIDEO_MODE,
			.im_pin_val = 1,
			.color_mode = {
				.color_bits = COLOR_CODE_24BIT,
			},
			.videomode_info = {
				
				.hsync = 16,
				.hbp = 90,
				.hfp = 90,
				.vsync = 4,
				.vbp = 12,
				.vfp = 15,
			    /*.hsync = 14,
			    .hbp = 6,
			    .hfp = 50,
			    .vsync = 12,
			    .vbp = 4,
			    .vfp = 16,*/
				.sync_pol = COMIPFB_VSYNC_HIGH_ACT,
				.lp_cmd_en = 1,                
				.lp_hfp_en = 1,
				.lp_hbp_en = 1,
				.lp_vact_en = 1,
				.lp_vfp_en = 1,
				.lp_vbp_en = 1,
				.lp_vsa_en = 1,
				.mipi_trans_type =VIDEO_BURST_WITH_SYNC_PULSES,
			},
			.phytime_info = {
				.clk_tprepare = 3,  //HSBYTECLK
			},
			.teinfo = {
				.te_source = 1,    //external signal
				.te_trigger_mode = 0,
				.te_en = 0,
				.te_sync_en = 0,
			},
			.ext_info = {
				.eotp_tx_en = 0,
			},
		},
	},
	
	.panel_id_info = {
		.id_info = lcd_common_id_info,
		.num_id_info = 1,
	//	.prepare_cmd = {ARRAY_AND_SIZE(lcd_pre_read_id)},
	},
	.cmds_init = {ARRAY_AND_SIZE(lcd_cmds_init)},
	.cmds_suspend = {ARRAY_AND_SIZE(lcd_cmds_suspend)},
	.cmds_resume = {ARRAY_AND_SIZE(lcd_cmds_init)},//{ARRAY_AND_SIZE(lcd_cmds_resume)},
	.power = lcd_tm5p0_icn9706_power,
	.reset = lcd_tm5p0_icn9706_reset,
	.suspend = lcd_tm5p0_icn9706_suspend,
	.resume = lcd_tm5p0_icn9706_resume,
//#ifdef CONFIG_FB_COMIP_ESD
	//.esd_id_info = {
	//.id_info = lcd_common_esd_info,
	//.num_id_info = 1,
	//},
//#endif
};

static int __init lcd_tm5p0_icn9706_init(void)
{
	return comipfb_dev_register(&lcd_tm5p0_icn9706_dev);
}

subsys_initcall(lcd_tm5p0_icn9706_init);
