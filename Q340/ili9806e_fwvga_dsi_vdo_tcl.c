/*
 * LCM Reversed by Yuvraj Saxena (abyss10@github.com)
 * Don't remove this blocl of code from here!
 * Contact Me (abyss10.github.io)
 */

#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif

/* ---------------------------------------------------------------------------
   Local Constants
   --------------------------------------------------------------------------- */

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (854)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xDD
#define LCM_ID       (0x0604)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

unsigned static int lcm_esd_test = 0;

/* ---------------------------------------------------------------------------
   Local Variables
   --------------------------------------------------------------------------- */

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))

/* ---------------------------------------------------------------------------
   Local Functions
   --------------------------------------------------------------------------- */

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)										lcm_util.dsi_read_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
 
 struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] =
{
	{0xFF,5,{0xFF,0x98,0x06,0x04,0x01}},
	{0x08,1,{0x10}},              
	{0x21,1,{0x01}},          
	{0x30,1,{0x01}},           
	{0x31,1,{0x00}},             
	{0x40,1,{0x10}},           
	{0x41,1,{0x33}},              
	{0x42,1,{0x01}},            
	{0x43,1,{0x09}},             
	{0x44,1,{0x09}},              
	{0x50,1,{0x78}},  
	{0x51,1,{0x78}}, 
	{0x52,1,{0x00}},             
	{0x53,1,{0x6E}},            
	{0x57,1,{0x50}},             
	{0x60,1,{0x07}},              
	{0x61,1,{0x04}},             
	{0x62,1,{0x07}},              
	{0x63,1,{0x02}},            
	{0xA0,1,{0x00}}, 
	{0xA1,1,{0x0D}}, 
	{0xA2,1,{0x11}},  
	{0xA3,1,{0x10}}, 
	{0xA4,1,{0x04}},  
	{0xA5,1,{0x07}},  
	{0xA6,1,{0x05}}, 
	{0xA7,1,{0x03}},  
	{0xA8,1,{0x07}},  
	{0xA9,1,{0x0A}},  
	{0xAA,1,{0x11}}, 
	{0xAB,1,{0x07}},  
	{0xAC,1,{0x0C}}, 
	{0xAD,1,{0x20}}, 
	{0xAE,1,{0x16}}, 
	{0xAF,1,{0x00}},
	{0xC0,1,{0x00}}, 
	{0xC1,1,{0x07}},  
	{0xC2,1,{0x0E}}, 
	{0xC3,1,{0x07}},  
	{0xC4,1,{0x03}}, 
	{0xC5,1,{0x09}}, 
	{0xC6,1,{0x06}}, 
	{0xC7,1,{0x05}},  
	{0xC8,1,{0x08}},  
	{0xC9,1,{0x0C}},  
	{0xCA,1,{0x11}},  
	{0xCB,1,{0x08}}, 
	{0xCC,1,{0x0F}},  
	{0xCD,1,{0x20}},  
	{0xCE,1,{0x1c}},  
	{0xCF,1,{0x00}}, 
	{0xFF,5,{0xFF,0x98,0x06,0x04,0x06}},
	{0x00,1,{0x23}},
	{0x01,1,{0x0A}},
	{0x02,1,{0x00}},    
	{0x03,1,{0x04}},
	{0x04,1,{0x01}},
	{0x05,1,{0x01}},
	{0x06,1,{0x80}},    
	{0x07,1,{0x08}},
	{0x08,1,{0x09}},
	{0x09,1,{0x00}},    
	{0x0A,1,{0x00}},    
	{0x0B,1,{0x00}},    
	{0x0C,1,{0x01}},
	{0x0D,1,{0x01}},
	{0x0E,1,{0x00}},
	{0x0F,1,{0x00}},
	{0x10,1,{0x7E}},
	{0x11,1,{0xF3}},
	{0x12,1,{0x04}},
	{0x13,1,{0x00}},
	{0x14,1,{0x00}},
	{0x15,1,{0xC0}},
	{0x16,1,{0x08}},
	{0x17,1,{0x00}},
	{0x18,1,{0x00}},
	{0x19,1,{0x00}},
	{0x1A,1,{0x00}},
	{0x1B,1,{0x00}},
	{0x1C,1,{0x00}},
	{0x1D,1,{0x00}},
	{0x20,1,{0x01}},
	{0x21,1,{0x23}},
	{0x22,1,{0x45}},
	{0x23,1,{0x67}},
	{0x24,1,{0x01}},
	{0x25,1,{0x23}},
	{0x26,1,{0x45}},
	{0x27,1,{0x67}}, 
	{0x30,1,{0x01}},
	{0x31,1,{0x86}},
	{0x32,1,{0xAA}},
	{0x33,1,{0xDB}},
	{0x34,1,{0xCC}},
	{0x35,1,{0xBD}},
	{0x36,1,{0xEE}},
	{0x37,1,{0xFF}},
	{0x38,1,{0x22}},
	{0x39,1,{0x22}},
	{0x3A,1,{0x99}},
	{0x3B,1,{0x68}},
	{0x3C,1,{0x22}},
	{0x3D,1,{0x22}},
	{0x3E,1,{0x22}},
	{0x3F,1,{0x22}},
	{0x40,1,{0x22}},
	{0x52,1,{0x10}},
	{0x53,1,{0x10}},
	{0x54,1,{0x13}},
	{0xFF,5,{0xFF,0x98,0x06,0x04,0x07}}, 
	{0xB3,1,{0x10}},                
	{0x02,1,{0x77}},     
	{0x17,1,{0x22}},
	{0xE1,1,{0x79}},
	{0xFF,5,{0xFF,0x98,0x06,0x04,0x00}},
	{0x35,1,{0x00}},             
	{0x11,1,{0x00}},              	
	{REGFLAG_DELAY, 120, {}},
	{0x29,1,{0x00}},
	{REGFLAG_DELAY, 10, {}},
        {REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_out_setting[] = {
	{0x13, 1, {0x00}},
	{REGFLAG_DELAY,20,{}},
	{0x11, 1, {0x00}},
        {REGFLAG_DELAY, 120, {}},
	{0x29, 1, {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 20, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
		        dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}

/* ---------------------------------------------------------------------------
   LCM Driver Implementations
   --------------------------------------------------------------------------- */

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));

		params->type = 2;
		params->width = 480;
		params->height = 854;
                params->physical_width  = 61.56;
                params->physical_height = 109.53;
		params->dbi.te_mode = 0;
		params->dsi.mode = 1;
		params->dsi.LANE_NUM = 2;
		params->dsi.data_format.color_order = 0;
		params->dsi.data_format.trans_seq = 0;
		params->dsi.data_format.padding = 0;
		params->dsi.data_format.format = 2;
		params->dsi.packet_size = 256;
		params->dsi.intermediat_buffer_num = 2;
		params->dsi.PS = 2;
		params->dsi.word_count = 1440;
		params->dsi.vertical_sync_active = 5;
		params->dsi.vertical_backporch = 8;
		params->dsi.vertical_frontporch = 8;
		params->dsi.vertical_active_line = 854;
		params->dsi.horizontal_sync_active = 8;
		params->dsi.horizontal_backporch = 50;
		params->dsi.horizontal_frontporch = 46;
		params->dsi.horizontal_active_pixel = 480;
		params->dsi.compatibility_for_nvk = 0;	
		params->dsi.pll_div1 = 1;
		params->dsi.pll_div2 = 1;
		params->dsi.fbk_div = 31;
                params->dsi.lcm_ext_te_monitor = 0;
                params->dsi.noncont_clock = 1;
		params->dsi.noncont_clock_period = 2;
}

static void lcm_init(void)
{
        SET_RESET_PIN(1);
        MDELAY(50);
        SET_RESET_PIN(0);
        MDELAY(120);
        SET_RESET_PIN(1);
        MDELAY(50);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(120);
}

static void lcm_resume(void)
{
	lcm_init();
}

static unsigned int lcm_compare_id(void)
{
	return 1;
}

/* ---------------------------------------------------------------------------
   Get LCM Driver Hooks
   --------------------------------------------------------------------------- */

LCM_DRIVER ili9806e_fwvga_dsi_vdo_tcl_lcm_drv = 
{
        .name		= "ili9806e_fwvga_dsi_vdo_tcl",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
};
