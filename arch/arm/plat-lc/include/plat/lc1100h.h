#ifndef __LC1100H_H__
#define __LC1100H_H__

#include <linux/types.h>
#include <plat/comip-pmic.h>

#define LC1100H_REG_DCEN			0x00
#define LC1100H_REG_DC1OVS			0x01
#define LC1100H_REG_DC2OVS0          		0x02
#define LC1100H_REG_DC3OVS			0x03
#define LC1100H_REG_DC4OVS			0x04
#define LC1100H_REG_DCOCPCR			0x05
#define LC1100H_REG_LDOAEN1         		0x06
#define LC1100H_REG_LDOAEN2			0x07
#define LC1100H_REG_LDOAONEN			0x08
#define LC1100H_REG_LDOAECO			0x09
#define LC1100H_REG_LDOA6OVS		    	0x0a
#define LC1100H_REG_LDOA7OVS			0x0b
#define LC1100H_REG_LDOAOCPEN1			0x0c
#define LC1100H_REG_LDOAOCPEN2			0x0d
#define LC1100H_REG_LDODEN1   			0x10
#define LC1100H_REG_LDODEN2   			0x11
#define LC1100H_REG_LDODONEN  			0x12
#define LC1100H_REG_LDODECO1  			0x13
#define LC1100H_REG_LDODECO2  			0x14
#define LC1100H_REG_LDODOVSEL 			0x15
#define LC1100H_REG_LDOD3OVS  			0x16
#define LC1100H_REG_LDOD7OVS  			0x17
#define LC1100H_REG_LDOD9OVS  			0x18
#define LC1100H_REG_LDOD10OVS 			0x19
#define LC1100H_REG_LDOD11OVS 			0x1a
#define LC1100H_REG_LDODSOFTEN 			0x1b
#define LC1100H_REG_DCMODE 			0x1c
#define LC1100H_REG_DCPFMCTRL 			0x1d
#define LC1100H_REG_LDODOCPEN1			0x1e
#define LC1100H_REG_LDODOCPEN2			0x20
#define LC1100H_REG_DC2OVS1			0x21
#define LC1100H_REG_LDOD1UVPEN			0x22
#define LC1100H_REG_DC_SLEEP_MODE   		0x23
#define LC1100H_REG_LDOA_SLEEP_MODE1		0x24
#define LC1100H_REG_LDOA_SLEEP_MODE2		0x25
#define LC1100H_REG_LDOD_SLEEP_MODE1		0x26
#define LC1100H_REG_LDOD_SLEEP_MODE2		0x27
#define LC1100H_REG_SINKCR          		0x28
#define LC1100H_REG_LEDIOUT         		0x29
#define LC1100H_REG_VIBIOUT         		0x2a
#define LC1100H_REG_INDDIM          		0x2c
#define LC1100H_REG_CHGCR           		0x30
#define LC1100H_REG_CHGVSET         		0x31
#define LC1100H_REG_CHGCSET         		0x32
#define LC1100H_REG_CHGTSET         		0x33
#define LC1100H_REG_CHGSTATUS       		0x34
#define LC1100H_REG_PCST            		0x35
#define LC1100H_REG_STARTUP_STATUS  		0x36
#define LC1100H_REG_SHDN_STATUS     		0x37
#define LC1100H_REG_BATUVCR         		0x49
#define LC1100H_REG_ADCLDOEN        		0x38
#define LC1100H_REG_ADCCR           		0x39
#define LC1100H_REG_ADCCMD          		0x3a
#define LC1100H_REG_ADCEN           		0x3b
#define LC1100H_REG_ADCDAT0         		0x3c
#define LC1100H_REG_ADCDAT1         		0x3d
#define LC1100H_REG_ADCFORMAT       		0x3e
#define LC1100H_REG_I2CCR           		0x40
#define LC1100H_REG_CLKCR           		0x41
#define LC1100H_REG_IRQST1          		0x43
#define LC1100H_REG_IRQST2          		0x44
#define LC1100H_REG_IRQEN1          		0x45
#define LC1100H_REG_IRQEN2          		0x46
#define LC1100H_REG_IRQMSK1         		0x47
#define LC1100H_REG_IRQMSK2         		0x48
#define LC1100H_REG_TSHUT           		0x55
#define LC1100H_REG_CHGTEST         		0x56
#define LC1100H_REG_TESTEN          		0x57
#define LC1100H_REG_VERSION         		0x5F
#define LC1100H_REG_RTC_SEC         		0x60
#define LC1100H_REG_RTC_MIN         		0x61
#define LC1100H_REG_RTC_HOUR        		0x62
#define LC1100H_REG_RTC_DAY         		0x63
#define LC1100H_REG_RTC_WEEK        		0x64
#define LC1100H_REG_RTC_MONTH       		0x65
#define LC1100H_REG_RTC_YEAR        		0x66
#define LC1100H_REG_RTC_TIME_UPDATE 		0x67
#define LC1100H_REG_RTC_32K_ADJL    		0x68
#define LC1100H_REG_RTC_32K_ADJH    		0x69
#define LC1100H_REG_RTC_SEC_CYCL    		0x6a
#define LC1100H_REG_RTC_SEC_CYCH    		0x6b
#define LC1100H_REG_RTC_CALIB_UPDATE		0x6c
#define LC1100H_REG_RTC_AL1_SEC     		0x6d
#define LC1100H_REG_RTC_AL1_MIN     		0x6e
#define LC1100H_REG_RTC_AL1_HOUR    		0x7c
#define LC1100H_REG_RTC_AL1_DAY    	    	0x7d
#define LC1100H_REG_RTC_AL1_MONTH    		0x7e
#define LC1100H_REG_RTC_AL1_YEAR    		0x7f
#define LC1100H_REG_RTC_AL2_SEC     		0x6f
#define LC1100H_REG_RTC_AL2_MIN     		0x70
#define LC1100H_REG_RTC_AL2_HOUR    		0x71
#define LC1100H_REG_RTC_AL2_DAY     		0x72
#define LC1100H_REG_RTC_AL2_MONTH   		0x73
#define LC1100H_REG_RTC_AL2_YEAR    		0x74
#define LC1100H_REG_RTC_AL_UPDATE   		0x75
#define LC1100H_REG_RTC_INT_EN      		0x76
#define LC1100H_REG_RTC_INT_STATUS  		0x77
#define LC1100H_REG_RTC_INT_RAW     		0x78
#define LC1100H_REG_RTC_CTRL        		0x79
#define LC1100H_REG_RTC_BK          		0x7a
#define LC1100H_REG_RTC_INT_MASK    		0x7b
#define LC1100H_REG_PWM_EN          		0x80
#define LC1100H_REG_PWM_UP          		0x81
#define LC1100H_REG_PWM_RESET       		0x82
#define LC1100H_REG_PWM_P           		0x83
#define LC1100H_REG_PWM_OCPY        		0x84

/* DCEN: DC/DC Enable Register */
#define LC1100H_REG_BITMASK_DC2_DVS_SEL		0x30
#define LC1100H_REG_BITMASK_DC4EN      		0x08
#define LC1100H_REG_BITMASK_DC3EN      		0x04
#define LC1100H_REG_BITMASK_DC2EN      		0x02
#define LC1100H_REG_BITMASK_DC1EN      		0x01

/* DC1OVS: DCDC1 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_DC1ONV		0xf0
#define LC1100H_REG_BITMASK_DC1SLPV     	0x0f

/* DC2OVS: DCDC2 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_DC2OVS0VOUT0	0x0f
#define LC1100H_REG_BITMASK_DC2OVS0VOUT1	0xf0
#define LC1100H_REG_BITMASK_DC2OVS1VOUT2	0x0f
#define LC1100H_REG_BITMASK_DC2OVS1VOUT3	0xf0

/* DC3OVS: DCDC3 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_DC3VOUT		0x07

/* DC4OVS: DCDC4 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_DC4VOUT		0x07

/* DCOCPCR: DC-DC Over-current Protection Control Register */
#define LC1100H_REG_BITMASK_DC4OCPOFF		0x80
#define LC1100H_REG_BITMASK_DC4OCPPRO		0x40
#define LC1100H_REG_BITMASK_DC3OCPOFF		0x20
#define LC1100H_REG_BITMASK_DC3OCPPRO		0x10
#define LC1100H_REG_BITMASK_DC2OCPOFF		0x08
#define LC1100H_REG_BITMASK_DC2OCPPRO		0x04
#define LC1100H_REG_BITMASK_DC1OCPOFF		0x02
#define LC1100H_REG_BITMASK_DC1OCPPRO		0x01

#define LC1100H_REG_DATA_OCP_DET_ENABLE		0x00
#define LC1100H_REG_DATA_OCP_DET_DISABLE	0x01
#define LC1100H_REG_DATA_OCP_ENABLE		0x01
#define LC1100H_REG_DATA_OCP_DISABLE		0x00

/* LDOAEN1: LDOA1-8 Enable Register */
#define LC1100H_REG_BITMASK_LDOA8EN		0x80
#define LC1100H_REG_BITMASK_LDOA7EN		0x40
#define LC1100H_REG_BITMASK_LDOA6EN		0x20
#define LC1100H_REG_BITMASK_LDOA5EN		0x10
#define LC1100H_REG_BITMASK_LDOA4EN		0x08
#define LC1100H_REG_BITMASK_LDOA3EN		0x04
#define LC1100H_REG_BITMASK_LDOA2EN		0x02
#define LC1100H_REG_BITMASK_LDOA1EN		0x01

/* LDOAEN2: LDOA9-10 Enable Register */
#define LC1100H_REG_BITMASK_LDOA10EN		0x02
#define LC1100H_REG_BITMASK_LDOA9EN 		0x01

#define LC1100H_REG_DATA_LDOA_ENABLE		0x01
#define LC1100H_REG_DATA_LDOA_DISABLE		0x00

/* LDOAONEN: LDOA External Pin Enable Register */
#define LC1100H_REG_BITMASK_LDOA7ONEN		0x20
#define LC1100H_REG_BITMASK_LDOA5ONEN		0x10
#define LC1100H_REG_BITMASK_LDOA4ONEN		0x08
#define LC1100H_REG_BITMASK_LDOA3ONEN		0x04
#define LC1100H_REG_BITMASK_LDOA2ONEN		0x02
#define LC1100H_REG_BITMASK_LDOA1ONEN		0x01

#define LC1100H_REG_DATA_LDOA_PIN_CTL_ENABLE	0x01
#define LC1100H_REG_DATA_LDOA_PIN_CTL_DISABLE	0x00

/* LDOAECO: LDOA8/9/10 ECO Mode Control Register */
#define LC1100H_REG_BITMASK_LDOA10ECO		0x04
#define LC1100H_REG_BITMASK_LDOA9ECO		0x02
#define LC1100H_REG_BITMASK_LDOA8ECO		0x01

#define LC1100H_REG_DATA_LDOA_ECO_ENABLE	0x01
#define LC1100H_REG_DATA_LDOA_ECO_DISABLE	0x00

/* LDOA6OVS: LDOA6 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_LDOA6OV		0x0f

/* LDOA7OVS: LDOA7 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_LDOA7OV		0x07

/* LDOA1-8 Over-current Protection Enable Register */
#define LC1100H_REG_BITMASK_LDOA8OCPEN		0x80
#define LC1100H_REG_BITMASK_LDOA7OCPEN		0x40
#define LC1100H_REG_BITMASK_LDOA6OCPEN		0x20
#define LC1100H_REG_BITMASK_LDOA5OCPEN		0x10
#define LC1100H_REG_BITMASK_LDOA4OCPEN		0x08
#define LC1100H_REG_BITMASK_LDOA3OCPEN		0x04
#define LC1100H_REG_BITMASK_LDOA2OCPEN		0x02
#define LC1100H_REG_BITMASK_LDOA1OCPEN		0x01

/* LDOA9-10 Over-current Protection Enable Register */
#define LC1100H_REG_BITMASK_LDOA10OCPEN		0x02
#define LC1100H_REG_BITMASK_LDOA9OCPEN		0x01

#define LC1100H_REG_DATA_OCP_ENABLE		0x01
#define LC1100H_REG_DATA_OCP_DISABLE		0x00

/* LDODEN1: LDOD1-8 Enable Register */
#define LC1100H_REG_BITMASK_LDOD8EN		0x80
#define LC1100H_REG_BITMASK_LDOD7EN		0x40
#define LC1100H_REG_BITMASK_LDOD6EN		0x20
#define LC1100H_REG_BITMASK_LDOD5EN		0x10
#define LC1100H_REG_BITMASK_LDOD4EN		0x08
#define LC1100H_REG_BITMASK_LDOD3EN		0x04
#define LC1100H_REG_BITMASK_LDOD2EN		0x02
#define LC1100H_REG_BITMASK_LDOD1EN		0x01

/* LDODEN2: LDOD9-11 Enable Register */
#define LC1100H_REG_BITMASK_LDOD11EN		0x04
#define LC1100H_REG_BITMASK_LDOD10EN		0x02
#define LC1100H_REG_BITMASK_LDOD9EN		0x01

#define LC1100H_REG_DATA_LDOD_ENABLE		0x01
#define LC1100H_REG_DATA_LDOD_DISABLE		0x00

/* LDODONEN: LDOD External Pin Enable Register */
#define LC1100H_REG_BITMASK_LDOD11ONEN		0x04
#define LC1100H_REG_BITMASK_LDOD9ONEN		0x02
#define LC1100H_REG_BITMASK_LDOD7ONEN		0x01

#define LC1100H_REG_DATA_LDOD_PIN_CTL_ENABLE	0x01
#define LC1100H_REG_DATA_LDOD_PIN_CTL_DISABLE	0x00

/* LDODECO1: LDOD1-8 ECO Mode Control Re	gister */
#define LC1100H_REG_BITMASK_LDOD7ECO		0x40
#define LC1100H_REG_BITMASK_LDOD6ECO		0x20
#define LC1100H_REG_BITMASK_LDOD5ECO		0x10
#define LC1100H_REG_BITMASK_LDOD4ECO		0x08
#define LC1100H_REG_BITMASK_LDOD3ECO		0x04
#define LC1100H_REG_BITMASK_LDOD2ECO		0x02
#define LC1100H_REG_BITMASK_LDOD1ECO		0x01

/* LDODECO2: LDOD9-11 ECO Mode Control Register */
#define LC1100H_REG_BITMASK_LDOD11ECO		0x02
#define LC1100H_REG_BITMASK_LDOD9ECO		0x01

#define LC1100H_REG_DATA_LDOD_ECO_ENABLE	0x01
#define LC1100H_REG_DATA_LDOD_ECO_DISABLE	0x00

/* LDODOVSEL: LDOD Output Voltage Select Register */
#define LC1100H_REG_BITMASK_LDOD8OV		0x80
#define LC1100H_REG_BITMASK_LDOD5OV		0x20
#define LC1100H_REG_BITMASK_LDOD4OV		0x10
#define LC1100H_REG_BITMASK_LDOD2OV		0x0c
#define LC1100H_REG_BITMASK_LDOD1OV		0x03

/* LDOD3OVS: LDOD3 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_LDOD3OV		0x0f

/* LDOD7OVS: LDOD7 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_LDOD7OV		0x0f

/* LDOD9OVS: LDOD9 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_LDOD9OV		0x0f

/* LDOD10OVS: LDOD10 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_LDOD10OV		0x0f

/* LDOD11OVS: LDOD11 Output Voltage Setting Register */
#define LC1100H_REG_BITMASK_LDOD11OV		0x0f

/* LDODOCPEN1: LDOD1-8 Over-Current Protection Enable Register */
#define LC1100H_REG_BITMASK_LDOD8OCPEN		0x80
#define LC1100H_REG_BITMASK_LDOD7OCPEN		0x40
#define LC1100H_REG_BITMASK_LDOD6OCPEN		0x20
#define LC1100H_REG_BITMASK_LDOD5OCPEN		0x10
#define LC1100H_REG_BITMASK_LDOD4OCPEN		0x08
#define LC1100H_REG_BITMASK_LDOD3OCPEN		0x04
#define LC1100H_REG_BITMASK_LDOD2OCPEN		0x02
#define LC1100H_REG_BITMASK_LDOD1OCPEN		0x01

/* LDODOCPEN2: LDOD9-11 Over-Current Protection Enable Register */
#define LC1100H_REG_BITMASK_LDOD11OCPEN		0x04
#define LC1100H_REG_BITMASK_LDOD10OCPEN		0x02
#define LC1100H_REG_BITMASK_LDOD9OCPEN		0x01

/* LDOD1UVPEN: LDOD1 Under-voltage Protection Enable Register */
#define LC1100H_REG_BITMASK_LDOD1UVPEN		0x01

/* DC_SLEEP_MODE: DC/DC Sleep Mode Register */
#define LC1100H_REG_BITMASK_DC4_ALLOW_IN_SLP	0x08
#define LC1100H_REG_BITMASK_DC3_ALLOW_IN_SLP	0x04
#define LC1100H_REG_BITMASK_DC2_ALLOW_IN_SLP	0x02
#define LC1100H_REG_BITMASK_DC1_ALLOW_IN_SLP	0x01

/* LDOA_SLEEP_MODE1: LDOA1-8 Sleep Mode Setting Register */
#define LC1100H_REG_BITMASK_LDOA8_ALLOW_IN_SLP	0x80
#define LC1100H_REG_BITMASK_LDOA7_ALLOW_IN_SLP	0x40
#define LC1100H_REG_BITMASK_LDOA6_ALLOW_IN_SLP	0x20
#define LC1100H_REG_BITMASK_LDOA5_ALLOW_IN_SLP	0x10
#define LC1100H_REG_BITMASK_LDOA4_ALLOW_IN_SLP	0x08
#define LC1100H_REG_BITMASK_LDOA3_ALLOW_IN_SLP	0x04
#define LC1100H_REG_BITMASK_LDOA2_ALLOW_IN_SLP	0x02
#define LC1100H_REG_BITMASK_LDOA1_ALLOW_IN_SLP	0x01

/* LDOA_SLEEP_MODE2: LDOA9-10 Sleep Mode Setting Register */
#define LC1100H_REG_BITMASK_LDOA10_ALLOW_IN_SLP	0x02
#define LC1100H_REG_BITMASK_LDOA9_ALLOW_IN_SLP	0x01

/* LDOD_SLEEP_MODE1: LDOD1-8 Sleep Mode Register */
#define LC1100H_REG_BITMASK_LDOD8_ALLOW_IN_SLP	0x80
#define LC1100H_REG_BITMASK_LDOD7_ALLOW_IN_SLP	0x40
#define LC1100H_REG_BITMASK_LDOD6_ALLOW_IN_SLP	0x20
#define LC1100H_REG_BITMASK_LDOD5_ALLOW_IN_SLP	0x10
#define LC1100H_REG_BITMASK_LDOD4_ALLOW_IN_SLP	0x08
#define LC1100H_REG_BITMASK_LDOD3_ALLOW_IN_SLP	0x04
#define LC1100H_REG_BITMASK_LDOD2_ALLOW_IN_SLP	0x02
#define LC1100H_REG_BITMASK_LDOD1_ALLOW_IN_SLP	0x01

/* LDOD_SLEEP_MODE2: LDOD9-11 Sleep Mode Register */
#define LC1100H_REG_BITMASK_LDOD11_ALLOW_IN_SLP	0x04
#define LC1100H_REG_BITMASK_LDOD10_ALLOW_IN_SLP	0x02
#define LC1100H_REG_BITMASK_LDOD9_ALLOW_IN_SLP	0x01

/* SINKCR: Current Sinks Control Register */
#define LC1100H_REG_BITMASK_LCDPWMEN		0x08
#define LC1100H_REG_BITMASK_VIBRATOREN		0x04
#define LC1100H_REG_BITMASK_LCDEN		0x02
#define LC1100H_REG_BITMASK_KEYEN		0x01

/* LEDIOUT: LED Driver Output Current Setting Register */
#define LC1100H_REG_BITMASK_LCDIOUT		0xf0
#define LC1100H_REG_BITMASK_KEYIOUT		0x0f

/* VIBIOUT: LED Driver Output Current Setting Register */
#define LC1100H_REG_BITMASK_VIBIOUT		0x0f

/* INDDIM: LCD current sink Dimming Register */
#define LC1100H_REG_BITMASK_DIMEN		0x02
#define LC1100H_REG_BITMASK_FDIM		0x01

/* CHGCR: Charging Control Register */
#define LC1100H_REG_BITMASK_EOCEN		0x20
#define LC1100H_REG_BITMASK_NTCEN		0x10
#define LC1100H_REG_BITMASK_CHGPROT		0x08
#define LC1100H_REG_BITMASK_TMONEN		0x04
#define LC1100H_REG_BITMASK_BATSEL		0x02
#define LC1100H_REG_BITMASK_ACHGON		0x01

/* CHGVSET: Charging Voltage Setting Register */
#define LC1100H_REG_BITMASK_CVSEL		0x10
#define LC1100H_REG_BITMASK_VHYSSEL		0x08
#define LC1100H_REG_BITMASK_RCHGSEL		0x07

/* CHGCSET: Charging Current Setting Register */
#define LC1100H_REG_BITMASK_CCPC		0x30
#define LC1100H_REG_BITMASK_ACCSEL		0x0f

/* CHGTSET: Charging Timer Setting Register */
#define LC1100H_REG_BITMASK_RTIM		0x60
#define LC1100H_REG_BITMASK_RTIMCLRB		0x08
#define LC1100H_REG_BITMASK_RTIMSTP		0x01

/* CHGSTATUS: Charging Status Register */
#define LC1100H_REG_BITMASK_RTIMOV		0x80
#define LC1100H_REG_BITMASK_CHGCV		0x40
#define LC1100H_REG_BITMASK_CHGOC		0x20
#define LC1100H_REG_BITMASK_ADPOV		0x10
#define LC1100H_REG_BITMASK_BATOT		0x08
#define LC1100H_REG_BITMASK_RECSTATUS		0x04
#define LC1100H_REG_BITMASK_CHGSHDN		0x02
#define LC1100H_REG_BITMASK_CHGCP		0x01

/* PCST: Power-related Status Register */
#define LC1100H_REG_BITMASK_HFPWR		0x40
#define LC1100H_REG_BITMASK_KONMON		0x20
#define LC1100H_REG_BITMASK_BATV45		0x10
#define LC1100H_REG_BITMASK_VBATV32		0x08
#define LC1100H_REG_BITMASK_VBATV33		0x04
#define LC1100H_REG_BITMASK_ADPIN		0x01

/* STARTUP_STATUS: Start-up Status Register */
#define LC1100H_REG_BITMASK_ALARM_STARTUP 	0x08
#define LC1100H_REG_BITMASK_ADPIN_STARTUP 	0x04
#define LC1100H_REG_BITMASK_HF_PWR_STARTUP	0x02
#define LC1100H_REG_BITMASK_KEYON_STARTUP 	0x01

/* SHDN_STATUS: Shut-down Status Register */
#define LC1100H_REG_BITMASK_REGUV_SHDN		0x08
#define LC1100H_REG_BITMASK_BATUV_SHDN		0x04
#define LC1100H_REG_BITMASK_TSD_SHDN		0x02
#define LC1100H_REG_BITMASK_PWREN_SHDN		0x01

/* BATUVCR: Battery Under Voltage Interrupt Control Register */
#define LC1100H_REG_BITMASK_TRMOD		0x10
#define LC1100H_REG_BITMASK_BATUV_TH		0x03

/* ADCLDOEN: LDO for ADC Enable Register */
#define LC1100H_REG_BITMASK_LDOADCEN		0x01

/* ADCCR: A/D Converter Control Register */
#define LC1100H_REG_BITMASK_ADMODE		0x10
#define LC1100H_REG_BITMASK_ADSEL		0x03

/* ADCCMD: A/D Converter Command Register */
#define LC1100H_REG_BITMASK_ADEND		0x10
#define LC1100H_REG_BITMASK_ADSTART		0x01

/* ADCEN: ADC Enable Register */
#define LC1100H_REG_BITMASK_ADCEN		0x01

/* ADCDAT0: 4 LSBs of A/D Conversion Data Register */
#define LC1100H_REG_BITMASK_ADDATL		0x0f

/* ADCDAT1: 8 MSBs of A/D Conversion Data Register */
#define LC1100H_REG_BITMASK_ADDATH		0xff

/* ADCFORMAT: A/D Conversion Data Format Register */
#define LC1100H_REG_BITMASK_ADFORMAT		0x01
#define LC1100H_REG_DATA_ADFORMAT_10		0x01
#define LC1100H_REG_DATA_ADFORMAT_12		0x00

/* RTC_SEC: RTC Second Counter Register */
#define LC1100H_REG_BITMASK_RTC_SEC		0x3f

/* RTC_MIN: RTC Minute Counter Register */
#define LC1100H_REG_BITMASK_RTC_MIN		0x3f

/* RTC_HOUR: RTC Hour Counter Register */
#define LC1100H_REG_BITMASK_RTC_HOUR		0x1f

/* RTC_DAY: RTC Day of a Month Counter Register */
#define LC1100H_REG_BITMASK_RTC_DAY		0x1f

/* RTC_WEEK: RTC Current Day of a Week Counter Register */
#define LC1100H_REG_BITMASK_RTC_WEEK		0x07

/* RTC_MONTH��RTC Month Counter Register */
#define LC1100H_REG_BITMASK_RTC_MONTH		0x0f

/* RTC_YEAR: RTC Year Counter Register */
#define LC1100H_REG_BITMASK_RTC_YEAR		0x7f

/* RTC_TIME_UPDATE: RTC Time Update Enable Register */
#define LC1100H_REG_BITMASK_RTC_TIME_UPDATE	0xff

/* RTC_32K_ADJL: 8 LSBS of RTC Clock Adjust Register */
#define LC1100H_REG_BITMASK_RTC_32K_ADJL	0xff

/* RTC_32K_ADJH: 8 MSBS of RTC Clock Adjust Register */
#define LC1100H_REG_BITMASK_RTC_32K_ADJH	0x0f

/* RTC_SEC_CYCL: 8 LSBS of RTC Second Cycle Limit Register */
#define LC1100H_REG_BITMASK_RTC_SEC_CYCL	0xff

/* RTC_SEC_CYCH: 8 MSBS of RTC Second Cycle Limit Register */
#define LC1100H_REG_BITMASK_RTC_SEC_CYCH	0xff

/* RTC_CALIB_UPDATE: RTC Clock Calibration Update Register */
#define LC1100H_REG_BITMASK_RTC_CALIB_UPDATE	0xff

/* RTC_AL1_SEC: second Setting for Alarm Interrupt 1 */
#define LC1100H_REG_BITMASK_RTC_AL1_SEC		0x3f

/* RTC_AL1_MIN: Minute Setting for Alarm Interrupt 1 */
#define LC1100H_REG_BITMASK_RTC_AL1_MIN		0x3f

/* RTC_AL1_HOUR: Hour Setting for Alarm Interrupt 1 */
#define LC1100H_REG_BITMASK_RTC_AL1_HOUR	0x0f

/* RTC_AL1_DAY: Day Setting for Alarm Interrupt 1 */
#define LC1100H_REG_BITMASK_RTC_AL1_DAY		0x1f

/* RTC_AL1_MONTH: Month Setting for Alarm Interrupt 1 */
#define LC1100H_REG_BITMASK_RTC_AL1_MONTH	0x0f

/* RTC_AL1_YEAR: Year Setting for Alarm Interrupt 1 */
#define LC1100H_REG_BITMASK_RTC_AL1_YEAR	0x7f

/* RTC_AL1_WEEK: Week Setting for Alarm Interrupt 1 */
#define LC1100H_REG_BITMASK_RTC_AL1_WEEK	0x3f

/* RTC_AL2_SEC: second Setting for Alarm Interrupt 2 */
#define LC1100H_REG_BITMASK_RTC_AL2_SEC		0x3f

/* RTC_AL2_MIN: Minute Setting for Alarm Interrupt 2 */
#define LC1100H_REG_BITMASK_RTC_AL2_MIN		0x3f

/* RTC_AL2_HOUR: Minute Setting for Alarm Interrupt 2 */
#define LC1100H_REG_BITMASK_RTC_AL2_HOUR	0x1f

/* RTC_AL2_DAY: Day Setting for Alarm Interrupt 2 */
#define LC1100H_REG_BITMASK_RTC_AL2_DAY		0x1f

/* RTC_AL2_MONTH: Month Setting for Alarm Interrupt 2 */
#define LC1100H_REG_BITMASK_RTC_AL2_MONTH	0x0f

/* RTC_AL2_YEAR: Year Setting for Alarm Interrupt 2 */
#define LC1100H_REG_BITMASK_RTC_AL2_YEAR	0x7f

/* RTC_AL_UPDATE: RTC Alarm Time Update Register */
#define LC1100H_REG_BITMASK_RTC_AL_UPDATE	0xff

/* RTC_INT_EN: RTC Interrupt Enable Register */
#define LC1100H_REG_BITMASK_RTC_AL1_EN		0x20
#define LC1100H_REG_BITMASK_RTC_AL2_EN		0x10
#define LC1100H_REG_BITMASK_RTC_SEC_EN		0x08
#define LC1100H_REG_BITMASK_RTC_MIN_EN		0x04
#define LC1100H_REG_BITMASK_RTC_HOUR_EN		0x02
#define LC1100H_REG_BITMASK_RTC_DAY_EN		0x01

/* RTC_INT_STATUS: RTC Interrupt Status Register */
#define LC1100H_REG_BITMASK_RTC_AL1IS		0x20
#define LC1100H_REG_BITMASK_RTC_AL2IS		0x10
#define LC1100H_REG_BITMASK_RTC_SECIS		0x08
#define LC1100H_REG_BITMASK_RTC_MINIS		0x04
#define LC1100H_REG_BITMASK_RTC_HOURIS		0x02
#define LC1100H_REG_BITMASK_RTC_DAYIS		0x01

/* RTC_INT_RAW: RTC Raw Interrupt Status Register */
#define LC1100H_REG_BITMASK_RTC_AL1_RAW		0x20
#define LC1100H_REG_BITMASK_RTC_AL2_RAW		0x10
#define LC1100H_REG_BITMASK_RTC_SEC_RAW		0x08
#define LC1100H_REG_BITMASK_RTC_MIN_RAW		0x04
#define LC1100H_REG_BITMASK_RTC_HOUR_RAW	0x02
#define LC1100H_REG_BITMASK_RTC_DAY_RAW		0x01

/* RTC_CTRL: RTC Control Register */
#define LC1100H_REG_BITMASK_RTC_RESET_FLAG	0x02
#define LC1100H_REG_BITMASK_RTC_CNT_EN		0x01

/* RTC_BK: RTC Backup Register */
#define LC1100H_REG_BITMASK_RTC_BK_DATA		0xff

/* RTC_MASK: RTC Interrupt Mask Register */
#define LC1100H_REG_BITMASK_RTC_INT_MASK	0xff

/* I2CCR: I2C Interface Control Register */
#define LC1100H_REG_BITMASK_I2C_INCR		0x03

/* CLKCR: Clock Control Register */
#define LC1100H_REG_BITMASK_CLKDIVEN		0x02
#define LC1100H_REG_BITMASK_CLKSEL		0x01

/* IRQST1: Interrupt Status Register 1 */
#define LC1100H_REG_BITMASK_CHGOVIR		0x40
#define LC1100H_REG_BITMASK_CHGOCIR		0x20
#define LC1100H_REG_BITMASK_RCHGIR		0x10
#define LC1100H_REG_BITMASK_CHGCVIR		0x08
#define LC1100H_REG_BITMASK_BATOVIR		0x04
#define LC1100H_REG_BITMASK_BATUVIR		0x02
#define LC1100H_REG_BITMASK_BATOTIR		0x01

/* IRQST2: Interrupt Status Register 2 */
#define LC1100H_REG_BITMASK_TSDIR		0x80
#define LC1100H_REG_BITMASK_RTIMIR		0x40
#define LC1100H_REG_BITMASK_CHGCPIR		0x20
#define LC1100H_REG_BITMASK_ADCCPIR		0x10
#define LC1100H_REG_BITMASK_KONIR		0x08
#define LC1100H_REG_BITMASK_KOFFIR		0x04
#define LC1100H_REG_BITMASK_ADPINIR		0x02
#define LC1100H_REG_BITMASK_ADPOUTIR		0x01

/* IRQEN1: Interrupt Enable Register 1 */
#define LC1100H_REG_BITMASK_CHGOVIREN		0x40
#define LC1100H_REG_BITMASK_CHGOCIREN		0x20
#define LC1100H_REG_BITMASK_RCHGIREN		0x10
#define LC1100H_REG_BITMASK_CHGCVIREN		0x08
#define LC1100H_REG_BITMASK_BATOVIREN		0x04
#define LC1100H_REG_BITMASK_BATUVIREN		0x02
#define LC1100H_REG_BITMASK_BATOTIREN		0x01

/* IRQEN2: Interrupt Enable Register 2 */
#define LC1100H_REG_BITMASK_TSDIREN		0x80
#define LC1100H_REG_BITMASK_RTIMIREN		0x40
#define LC1100H_REG_BITMASK_CHGCPIREN		0x20
#define LC1100H_REG_BITMASK_ADCCPIREN		0x10
#define LC1100H_REG_BITMASK_KONIREN		0x08
#define LC1100H_REG_BITMASK_KOFFIREN		0x04
#define LC1100H_REG_BITMASK_ADPINIREN		0x02
#define LC1100H_REG_BITMASK_ADPOUTIREN		0x01

/* IRQMSK1: Interrupt Mask Register 1 */
#define LC1100H_REG_BITMASK_CHGOVIRMSK		0x40
#define LC1100H_REG_BITMASK_CHGOCIRMSK		0x20
#define LC1100H_REG_BITMASK_RCHGIRMSK		0x10
#define LC1100H_REG_BITMASK_CHGCVIRMSK		0x08
#define LC1100H_REG_BITMASK_BATOVIRMSK		0x04
#define LC1100H_REG_BITMASK_BATUVIRMSK		0x02
#define LC1100H_REG_BITMASK_BATOTIRMSK		0x01

/* IRQMSK2: Interrupt Mask Register 2 */
#define LC1100H_REG_BITMASK_TSDIRMSK		0x80
#define LC1100H_REG_BITMASK_RTIMIRMSK		0x40
#define LC1100H_REG_BITMASK_CHGCPIRMSK		0x20
#define LC1100H_REG_BITMASK_ADCCPIRMSK		0x10
#define LC1100H_REG_BITMASK_KONIRMSK		0x08
#define LC1100H_REG_BITMASK_KOFFIRMSK		0x04
#define LC1100H_REG_BITMASK_ADPINIRMSK		0x02
#define LC1100H_REG_BITMASK_ADPOUTIRMSK		0x01

/* TSHUT: Thermal Shutdown Control Register */
#define LC1100H_REG_BITMASK_TSDEN		0x01

/* PWM_EN: PWM Enable Register */
#define LC1100H_REG_BITMASK_PWM_EN		0x01

/* PWM_UP: PWM Update Register */
#define LC1100H_REG_BITMASK_PWM_UPDATE		0x01

/* PWM_RESET_PWM Reset Register */
#define LC1100H_REG_BITMASK_PWM_RESET		0x01

/* PWM_P: PWM Period Setting Register */
#define LC1100H_REG_BITMASK_PWM_PERIOD		0xff

/* PWM_OCPY: PWM Duty Cycle Setting Register */
#define LC1100H_REG_BITMASK_PWM_OCPY		0x3f

/* CHGTEST: Charger Test Control Register */
#define LC1100H_REG_BITMASK_CHGTEST		0x0f

/* TESTEN: Test Enable Register */
#define LC1100H_REG_BITMASK_DCMEAS		0x10

/* VERSION: Version Number Register */
#define LC1100H_REG_BITMASK_VERSION		0xff

/*-------------------------------------------------------------------------------
			LC1100H register data.
-------------------------------------------------------------------------------*/
/* LC1100H interrupt. */
#define LC1100H_INT_BATOTIR 			(0x000001)
#define LC1100H_INT_BATUVIR			(0x000002)
#define LC1100H_INT_BATOVIR 			(0x000004)
#define LC1100H_INT_CHGCVIR 			(0x000008)
#define LC1100H_INT_RCHGIR			(0x000010)
#define LC1100H_INT_CHGOCIR			(0x000020)
#define LC1100H_INT_CHGOVIR			(0x000040)

#define LC1100H_INT_ADPOUTIR			(0x000100)
#define LC1100H_INT_ADPINIR			(0x000200)
#define LC1100H_INT_KOFFIR			(0x000400)
#define LC1100H_INT_KONIR 			(0x000800)
#define LC1100H_INT_ADCCPIR			(0x001000)
#define LC1100H_INT_CHGCPIR 			(0x002000)
#define LC1100H_INT_RTIMIR			(0x004000)
#define LC1100H_INT_TSDIR			(0x008000)

#define LC1100H_INT_RTC_DAYIS 			(0x010000)
#define LC1100H_INT_RTC_HOURIS			(0x020000)
#define LC1100H_INT_RTC_MINIS 			(0x040000)
#define LC1100H_INT_RTC_SECIS			(0x080000)
#define LC1100H_INT_RTC_AL2IS			(0x100000)
#define LC1100H_INT_RTC_AL1IS			(0x200000)

/* LC1100H week Days. */
#define LC1100H_RTC_WEEK_DAYS			(7)

/* LC1100H RTC base year. */
#define LC1100H_RTC_YEAR_BASE			(2000)
#define LC1100H_RTC_YEAR_MIN 			(2000)
#define LC1100H_RTC_YEAR_MAX 			(2038)

/* LC1100H RTC alarm number. */
#define LC1100H_RTC_ALARM_NUM			(2)

/* LC1100H RTC bit values of alarm disable. */
#define LC1100H_RTC_ALARM_TIME_IGNORE 		(0x80)
#define LC1100H_RTC_ALARM_TIME_MASK 		(0x7f)

/*-------------------------------------------------------------------------------
				LC1100H platform data.
-------------------------------------------------------------------------------*/

#define PMIC_DCDC_NUM				(4)
#define PMIC_DLDO_NUM				(11)
#define PMIC_ALDO_NUM				(10)
#define PMIC_ISINK_NUM				(3)
#define PMIC_EXTERNAL_NUM			(0)

/* LC1100H ADC channel. */
enum {
	LC1100H_ADC_VBATT = 0,
	LC1100H_ADC_TEMP,
	LC1100H_ADC_ADCIN,
};


/* LC1100H LDO VOUT map. */
struct lc1100h_ldo_vout_map {
	u8 ldo_id;
	int *vout_range;
	int vout_range_size;
	unsigned int reg;
	u8 mask;
};

#define LC1100H_FLAGS_NO_BATTERY_DEVICE		(0x00000001)
#define LC1100H_FLAGS_DEVICE_CHECK			(0x00000002)

/* LC1100H platform data. */
struct lc1100h_platform_data {
	int irq_gpio;
	int flags;
	struct pmic_power_ctrl_map* ctrl_map;
	int ctrl_map_num;
	struct pmic_power_module_map* module_map;
	int module_map_num;
	struct pmic_reg_st* init_regs;
	int init_regs_num;
	int (*init)(void);
};

extern int lc1100h_reg_write(u8 reg, u8 value);
extern int lc1100h_reg_read(u8 reg, u8 *value);
extern int lc1100h_read_adc(u8 val, u16 *out);
extern int lc1100h_reg_bit_write(u8 addr, u8 reg_bitmask, u8 value);
extern int lc1100h_reg_bit_set(u8 addr, u8 mask);
extern int lc1100h_reg_bit_clr(u8 addr, u8 mask);
extern int lc1100h_int_mask(int mask);
extern int lc1100h_int_unmask(int mask);
extern int lc1100h_int_status_get(void);

#endif

