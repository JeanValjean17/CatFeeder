cmake_minimum_required(VERSION 3.29.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# Setup project, output and linker file
project(CatFeeder)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/device/STM32L073RZTX_FLASH.ld)

enable_language(CXX C ASM)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

# List of source files
set(SRC_FILES
        src/display/display_gfx.cpp
        src/display/fonts.c
        src/display/ili9341.cpp
        src/main.cpp
        src/usart.cpp
        src/system_stm32l0xx.c
        src/syscalls.c
        src/sysmem.c
        src/stm32l0xx_it.c
        src/stm32l0xx_hal_msp.c
        src/startup/startup_stm32l073xx.s
        drivers/st/stm32l0xx_hal.c
        drivers/st/stm32l0xx_hal_uart.c
        drivers/st/stm32l0xx_hal_spi.c
        drivers/st/stm32l0xx_hal_rcc.c
        drivers/st/stm32l0xx_hal_rcc_ex.c
        drivers/st/stm32l0xx_hal_gpio.c
        drivers/st/stm32l0xx_hal_dma.c
        drivers/st/stm32l0xx_hal_cortex.c
        )

# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
        -DSTM32L073xx
        -DDEBUG
        -DUSE_HAL_DRIVER
        )
 
# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
        inc
        inc/display
        drivers/cmsis
        drivers/gpio
        drivers/st
        )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m0plus 
        -mfloat-abi=soft 
        -mthumb
        
        -Wall 
        -Wextra 
        -Wpedantic 
        -O0 
        -g3 
        -fdata-sections 
        -ffunction-sections
        -std=c++23
        -fno-rtti 
        -fno-exceptions 
        -fno-threadsafe-statics
        )

# # Linker options
target_link_options(${EXECUTABLE} PRIVATE
        -T${LINKER_FILE}
        -mcpu=cortex-m0plus 
        -mfloat-abi=soft 
        -mthumb
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -lstdc++ 
        -lsupc++
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        -Xlinker -print-memory-usage -Xlinker
        )

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
