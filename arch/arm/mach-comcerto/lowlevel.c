#include <common.h>
#include <config.h>	
#include <mach/clkcore.h>
#include <mach/ddr.h>
#include <mach/gpio.h>
#include <asm/io.h>
#include <mach/serdes.h>
#include <mach/gpio.h>

extern void SoC_PLL_init(void);

extern void SoC_DDR2_init(void);
extern int serdes_phy_init(int phy_num, struct serdes_regs_s *regs, int size, int type);
void comcerto_pad_config();

#if defined CONFIG_COMCERTO_DDR && CONFIG_COMCERTO_DDR_ECC
#define  TRANSFER_LEN   60 * 1024

void ddr_ecc_init(void)
{
        u32 status;
        int incr;
        int i;
        int length = TRANSFER_LEN;

        for(incr = 0 ; incr < DDR_SIZE; )
        {
                mdma_transfer_single(COMCERTO_AXI_IRAM_BASE, COMCERTO_AXI_DDR_BASE + incr, length);
                incr += TRANSFER_LEN/4;
        }
}
#endif


void bsp_init(void)
{
	int val;
#ifdef CONFIG_COMCERTO_PLL
	SoC_PLL_init();

	/* Populate Part Number Info for Linux */
	*((unsigned int *) IRAM_C2K_PART_NO_LOCATION) = *((unsigned int *)(COMCERTO_AXI_IRAM_BASE+0x30));
#endif
	comcerto_pad_config();

	//DDRC ODT Source Select
	writel( (1 << 6) | readl(COMCERTO_GPIO_MISC_PIN_SELECT_REG), COMCERTO_GPIO_MISC_PIN_SELECT_REG);

	// set GPIO pin select for GPIO[15:0]
	writel(0x05000000, COMCERTO_GPIO_PIN_SELECT_REG);       // GPIO[12] = PWM[4], GPIO[13] = PWM[5]

	// set Output Enable for GPIO[31:0] (0: Disable (default), 1: Enable)
	writel((readl(COMCERTO_GPIO_OE_REG) & ~0xFFFF) | 0xFF00, COMCERTO_GPIO_OE_REG);

	// set default output values for GPIO[15:8] - Power Off Pin(GPIO[15]) must be set to "0"
	writel((0x0 << 8), COMCERTO_GPIO_OUTPUT_REG);

	
	// set GPIO pin select for GPIO[63:32]
	writel(0x0FFFFFFE, COMCERTO_GPIO_EXT_PIN_SELECT_REG);

	// set Output Enable for GPIO[63:32] (0: Enable (output) (default), 1: Disable (input))
	writel(0x10000001, COMCERTO_GPIO_EXT_OE_REG);

	// set default output values for GPIO[63:32]
	//writel(0x01000020, COMCERTO_GPIO_EXT_OUTPUT_REG);	// SYS LED will be set to white, and MCU will be set to normal mode
	writel(0x05550020, COMCERTO_GPIO_EXT_OUTPUT_REG);	// set all LEDs to white, and MCU will be set to normal mode

#ifdef CONFIG_COMCERTO_DDR
	/*Double reset HFE helps some times */
        val = readl(AXI_RESET_1);
	writel((val | HFE_AXI_RESET), AXI_RESET_1);
        writel((val & ~(HFE_AXI_RESET)), AXI_RESET_1);

	SoC_DDR_init();
        /* Disable remapping so that lower address space is mapped to DDR space */
	writel((DISABLE_FABRIC_REMAP | readl(COMCERTO_GPIO_PAD_CTRL)), COMCERTO_GPIO_PAD_CTRL);

#ifdef CONFIG_COMCERTO_DDR_ECC
	writel(0x10000, DDRC_CTL_32_REG);
	writel(0x10000, DDRC_CTL_33_REG);
	writel(0x1f0023, DDR_PHY_CTL_00_REG);
	ddr_ecc_init();
	writel(0x30000, DDRC_CTL_32_REG);
	writel(0x0, DDRC_CTL_33_REG);
#endif
#endif
}

int HAL_get_c2k_revision()
{
	return (readl(COMCERTO_GPIO_DEVICE_ID_REG) >> 24) & 0xf;
}
