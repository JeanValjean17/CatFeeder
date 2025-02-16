#include "stm32l0xx.h"
#include <cstdint>
#include <assert.h>

namespace gpio
{
    enum class pin_state
    {
        low,
        high
    };

    enum class pins : uint16_t
    {
        gpio0 = 0x0001,
        gpio1 = 0x0002,
        gpio2 = 0x0004,
        gpio3 = 0x0008,
        gpio4 = 0x0010,
        gpio5 = 0x0020,
        gpio6 = 0x0040,
        gpio7 = 0x0080,
        gpio8 = 0x0100,
        gpio9 = 0x0200,
        gpio10 = 0x0400,
        gpio11 = 0x0800,
        gpio12 = 0x1000,
        gpio13 = 0x2000,
        gpio14 = 0x4000,
        gpio15 = 0x8000,
    };

    enum class ports : uint8_t
    {
        porta = 0,
        portb = 1,
        portc = 2,
        portd = 3,
    };

    enum class pull_mode : uint32_t
    {
        none = 0x00000000u,
        up = 0x00000001u,
        down = 0x00000002u,
    };

    // Mode definition
    enum class pin_mode : uint32_t
    {
        input = 0x00000000u,              // input mode
        output = 0x00000001u,             // output mode
        alternate_function = 0x00000002u, // alternate function mode
        analog_mode = 0x00000003u
        // af_pp = 0x00000002u,             // alternate function push pull mode
        // af_od = 0x00000012u,             // alternate function open drain mode
    };

    enum class output_type : uint16_t
    {
        push_pull = 0x00000000u,
        open_drain = 0x00000001u,
    };

    enum class speed : uint8_t
    {
        low = 0x00,
        medium = 0x01u,
        high = 0x02u,
        very_high = 0x03u,
    };

    struct config
    {
        gpio::pin_mode mode = gpio::pin_mode::output;
        const GPIO_TypeDef *gpio_port = GPIOA;
        gpio::output_type out_type = gpio::output_type::open_drain;
        gpio::speed speed = gpio::speed::low;
        gpio::pull_mode pull_mode = gpio::pull_mode::none;
    };

    void enable_gpio_port_clk(gpio::ports port)
    {
        volatile uint32_t reg_out;
        switch (port)
        {
        case gpio::ports::porta:
            SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
            reg_out = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
            UNUSED(reg_out);
            break;
        case gpio::ports::portb:
            SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
            reg_out = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOBEN);
            UNUSED(reg_out);
            break;
        case gpio::ports::portc:
            SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
            reg_out = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOCEN);
            UNUSED(reg_out);
            break;
        case gpio::ports::portd:
            SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
            reg_out = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOEEN);
            UNUSED(reg_out);
            break;
        }
    }

    template <pins pin_no>
    class pin
    {
    public:
        static constexpr pins pin_num = pin_no;

        constexpr void configure(gpio::config pin_configuration)
        {
            auto m_mode = pin_configuration.pull_mode;

            if (pin_configuration.mode == gpio::pin_mode::output || 
                pin_configuration.mode == gpio::pin_mode::alternate_function)
            {
                // uint32_t temp =                 
            }
        }
        
        // Check at compile time if the pin_mode is valid
        constexpr void set_output(pin_state state)
        {
            // static_assert(mode == pin_mode::output, "This pin is not configured as output");
        }
        constexpr void toggle();
        constexpr bool read()
        {
            // How to accomodate for both analogue and digital inputs?
            // static_assert(mode == pin_mode::input, "This pin is not configured as input");
            return true;
        }
    };
} // namespace gpio
