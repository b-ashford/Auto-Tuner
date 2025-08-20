#--------------------------------------#
#               DEFINES                #
#--------------------------------------#
set(DEFS_BOARD
    STM32L431xx
    USE_HAL_DRIVER
)
# BOARD PATHS
set(DIR_BRD_ROOT   ${DIR_BOARDS}/Raptor_L431CBTx)
set(DIR_BRD_LINKER ${DIR_BRD_ROOT}/linker)
set(DIR_BRD_DEBUG  ${DIR_BRD_ROOT}/debug)
set(DIR_BRD_SETUP  ${DIR_BRD_ROOT}/setup)
set(DIR_BRD_SYS    ${DIR_BRD_ROOT}/system)
set(FILE_LINKER    ${DIR_BRD_LINKER}/STM32L431XX_FLASH.ld)
set(FILE_FLASH_CFG ${DIR_BRD_ROOT}/Raptor_L431CBTx.cfg)
set(LIB_CMSIS_MATH ${DIR_CMSIS}/DSP/Lib/GCC/libarm_cortexM4lf_math.a)


#--------------------------------------#
#               SOURCES                #
#--------------------------------------#
# INCLUDES
list(APPEND INC_BOARD
    ${DIR_BRD_ROOT}
    ${DIR_BRD_DEBUG}
    ${DIR_BRD_SYS}
    ${DIR_BRD_SETUP}
    ${DIR_L4HAL}/Inc
    ${DIR_CMSIS}/Device/ST/STM32L4xx/Include
    ${DIR_CMSIS}/Include
    ${DIR_CMSIS}/DSP/Include
)

# LIBS
set(LIBS_BOARD c m nosys ${LIB_CMSIS_MATH})

# DRIVERS 
set(BOARD_STARTUP ${DIR_BRD_ROOT}/system/startup_stm32l431cbtx.s)
file(GLOB BOARD_API     CONFIGURE_DEPENDS "${DIR_BRD_ROOT}/board_api.c")
file(GLOB BOARD_SETUP   CONFIGURE_DEPENDS "${DIR_BRD_ROOT}/setup/*.c")
file(GLOB BOARD_SYSTEM  CONFIGURE_DEPENDS "${DIR_BRD_ROOT}/system/*.c")
file(GLOB BOARD_DEBUG   CONFIGURE_DEPENDS "${DIR_BRD_ROOT}/debug/*.c")

# HAL
file(GLOB HAL_SOURCES CONFIGURE_DEPENDS "${DIR_L4HAL}/Src/*.c")

set(SRC_BOARD
    ${BOARD_STARTUP}
    ${BOARD_API}
    ${BOARD_SETUP}
    ${BOARD_SYSTEM}
    ${BOARD_DEBUG}
    ${HAL_SOURCES}
)

#--------------------------------------#
#               CONFIGS                #
#--------------------------------------#
# MCU FLAGS
set(CPU "-mcpu=cortex-m4")
set(FPU "-mfpu=fpv4-sp-d16")
set(FLOAT_ABI "-mfloat-abi=hard")
set(MCU "${CPU} -mthumb ${FPU} ${FLOAT_ABI}")

# COMPILER FLAGS (you can switch to target_compile_options later)
set(CMAKE_C_FLAGS "${MCU} -Wall -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS_DEBUG "-Og -g -gdwarf-2")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_ASM_FLAGS "${MCU} -Wall -fdata-sections -ffunction-sections")

# LINKER FLAGS (or use target_link_options)
set(CMAKE_EXE_LINKER_FLAGS
    "${MCU} -specs=nano.specs -T${FILE_LINKER} -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections"
)

#--------------------------------------#
#             FLASH BOARD              #
#--------------------------------------#
add_custom_target(flash
  COMMAND openocd -f "${FILE_FLASH_CFG}"
                  -c "program $<TARGET_FILE:${PROJECT_NAME}> verify reset exit"
  DEPENDS ${PROJECT_NAME}
  USES_TERMINAL VERBATIM
)