#ifndef __ASM_ARCH_BOARD_LC1860_H__
#define __ASM_ARCH_BOARD_LC1860_H__

#include <plat/mfp.h>
#include <plat/comipfb.h>

#if defined(CONFIG_COMIP_PROJECT_LC1860)

#if defined(CONFIG_COMIP_BOARD_LC1860_EVB2)

/* PMIC&CODEC-LC1132. */
#define LC1132_INT_PIN			MFP_PIN_GPIO(239)

/* PMIC&CODEC-LC1160. */
#define LC1160_GPIO_MCLK		MFP_PIN_GPIO(90)
#define LC1160_GPIO_ECLK		MFP_PIN_GPIO(91)
#define LC1160_INT_PIN			MFP_PIN_GPIO(99)
#define LC1160_INT2_PIN		MFP_PIN_GPIO(223)
#define CODEC_PA_PIN			MFP_PIN_GPIO(75)
#define CODEC_JACK_POLARITY	0

/* BQ27510 GASGAUGE. */
#define BQ27510_GASGAUGE_INT_PIN    MFP_PIN_GPIO(134)
/* max17058 GASGAUGE. */
#define MAX17058_FG_INT_PIN         MFP_PIN_GPIO(134)
/* extern charger. */
#define EXTERN_CHARGER_INT_PIN      MFP_PIN_GPIO(132)

/* GPIO keypad. */
#define COMIP_GPIO_KEY_VOLUMEUP		MFP_PIN_GPIO(72) //zhangchg
#define COMIP_GPIO_KEY_VOLUMEDOWN		MFP_PIN_GPIO(73)

/* LCD. */
#define LCD_RESET_PIN			MFP_PIN_GPIO(154)
#define LCD_AVDD_EN_PIN			MFP_PIN_GPIO(158)
#define LCD_AVEE_EN_PIN			MFP_PIN_GPIO(159)

/* KEYPAD LED*/
#define KEYPAD_LED_PIN			MFP_PIN_GPIO(74)

/*Sensors*/
/*ALS+PS*/
#if defined(CONFIG_LIGHT_PROXIMITY_GP2AP030)
#define GP2AP030_INT_PIN		MFP_PIN_GPIO(211)
#endif
#if defined(CONFIG_LIGHT_PROXIMITY_RPR521)
#define RPR521_INT_PIN			MFP_PIN_GPIO(211)
#endif
/* Touchscreen-FT5X06. */
#if defined(CONFIG_TOUCHSCREEN_FT5X06)
#define FT5X06_INT_PIN			MFP_PIN_GPIO(166)
#define FT5X06_RST_PIN			MFP_PIN_GPIO(165)
#endif

#if defined(CONFIG_TOUCHSCREEN_MELFAS_MMS200)
#define MMS200_INT_PIN			MFP_PIN_GPIO(166)
#define MMS200_RST_PIN			MFP_PIN_GPIO(165)
#endif

#if defined(CONFIG_TOUCHSCREEN_MELFAS_MMS438)
#define MMS400_INT_PIN			MFP_PIN_GPIO(166)
#define MMS400_RST_PIN			MFP_PIN_GPIO(165)
#endif

/* Touchscreen-BF686x. */
#if defined(CONFIG_TOUCHSCREEN_BF686x)||defined(CONFIG_TOUCHSCREEN_BF67xx)||\
	defined(CONFIG_TOUCHSCREEN_BF675x)
#define BF686x_INT_PIN                MFP_PIN_GPIO(162)
#define BF686x_RST_PIN                MFP_PIN_GPIO(165)
#endif

/* CAMERA. */
#if defined(CONFIG_VIDEO_OV5648_2LANE_19M)
#define OV5648_POWERDOWN_PIN		MFP_PIN_GPIO(172)
#define OV5648_RESET_PIN		MFP_PIN_GPIO(170)
#endif

#if defined(CONFIG_VIDEO_OV4689)
#define OV4689_POWERDOWN_PIN	MFP_PIN_GPIO(171)
#define OV4689_RESET_PIN		MFP_PIN_GPIO(169)
#endif
#if defined(CONFIG_VIDEO_OV8865)
#define OV8865_POWERDOWN_PIN		MFP_PIN_GPIO(172)
#define OV8865_RESET_PIN		MFP_PIN_GPIO(170)
#endif

#if defined(CONFIG_VIDEO_OV13850)
#define OV13850_POWERDOWN_PIN		MFP_PIN_GPIO(171)
#define OV13850_RESET_PIN		MFP_PIN_GPIO(169)
#define OV13850_CORE_POWER_PIN		MFP_PIN_GPIO(176)
#endif

#if defined(CONFIG_VIDEO_GC2355)
#define GC2355_POWERDOWN_PIN		MFP_PIN_GPIO(172)
#define GC2355_RESET_PIN		MFP_PIN_GPIO(170)
#define GC2355_CORE_POWER_PIN		MFP_PIN_GPIO(176)
#endif

#if defined(CONFIG_VIDEO_OV9760)
#define OV9760_POWERDOWN_PIN		MFP_PIN_GPIO(172)
#define OV9760_RESET_PIN		MFP_PIN_GPIO(170)
#endif

#if defined(CONFIG_VIDEO_BF3A20_MIPI)
#define BF3A20_MIPI_POWERDOWN_PIN		MFP_PIN_GPIO(172)
#define BF3A20_MIPI_RESET_PIN			MFP_PIN_GPIO(170)
#endif

#if defined(CONFIG_GPS_UBLOX)
#define UBLOX_GPS_RESET_PIN		MFP_PIN_GPIO(205)
#endif

#ifdef CONFIG_BRCM_GPS
#define BCM_GPS_RESET_PIN		MFP_PIN_GPIO(MFP_PIN_MAX)
#define BCM_GPS_STANDBY_PIN		MFP_PIN_GPIO(206)
#endif

#if defined(CONFIG_VIDEO_HM5040)
#define HM5040_POWERDOWN_PIN		MFP_PIN_GPIO(171)
#define HM5040_RESET_PIN		MFP_PIN_GPIO(169)
#define HM5040_CORE_POWER_PIN		MFP_PIN_GPIO(176)
#endif

#if defined(CONFIG_VIDEO_SP5408)
#define SP5408_POWERDOWN_PIN		MFP_PIN_GPIO(171)
#define SP5408_RESET_PIN		MFP_PIN_GPIO(169)
#define SP5408_CORE_POWER_PIN		MFP_PIN_GPIO(176)
#endif

#if defined(CONFIG_VIDEO_SP5409)
#define SP5409_POWERDOWN_PIN		MFP_PIN_GPIO(172)
#define SP5409_RESET_PIN		MFP_PIN_GPIO(170)
#endif

#if defined(CONFIG_VIDEO_SP2508)
#define SP2508_POWERDOWN_PIN		MFP_PIN_GPIO(172)
#define SP2508_RESET_PIN		MFP_PIN_GPIO(170)
#endif

#if defined(CONFIG_VIDEO_SP2529)
#define SP2529_POWERDOWN_PIN		MFP_PIN_GPIO(172)
#define SP2529_RESET_PIN		MFP_PIN_GPIO(170)
#endif

/*rtk wifi*/
#ifdef CONFIG_RTK_WLAN_SDIO
#define RTK_WIFI_ONOFF_PIN		MFP_PIN_GPIO(199)
#define RTK_WIFI_WAKE_I_PIN		MFP_PIN_GPIO(203)
#define RTK_BTWIFI_CHIPEN_PIN		MFP_PIN_GPIO(140)
#endif

#if defined (CONFIG_BRCM_BLUETOOTH) || defined (CONFIG_RTK_BLUETOOTH)
#define UART2_CTS_PIN		MFP_PIN_GPIO(185)
#endif

/* BT-RTL8723BS. */
#ifdef CONFIG_RTK_BLUETOOTH
#define RTK_BT_ONOFF_PIN		MFP_PIN_GPIO(MFP_PIN_MAX)
#define RTK_BT_RESET_PIN		MFP_PIN_GPIO(198)
#define RTK_BT_WAKE_I_PIN		MFP_PIN_GPIO(200)
#define RTK_BT_WAKE_O_PIN		MFP_PIN_GPIO(202)
#endif

#ifdef CONFIG_RADIO_RTK8723B
#define RTK_FM_EN_PIN 			MFP_PIN_GPIO(143)
#endif

#ifdef CONFIG_USB_COMIP_OTG
/* USB OTG ID PIN. */
#define USB_OTG_ID_PIN 			MFP_PIN_GPIO(161)
#endif

#ifdef CONFIG_COMIP_SDCARD_DETECT
/*SDCARD DETECT PIN.*/ 
#define SDCARD_DETECT_PIN 			MFP_PIN_GPIO(243)
#endif

#if defined(CONFIG_SENSORS_AK09911)
#define AKM09911_GPIO_RST              MFP_PIN_GPIO(210)
#endif

#elif defined(CONFIG_COMIP_BOARD_LC1860_EVB3)

/* PMIC&CODEC-LC1132. */
#define LC1132_INT_PIN			MFP_PIN_GPIO(239)

/* PMIC&CODEC-LC1160. */
#define LC1160_GPIO_MCLK		MFP_PIN_GPIO(90)
#define LC1160_GPIO_ECLK		MFP_PIN_GPIO(91)
#define LC1160_INT_PIN			MFP_PIN_GPIO(99)
#define LC1160_INT2_PIN		MFP_PIN_GPIO(223)
#define HOOK_PIN			MFP_PIN_GPIO(226)
#define FSA8108_INT_PIN			MFP_PIN_GPIO(135)
//#define CODEC_PA_PIN			MFP_PIN_GPIO(102)
//#define EAR_SWITCH_PIN			MFP_PIN_GPIO(75)
//#define CODEC_PA_PIN			MFP_PIN_GPIO(75)
#define CODEC_JACK_POLARITY	0

/* BQ27510 GASGAUGE. */
#define BQ27510_GASGAUGE_INT_PIN    MFP_PIN_GPIO(134)
/* max17058 GASGAUGE. */
#define MAX17058_FG_INT_PIN         MFP_PIN_GPIO(134)
/* extern charger. */
#define EXTERN_CHARGER_INT_PIN      MFP_PIN_GPIO(132)

/* GPIO keypad. */
#define COMIP_GPIO_KEY_VOLUMEUP		MFP_PIN_GPIO(72)
#define COMIP_GPIO_KEY_VOLUMEDOWN		MFP_PIN_GPIO(73)

/* LCD. */
#define LCD_RESET_PIN			MFP_PIN_GPIO(154)
#define LCD_BACKLIGHT_PIN		MFP_PIN_GPIO(146) // zhangchg
/* remove by jinmeng 
#define LCD_AVDD_EN_PIN			MFP_PIN_GPIO(158)
#define LCD_AVEE_EN_PIN			MFP_PIN_GPIO(159)
end remove 2016/11/15*/

//add by jinmeng 
/* CSD01 */
#define DBB2CSD_SPI0_CS         MFP_PIN_GPIO(160)
#define DBB2CSD_SPI0_CLK		MFP_PIN_GPIO(159)
#define CSD2DBB_SPI0_DI         MFP_PIN_GPIO(158)
#define DBB2CSD_SPI0_DO         MFP_PIN_GPIO(157)
//end add 2016/11/15


/* KEYPAD LED*/
#define KEYPAD_LED_PIN			MFP_PIN_GPIO(72)

/*Sensors*/
/*ALS+PS*/
#if defined(CONFIG_LIGHT_PROXIMITY_GP2AP030)
#define GP2AP030_INT_PIN		MFP_PIN_GPIO(211)
#endif

#if defined(CONFIG_LIGHT_PROXIMITY_RPR521)
#define RPR521_INT_PIN			MFP_PIN_GPIO(211)
#endif

#if defined(CONFIG_SENSORS_AK09911)
#define AKM09911_GPIO_RST              MFP_PIN_GPIO(210)
#endif

/* Touchscreen-FT5X06. */
#if defined(CONFIG_TOUCHSCREEN_FT5X06)
#define FT5X06_INT_PIN			MFP_PIN_GPIO(166)
#define FT5X06_RST_PIN			MFP_PIN_GPIO(165)
#endif

/* Touchscreen-ICN85xx. */
#if defined(CONFIG_TOUCHSCREEN_ICN8528F)
#define ICN85XX_INT_PIN			MFP_PIN_GPIO(166)
#define ICN85XX_RST_PIN			MFP_PIN_GPIO(165)
#endif

#if defined(CONFIG_TOUCHSCREEN_MELFAS_MMS200)
#define MMS200_INT_PIN			MFP_PIN_GPIO(166)
#define MMS200_RST_PIN			MFP_PIN_GPIO(165)
#endif

#if defined(CONFIG_TOUCHSCREEN_MELFAS_MMS438)
#define MMS400_INT_PIN			MFP_PIN_GPIO(166)
#define MMS400_RST_PIN			MFP_PIN_GPIO(165)
#endif

/* Touchscreen-BF686x. */
#if defined(CONFIG_TOUCHSCREEN_BF686x)||defined(CONFIG_TOUCHSCREEN_BF67xx)||\
	defined(CONFIG_TOUCHSCREEN_BF675x)
#define BF686x_INT_PIN                MFP_PIN_GPIO(162)
#define BF686x_RST_PIN                MFP_PIN_GPIO(165)
#endif

//flash
#define FLASHER_EN			MFP_PIN_GPIO(175)
#define FLASHER_ENM			MFP_PIN_GPIO(174)

/* CAMERA. */
#if defined(CONFIG_VIDEO_OV5648_2LANE_19M)
#define OV5648_POWERDOWN_PIN		MFP_PIN_GPIO(202)
#define OV5648_RESET_PIN		MFP_PIN_GPIO(205)
#endif

#if defined(CONFIG_VIDEO_OV2680)
#define OV2680_POWERDOWN_PIN		MFP_PIN_GPIO(171)
#define OV2680_RESET_PIN		MFP_PIN_GPIO(169)
#define OV2680_MCLK_PIN			MFP_PIN_GPIO(178)
#endif

#if defined(CONFIG_VIDEO_OV4689)
#define OV4689_POWERDOWN_PIN		MFP_PIN_GPIO(201)
#define OV4689_RESET_PIN		MFP_PIN_GPIO(204)
#endif
#if defined(CONFIG_VIDEO_OV8865)
#define OV8865_POWERDOWN_PIN		MFP_PIN_GPIO(202)
#define OV8865_RESET_PIN		MFP_PIN_GPIO(205)
#endif

#if defined(CONFIG_VIDEO_OV13850)
#define OV13850_POWERDOWN_PIN		MFP_PIN_GPIO(201)
#define OV13850_RESET_PIN		MFP_PIN_GPIO(204)
#endif

#if defined(CONFIG_VIDEO_GC2355)
#define GC2355_POWERDOWN_PIN		MFP_PIN_GPIO(202)
#define GC2355_RESET_PIN		MFP_PIN_GPIO(205)
#define GC2355_CORE_POWER_PIN		MFP_PIN_GPIO(176)
#endif

#if defined(CONFIG_VIDEO_OV9760)
#define OV9760_POWERDOWN_PIN		MFP_PIN_GPIO(202)
#define OV9760_RESET_PIN		MFP_PIN_GPIO(205)
#endif

#if defined(CONFIG_VIDEO_BF3A20_MIPI)
#define BF3A20_MIPI_POWERDOWN_PIN		MFP_PIN_GPIO(202)
#define BF3A20_MIPI_RESET_PIN			MFP_PIN_GPIO(205)
#endif

#if defined(CONFIG_VIDEO_HM5040)
#define HM5040_POWERDOWN_PIN		MFP_PIN_GPIO(201)
#define HM5040_RESET_PIN		MFP_PIN_GPIO(204)
#define HM5040_CORE_POWER_PIN		MFP_PIN_GPIO(176)
#endif

#if defined(CONFIG_VIDEO_SP5408)
#define SP5408_POWERDOWN_PIN		MFP_PIN_GPIO(201)
#define SP5408_RESET_PIN		MFP_PIN_GPIO(204)
#define SP5408_CORE_POWER_PIN		MFP_PIN_GPIO(176)
#endif

#if defined(CONFIG_VIDEO_SP5409)
#define SP5409_POWERDOWN_PIN		MFP_PIN_GPIO(202)
#define SP5409_RESET_PIN		MFP_PIN_GPIO(205)
#endif

#if defined(CONFIG_VIDEO_SP2508)
#define SP2508_POWERDOWN_PIN		MFP_PIN_GPIO(202)
#define SP2508_RESET_PIN		MFP_PIN_GPIO(205)
#endif

#if defined(CONFIG_VIDEO_SP2529)
#define SP2529_POWERDOWN_PIN		MFP_PIN_GPIO(202)
#define SP2529_RESET_PIN		MFP_PIN_GPIO(205)
#endif

#if defined(CONFIG_GPS_UBLOX)
#define UBLOX_GPS_RESET_PIN             MFP_PIN_GPIO(172) //205
#endif

#ifdef CONFIG_BRCM_GPS
#define BCM_GPS_RESET_PIN               MFP_PIN_GPIO(MFP_PIN_MAX)
#define BCM_GPS_STANDBY_PIN             MFP_PIN_GPIO(206) // ? 
#endif
#ifdef CONFIG_BRCM_BLUETOOTH
#define BRCM_BT_ONOFF_PIN       MFP_PIN_GPIO(198) //zhangchg
#define BRCM_BT_RESET_PIN		MFP_PIN_GPIO(101) //zhangchg
#define BRCM_BT_WAKE_I_PIN		MFP_PIN_GPIO(200)
#define BRCM_BT_WAKE_O_PIN		MFP_PIN_GPIO(202) //zhangchg
#endif

#ifdef CONFIG_BRCM_WIFI
#define BRCM_WIFI_ONOFF_PIN		MFP_PIN_GPIO(199) //zhangchg
#define BRCM_WIFI_WAKE_I_PIN		MFP_PIN_GPIO(203)
#define BRCM_WIFI_SDIO_CLK_PIN		MFP_PIN_GPIO(191)
#define BRCM_WIFI_SDIO_CMD_PIN		MFP_PIN_GPIO(192)
#define BRCM_WIFI_SDIO_DATA3_PIN	MFP_PIN_GPIO(193)
#define BRCM_WIFI_SDIO_DATA2_PIN	MFP_PIN_GPIO(194)
#define BRCM_WIFI_SDIO_DATA1_PIN	MFP_PIN_GPIO(195)
#define BRCM_WIFI_SDIO_DATA0_PIN	MFP_PIN_GPIO(196)
#endif

/*rtk wifi*/
#ifdef CONFIG_RTK_WLAN_SDIO
#define RTK_WIFI_ONOFF_PIN		MFP_PIN_GPIO(199)
#define RTK_WIFI_WAKE_I_PIN		MFP_PIN_GPIO(203)
#define RTK_BTWIFI_CHIPEN_PIN		MFP_PIN_GPIO(140)
#endif

#if defined (CONFIG_BRCM_BLUETOOTH) || defined (CONFIG_RTK_BLUETOOTH)
#define UART2_CTS_PIN		MFP_PIN_GPIO(185)
#endif

/* BT-RTL8723BS. */
#ifdef CONFIG_RTK_BLUETOOTH
#define RTK_BT_ONOFF_PIN		MFP_PIN_GPIO(MFP_PIN_MAX)
#define RTK_BT_RESET_PIN		MFP_PIN_GPIO(198)
#define RTK_BT_WAKE_I_PIN		MFP_PIN_GPIO(200)
#define RTK_BT_WAKE_O_PIN		MFP_PIN_GPIO(198)
#endif

#ifdef CONFIG_RADIO_RTK8723B
#define RTK_FM_EN_PIN 			MFP_PIN_GPIO(143)
#endif

#ifdef CONFIG_USB_COMIP_OTG
/* USB OTG ID PIN. */
#define USB_OTG_ID_PIN 			MFP_PIN_GPIO(161)
#endif

#ifdef CONFIG_COMIP_SDCARD_DETECT
/* SDCARD DETECT PIN. */
#define SDCARD_DETECT_PIN 			MFP_PIN_GPIO(134) //zhangchg
#endif

#endif

#endif
#endif /*__ASM_ARCH_BOARD_LC1860_H__*/
