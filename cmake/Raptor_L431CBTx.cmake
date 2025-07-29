#--------------------------------------#
#               DEFINES                #
#--------------------------------------#
target_compile_definitions(${PROJECT_NAME} PRIVATE
    STM32L431xx
    USE_HAL_DRIVER
)

#--------------------------------------#
#               INCLUDES               #
#--------------------------------------#
target_include_directories(${PROJECT_NAME} PRIVATE

    ${DIR_SRC}
    ${DIR_BOARD_RAPTOR_L431CBTx}
    ${DIR_BOARD_RAPTOR_L431CBTx}/debug
    ${DIR_BOARD_RAPTOR_L431CBTx}/hal
    ${DIR_BOARD_RAPTOR_L431CBTx}/startup
    ${DIR_BOARD_RAPTOR_L431CBTx}/system
    ${DIR_L4HAL}/Inc 
    ${DIR_CMSIS}/Device/ST/STM32L4xx/Include  
    ${DIR_CMSIS}/Include
)


#--------------------------------------#
#          MCU FLAGS                   #
#--------------------------------------#
set(CPU "-mcpu=cortex-m4")
set(FPU "-mfpu=fpv4-sp-d16")
set(FLOAT_ABI "-mfloat-abi=hard")
set(MCU "${CPU} -mthumb ${FPU} ${FLOAT_ABI}")

#--------------------------------------#
#          COMPILER FLAGS              #
#--------------------------------------#
set(CMAKE_C_FLAGS "${MCU} -Wall -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS_DEBUG "-Og -g -gdwarf-2")
set(CMAKE_C_FLAGS_RELEASE "-Os")

# Assembly flags
set(CMAKE_ASM_FLAGS "${MCU} -Wall -fdata-sections -ffunction-sections")

#--------------------------------------#
#          LINKER FLAGS                #
#--------------------------------------#
set(CMAKE_EXE_LINKER_FLAGS "${MCU} -specs=nano.specs -T${CMAKE_SOURCE_DIR}/tools/linker/STM32L431XX_FLASH.ld -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections")

# Use newlib-nano for smaller code size
target_link_libraries(${PROJECT_NAME} PRIVATE
    c
    m
    nosys
)


#--------------------------------------#
#             APPLICATION              #
#--------------------------------------#
set(SOURCES
    ${DIR_SRC}/main.c
    ${DIR_SRC}/debug-utils.c    
)

#--------------------------------------#
#               DRIVERS                #
#--------------------------------------#

# Raptor L431CBTx Board
list(APPEND SOURCES
    ${DIR_BOARD_RAPTOR_L431CBTx}/board_interface.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/startup/startup_stm32l431xx.s
    ${DIR_BOARD_RAPTOR_L431CBTx}/hal/stm32l4xx_hal_msp.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/hal/stm32l4xx_it.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/debug/usart.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/system/clock_config.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/system/error_handler.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/system/peripherals.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/system/syscalls.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/system/sysmem.c
    ${DIR_BOARD_RAPTOR_L431CBTx}/system/system_stm32l4xx.c 
)

#--------------------------------------#
#             STM32L4 HAL              #
#--------------------------------------#
list(APPEND SOURCES
    ${DIR_L4HAL}/Src/stm32l4xx_hal.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_gpio.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_rcc.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_uart.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_dma.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_flash.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_pwr.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_cortex.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_pwr_ex.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_adc.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_adc_ex.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_uart_ex.c
    ${DIR_L4HAL}/Src/stm32l4xx_hal_rcc_ex.c
)