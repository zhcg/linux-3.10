#include "comipfb.h"
#include "comipfb_dev.h"
#include "mipi_cmd.h"
#include "mipi_interface.h"

//[0]: delay after transfer; [1]:count of data; [2]: word count ls; [3]:word count ms; [4]...: data for transfering
static u8 lcd_cmds_init[][ROW_LINE] = {
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x00},
	{0x00, GEN_CMD, LW_PACK, 0x08, 0x06, 0x00, 0xB3, 0x1C, 0x00, 0x00, 0x00, 0x00},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xD6, 0x01},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x03},
	{0x32, DCS_CMD, SW_PACK1, 0x01, 0x29},
	{0x8C, DCS_CMD, SW_PACK1, 0x01, 0x11},
};

static u8 lcd_cmds_pre_suspend[][ROW_LINE] = {
	{0x14, DCS_CMD, SW_PACK1, 0x01, 0x28},
	{0x64, DCS_CMD, SW_PACK1, 0x01, 0x10},
};

static u8 lcd_cmds_suspend[][ROW_LINE] = {
	{0x14, DCS_CMD, SW_PACK1, 0x01, 0x28},
	{0xDC, DCS_CMD, SW_PACK1, 0x01, 0x10},
	//{0x14, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x00},	//B0->00h  Protect Off
	//{0x14, GEN_CMD, SW_PACK2, 0x02, 0xB1, 0x01},	//B1->01h  Deep Standby
};

static u8 lcd_cmds_resume[][ROW_LINE] = {
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x00},
	{0x00, GEN_CMD, LW_PACK, 0x08, 0x06, 0x00, 0xB3, 0x1C, 0x00, 0x00, 0x00, 0x00},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xD6, 0x01},
	{0x00, GEN_CMD, SW_PACK2, 0x02, 0xB0, 0x03},
	{0x00, DCS_CMD, SW_PACK1, 0x01, 0x29},
	{0x8C, DCS_CMD, SW_PACK1, 0x01, 0x11},
};
static struct common_id_info lcd_common_id_info[] = {
	{ DCS_CMD, {0x94, 0x31}, 2, 0X04},
};
static struct common_id_info lcd_common_esd_info[] = {
	{ DCS_CMD, {0x94, 0x31}, 2, 0X04},
};

static u8 prefer_warm_cmds[][ROW_LINE] = {
	/*6500K warm*/
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x08, 0x06, 0x00, 0xB3, 0x1C, 0x00, 0x00, 0x00, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC7, 0x12, 0x1F, 0x2E, 0x3B, 0x47, 0x51, 0x67, 0x77, 0x84, 0x90, 0x42, 0x4C, 0x59, 0x6B, 0x73, 0x7F, 0x8F, 0x9D, 0xA6, 0x00, 0x19, 0x28, 0x33, 0x3F, 0x4B, 0x63, 0x73, 0x82, 0x90, 0x42, 0x4E, 0x5B, 0x6F, 0x79, 0x87, 0x93, 0x9D, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC8, 0x42, 0x48, 0x4C, 0x54, 0x5B, 0x62, 0x72, 0x7F, 0x8B, 0x94, 0x46, 0x4F, 0x5B, 0x6D, 0x76, 0x81, 0x92, 0x9F, 0xA6, 0x3C, 0x40, 0x46, 0x4E, 0x55, 0x5C, 0x6E, 0x7D, 0x89, 0x94, 0x46, 0x51, 0x5F, 0x73, 0x7C, 0x89, 0x96, 0x9F, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC9, 0x4A, 0x4C, 0x51, 0x58, 0x5F, 0x66, 0x74, 0x80, 0x8C, 0x95, 0x47, 0x51, 0x5C, 0x6D, 0x76, 0x82, 0x92, 0xA0, 0xA6, 0x44, 0x48, 0x4B, 0x52, 0x59, 0x60, 0x70, 0x7E, 0x8A, 0x95, 0x47, 0x51, 0x5E, 0x73, 0x7C, 0x8A, 0x96, 0xA0, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xD6, 0x01, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x03, },

};

static u8 prefer_nature_cmds[][ROW_LINE] = {
	/*7300K normal*/
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x08, 0x06, 0x00, 0xB3, 0x1C, 0x00, 0x00, 0x00, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC7, 0x12, 0x21, 0x2E, 0x3D, 0x4A, 0x54, 0x6B, 0x7B, 0x88, 0x93, 0x46, 0x50, 0x5D, 0x70, 0x79, 0x83, 0x98, 0x9E, 0xA6, 0x00, 0x19, 0x24, 0x35, 0x42, 0x4E, 0x65, 0x77, 0x86, 0x93, 0x44, 0x52, 0x5F, 0x74, 0x7D, 0x89, 0x98, 0xA8, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC8, 0x35, 0x3A, 0x42, 0x4D, 0x56, 0x5F, 0x71, 0x80, 0x8C, 0x96, 0x48, 0x52, 0x5E, 0x71, 0x7B, 0x84, 0x99, 0xA3, 0xA6, 0x27, 0x32, 0x3A, 0x45, 0x50, 0x59, 0x6D, 0x7C, 0x8A, 0x96, 0x48, 0x54, 0x62, 0x77, 0x81, 0x8C, 0x99, 0xA3, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC9, 0x12, 0x21, 0x2E, 0x3D, 0x4A, 0x54, 0x6B, 0x7B, 0x88, 0x93, 0x46, 0x50, 0x5D, 0x70, 0x79, 0x83, 0x98, 0x9E, 0xA6, 0x00, 0x19, 0x24, 0x35, 0x42, 0x4E, 0x65, 0x77, 0x86, 0x93, 0x44, 0x52, 0x5F, 0x74, 0x7D, 0x89, 0x98, 0xA8, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xD6, 0x01, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x03, },

};

static u8 prefer_cool_cmds[][ROW_LINE] = {
	/*8100K cool*/
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x08, 0x06, 0x00, 0xB3, 0x1C, 0x00, 0x00, 0x00, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC7, 0x31, 0x38, 0x40, 0x4A, 0x52, 0x5B, 0x6E, 0x7E, 0x8A, 0x95, 0x47, 0x51, 0x5D, 0x71, 0x7B, 0x84, 0x99, 0xA3, 0xA6, 0x23, 0x30, 0x38, 0x42, 0x4C, 0x55, 0x6A, 0x7A, 0x88, 0x95, 0x47, 0x53, 0x61, 0x77, 0x81, 0x8C, 0x99, 0xA3, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC8, 0x39, 0x3D, 0x43, 0x4C, 0x56, 0x5E, 0x71, 0x7F, 0x8B, 0x96, 0x48, 0x52, 0x5E, 0x72, 0x7B, 0x86, 0x97, 0xA3, 0xA6, 0x2B, 0x35, 0x3B, 0x46, 0x50, 0x58, 0x6D, 0x7D, 0x8B, 0x96, 0x48, 0x54, 0x62, 0x78, 0x81, 0x8E, 0x9B, 0xA3, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x29, 0x27, 0x00, 0xC9, 0x12, 0x21, 0x2E, 0x3D, 0x49, 0x53, 0x69, 0x7A, 0x87, 0x93, 0x45, 0x50, 0x5D, 0x70, 0x78, 0x83, 0x97, 0x9E, 0xA6, 0x00, 0x19, 0x24, 0x35, 0x41, 0x4D, 0x65, 0x76, 0x85, 0x93, 0x45, 0x52, 0x5F, 0x74, 0x7E, 0x89, 0x99, 0xA8, 0xAF, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xD6, 0x01, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x03, },

};

static u8 ce_bright_cmds[][ROW_LINE] = {

};

static u8 ce_velvia_cmds[][ROW_LINE] = {
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x08, 0x06, 0x00, 0xB3, 0x1C, 0x00, 0x00, 0x00, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x23, 0x21, 0x00, 0xCA, 0x01, 0x80, 0x85, 0x80, 0xB0, 0xA0, 0xB0, 0x85, 0x0A, 0x20, 0x80, 0x80, 0x10, 0x4A, 0x00, 0xFF, 0x00, 0xFF, 0x10, 0x06, 0x20, 0x10, 0x3F, 0x3F, 0x00, 0x00, 0x10, 0x10, 0x3F, 0x3F, 0x3F, 0x3F, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xD6, 0x01, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x03, },

};

static u8 ce_standard_cmds[][ROW_LINE] = {
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x08, 0x06, 0x00, 0xB3, 0x1C, 0x00, 0x00, 0x00, 0x00, },
	{0x00, GEN_CMD, LW_PACK, 0x23, 0x21, 0x00, 0xCA, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x08, 0x20, 0x80, 0x80, 0x0A, 0x4A, 0x37, 0xA0, 0x55, 0xF8, 0x0C, 0x0C, 0x20, 0x10, 0x3F, 0x3F, 0x00, 0x00, 0x10, 0x10, 0x3F, 0x3F, 0x3F, 0x3F, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xD6, 0x01, },
	{0x00, GEN_CMD, LW_PACK, 0x04, 0x02, 0x00, 0xB0, 0x03, },

};

static struct prefer_ce_info prefer_info[] = {
	{PREFER_WARM, {ARRAY_AND_SIZE(prefer_warm_cmds)}},
	{PREFER_NATURE, {ARRAY_AND_SIZE(prefer_nature_cmds)}},
	{PREFER_COOL, {ARRAY_AND_SIZE(prefer_cool_cmds)}},
};

static struct prefer_ce_info ce_info[] = {
	{CE_BRIGHT, {ARRAY_AND_SIZE(ce_bright_cmds)}},
	{CE_VELVIA, {ARRAY_AND_SIZE(ce_velvia_cmds)}},
	{CE_STANDARD, {ARRAY_AND_SIZE(ce_standard_cmds)}},
};

static int lcd_sharp_eR69431_power(struct comipfb_info *fbi, int onoff)
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
		mdelay(20);
		pmic_voltage_set(PMIC_POWER_LCD_CORE, 0, PMIC_POWER_VOLTAGE_ENABLE);
		mdelay(50);
		gpio_direction_output(gpio_rst, 1);
		mdelay(50);
	} else {
		pmic_voltage_set(PMIC_POWER_LCD_CORE, 0, PMIC_POWER_VOLTAGE_DISABLE);
		mdelay(10);
		gpio_direction_output(gpio_rst, 0);
		mdelay(10);
		pmic_voltage_set(PMIC_POWER_LCD_IO, 0, PMIC_POWER_VOLTAGE_DISABLE);
		mdelay(10);
	}

	gpio_free(gpio_rst);

	return 0;
}

static int lcd_sharp_eR69431_reset(struct comipfb_info *fbi)
{
	int gpio_rst = fbi->pdata->gpio_rst;

	if (gpio_rst >= 0) {
		gpio_request(gpio_rst, "LCD Reset");
		gpio_direction_output(gpio_rst, 1);
		mdelay(10);
		gpio_direction_output(gpio_rst, 0);
		mdelay(20);
		gpio_direction_output(gpio_rst, 1);
		mdelay(100);
		gpio_free(gpio_rst);
	}
	return 0;
}

static int lcd_sharp_eR69431_suspend(struct comipfb_info *fbi)
{
	int ret=0;
	struct comipfb_dev_timing_mipi *mipi;
	//int gpio_rst = fbi->pdata->gpio_rst;

	mipi = &(fbi->cdev->timing.mipi);

	//if (mipi->display_mode == MIPI_VIDEO_MODE) {
	//	mipi_dsih_hal_mode_config(fbi, 1);
	//}
#if 0
	comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_pre_suspend);

	if (gpio_rst >= 0) {
		gpio_request(gpio_rst, "LCD Reset");
		gpio_direction_output(gpio_rst, 0);
		mdelay(10);
		gpio_direction_output(gpio_rst, 1);
		mdelay(10);
		gpio_free(gpio_rst);
	}
#endif
	comipfb_if_mipi_dev_cmds(fbi, &fbi->cdev->cmds_suspend);

	mdelay(20);
	mipi_dsih_dphy_enable_hs_clk(fbi, 0);

	mipi_dsih_dphy_reset(fbi, 0);
	mipi_dsih_dphy_shutdown(fbi, 0);

	return ret;
}

static int lcd_sharp_eR69431_resume(struct comipfb_info *fbi)
{
	int ret=0;
	struct comipfb_dev_timing_mipi *mipi;

	mipi = &(fbi->cdev->timing.mipi);

	mipi_dsih_dphy_shutdown(fbi, 1);
	mipi_dsih_dphy_reset(fbi, 1);
	//mipi_dsih_dphy_ulps_en(fbi, 0);

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

struct comipfb_dev lcd_sharp_eR69431_dev = {
	.name = "lcd_sharp_eR69431",
	.interface_info = COMIPFB_MIPI_IF,
	.lcd_id = LCD_ID_SHARP_R69431,
	.refresh_en = 1,
	.bpp = 32,
	.xres = 720,
	.yres = 1280,
	.flags = RESUME_WITH_PREFER | RESUME_WITH_CE,
	.pclk = 65000000,
	.timing = {
		.mipi = {
			.hs_freq = 65000,//71500,		//Kbyte
			.lp_freq = 13000,//14300,		//KHZ
			.no_lanes = 3,
			.display_mode = MIPI_VIDEO_MODE,
			.im_pin_val = 1,
			.color_mode = {
				.color_bits = COLOR_CODE_24BIT,
			},
			.videomode_info = {
				.hsync = 2,
				.hbp = 28,
				.hfp = 100,
				.vsync = 1,
				.vbp = 14,
				.vfp = 8,
				.sync_pol = COMIPFB_VSYNC_HIGH_ACT,
				.lp_cmd_en = 1,
				.lp_hfp_en = 1,
				.lp_hbp_en = 1,
				.lp_vact_en = 1,
				.lp_vfp_en = 1,
				.lp_vbp_en = 1,
				.lp_vsa_en = 1,
				.mipi_trans_type = VIDEO_BURST_WITH_SYNC_PULSES,
			},
			.phytime_info = {
				.clk_tprepare = 3, //HSBYTECLK
			},
			.teinfo = {
				.te_source = 1, //external signal
				.te_trigger_mode = 0,
				.te_en = 0,
				.te_sync_en = 0,
			},
			.ext_info = {
				.eotp_tx_en = 1,
			},
		},
	},
	.panel_id_info = {
		.id_info = lcd_common_id_info,
		.num_id_info = 1,
	},
	.cmds_init = {ARRAY_AND_SIZE(lcd_cmds_init)},
	.cmds_suspend = {ARRAY_AND_SIZE(lcd_cmds_suspend)},
	.cmds_resume = {ARRAY_AND_SIZE(lcd_cmds_resume)},
	.cmds_pre_suspend = {ARRAY_AND_SIZE(lcd_cmds_pre_suspend)},
#ifdef CONFIG_FB_COMIP_ESD
	.esd_id_info = {
		.id_info = lcd_common_esd_info,
		.num_id_info = 1,
	},
#endif
	.display_prefer_info = {
		.types = ARRAY_SIZE(prefer_info),
		.info = prefer_info,
	},
	.display_ce_info = {
		.types = ARRAY_SIZE(ce_info),
		.info = ce_info,
	},
	.init_last = 1,

	.power = lcd_sharp_eR69431_power,
	.reset = lcd_sharp_eR69431_reset,
	.suspend = lcd_sharp_eR69431_suspend,
	.resume = lcd_sharp_eR69431_resume,
};

static int __init lcd_sharp_eR69431_init(void)
{
	return comipfb_dev_register(&lcd_sharp_eR69431_dev);
}

subsys_initcall(lcd_sharp_eR69431_init);
