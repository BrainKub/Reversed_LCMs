/*----------------------------------------------------------------
 * Author : Yuvraj Saxena (frap130@github.com)
 * Email : thefrap130@gmail.com
 * Note : (Don't remove this block of code from here!)
 * Neither you and nor your any friends or family members will
 * change credits from here! you know the consequences of removing!
 *---------------------------------------------------------------*/

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <platform/mt_pmic.h>
	#define Lcd_Log printf
#else
        #include <linux/string.h>
	#include <linux/kernel.h>
	#include <mach/mt_gpio.h>
	#include <mach/mt_pm_ldo.h>
	#define Lcd_Log printk
#endif

#include "lcm_drv.h"

/*  ---------------------------------------------------------------------------
    Local Constants
    --------------------------------------------------------------------------- */

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (800)
#define LCM_ID       (0x98)

#define   __LCM_TE_ON__

/* ---------------------------------------------------------------------------
   Local Variables
   --------------------------------------------------------------------------- */

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

/* ---------------------------------------------------------------------------
   Local Functions
   --------------------------------------------------------------------------- */

static __inline void send_ctrl_cmd(unsigned int cmd)
{
	lcm_util.send_cmd(cmd);
}

static __inline void send_data_cmd(unsigned int data)
{
	lcm_util.send_data(data);
}

static __inline unsigned int read_data_cmd(void)
{
    return lcm_util.read_data();
}

static __inline void set_lcm_register(unsigned int regIndex,
		unsigned int regData)
{
	send_ctrl_cmd(regIndex);
	send_data_cmd(regData);
}

static void sw_clear_panel(unsigned int color)
{
	unsigned short x0, y0, x1, y1, x, y;
	unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

	x0 = (unsigned short)0;
	y0 = (unsigned short)0;
	x1 = (unsigned short)FRAME_WIDTH-1;
	y1 = (unsigned short)FRAME_HEIGHT-1;

	h_X_start=((x0&0xFF00)>>8);
	l_X_start=(x0&0x00FF);
	h_X_end=((x1&0xFF00)>>8);
	l_X_end=(x1&0x00FF);

	h_Y_start=((y0&0xFF00)>>8);
	l_Y_start=(y0&0x00FF);
	h_Y_end=((y1&0xFF00)>>8);
	l_Y_end=(y1&0x00FF);

        send_ctrl_cmd(0x2A);
	send_data_cmd(h_X_start); 	
	send_data_cmd(l_X_start); 	
	send_data_cmd(h_X_end); 	
	send_data_cmd(l_X_end);
	send_ctrl_cmd(0x2B);
	send_data_cmd(h_Y_start); 
	send_data_cmd(l_Y_start); 
	send_data_cmd(h_Y_end); 
	send_data_cmd(l_Y_end); 
	send_ctrl_cmd(0x2C);
    for (y = y0; y <= y1; ++ y) {
        for (x = x0; x <= x1; ++ x) {
            lcm_util.send_data(color);
        }
    }
}

static unsigned int lcm_compare_id(void)
{
	return 1;
}

static int get_lcd_id(void)
{
	mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
	mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
	mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
	mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
	MDELAY(1);
	
	return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}

static void init_lcm_registers(void)
{
send_ctrl_cmd(0xFF);
send_data_cmd(0xFF); 
send_data_cmd(0x98); 
send_data_cmd(0x26); 
send_ctrl_cmd(0xBC); 
send_data_cmd(0x21); 
send_data_cmd(0x06); 
send_data_cmd(0x20); 
send_data_cmd(0x02); 
send_data_cmd(0x01); 
send_data_cmd(0x01); 
send_data_cmd(0x98); 
send_data_cmd(0x04); 
send_data_cmd(0x05); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x01); 
send_data_cmd(0x01); 
send_data_cmd(0xFF); 
send_data_cmd(0xF0);
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0xC0); 
send_data_cmd(0x08); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00);
send_ctrl_cmd(0xBD);
send_data_cmd(0x01); 
send_data_cmd(0x23); 
send_data_cmd(0x45); 
send_data_cmd(0x67); 
send_data_cmd(0x01); 
send_data_cmd(0x23); 
send_data_cmd(0x45); 
send_data_cmd(0x67); 
send_ctrl_cmd(0xBE);
send_data_cmd(0x12); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x87); 
send_data_cmd(0x96); 
send_data_cmd(0xBA); 
send_data_cmd(0xAB); 
send_data_cmd(0xDC); 
send_data_cmd(0xCD); 
send_data_cmd(0x78); 
send_data_cmd(0x69);
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_ctrl_cmd(0x3A);  
send_data_cmd(0x66); 
send_ctrl_cmd(0xFA); 
send_data_cmd(0x08); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x02); 
send_data_cmd(0x08); 
send_ctrl_cmd(0xB1);
send_data_cmd(0x00); 
send_data_cmd(0x58); 
send_data_cmd(0x03);
send_ctrl_cmd(0xB4);
send_data_cmd(0x00); 
send_ctrl_cmd(0xC1);
send_data_cmd(0x15); 
send_data_cmd(0x80); 
send_data_cmd(0x80);
send_ctrl_cmd(0xC7);	
send_data_cmd(0x59);
send_data_cmd(0x00);
send_data_cmd(0x65);
send_data_cmd(0x00);
send_ctrl_cmd(0x35);
send_data_cmd(0x00); 
send_ctrl_cmd(0xF7);
send_data_cmd(0x02); 
send_ctrl_cmd(0xE0);
send_data_cmd(0x00);
send_data_cmd(0x0A);
send_data_cmd(0x17); 
send_data_cmd(0x13); 
send_data_cmd(0x14);
send_data_cmd(0x1E); 
send_data_cmd(0xCD); 
send_data_cmd(0x0A);
send_data_cmd(0x03);
send_data_cmd(0x05);
send_data_cmd(0x02);
send_data_cmd(0x0D);
send_data_cmd(0x0D);
send_data_cmd(0X33); 
send_data_cmd(0x30);
send_data_cmd(0x00);
send_ctrl_cmd(0xE1); 
send_data_cmd(0x07); 
send_data_cmd(0x13);
send_data_cmd(0x12); 
send_data_cmd(0x19); 
send_data_cmd(0X77); 
send_data_cmd(0x06); 
send_data_cmd(0x05); 
send_data_cmd(0x0B); 
send_data_cmd(0x06); 
send_data_cmd(0x0C); 
send_data_cmd(0x0B); 
send_data_cmd(0x1D); 
send_data_cmd(0x18); 
send_data_cmd(0x00); 
send_ctrl_cmd(0x11);
MDELAY(120);
send_ctrl_cmd(0x29);
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

params->type = 0;
params->width = 480;
params->dbi.port = 0;
params->dbi.clock_freq = 0;
params->dbi.data_format.color_order = 0;
params->dbi.data_format.trans_seq = 0;
params->dbi.data_format.padding = 0;
params->ctrl = 2;
params->dbi.cpu_write_bits = 32;
params->height = 800; 
params->io_select_mode = 1;
params->dbi.data_width = 3;
params->dbi.data_format.format = 3;
params->dbi.data_format.width = 3;
params->dbi.io_driving_current = 1;
params->dpi.mipi_pll_clk_ref = 1;
params->dpi.mipi_pll_clk_div1 = 2;
params->dpi.mipi_pll_clk_div2 = 3;
params->dpi.dpi_clk_div = 3;
params->dpi.clk_pol = 20;
params->dpi.de_pol = 1;
params->dbi.te_mode = 1;
params->dbi.te_edge_polarity = 1;
}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
	MDELAY(1);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(120);

	init_lcm_registers();
}

static void lcm_suspend(void)
{
	send_ctrl_cmd(0x28);
	MDELAY(100);
	send_ctrl_cmd(0x10);
}

static void lcm_resume(void)
{
	lcm_init();
}

static void lcm_update(unsigned int x, unsigned int y,
		unsigned int width, unsigned int height)
{
	unsigned short x0, y0, x1, y1;
	unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

	x0 = (unsigned short)x;
	y0 = (unsigned short)y;
	x1 = (unsigned short)x+width-1;
	y1 = (unsigned short)y+height-1;

	h_X_start=((x0&0xFF00)>>8);
	l_X_start=(x0&0x00FF);
	h_X_end=((x1&0xFF00)>>8);
	l_X_end=(x1&0x00FF);

	h_Y_start=((y0&0xFF00)>>8);
	l_Y_start=(y0&0x00FF);
	h_Y_end=((y1&0xFF00)>>8);
	l_Y_end=(y1&0x00FF);

        send_ctrl_cmd(0x2A);
	send_data_cmd(h_X_start); 	
	send_data_cmd(l_X_start); 
	send_data_cmd(h_X_end); 
	send_data_cmd(l_X_end); 
	send_ctrl_cmd(0x2B);
	send_data_cmd(h_Y_start); 
	send_data_cmd(l_Y_start); 
	send_data_cmd(h_Y_end); 
	send_data_cmd(l_Y_end); 
	send_ctrl_cmd(0x2C);
}

/* ---------------------------------------------------------------------------
   Get LCM Driver Hooks
   --------------------------------------------------------------------------- */

LCM_DRIVER ili9806h_wvga_lcm_drv = 
{
        .name		= "ili9806h_wvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	.compare_id     = lcm_compare_id,
};
