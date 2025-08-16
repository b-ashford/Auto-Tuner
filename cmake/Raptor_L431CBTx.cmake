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
    ${DIR_SRC}/autotune
    ${DIR_BOARD_RAPTOR_L431CBTx}
    ${DIR_BOARD_RAPTOR_L431CBTx}/debug
    ${DIR_BOARD_RAPTOR_L431CBTx}/system
    ${DIR_BOARD_RAPTOR_L431CBTx}/setup
    ${DIR_L4HAL}/Inc 
    ${DIR_CMSIS}/Device/ST/STM32L4xx/Include  
    ${DIR_CMSIS}/Include
    ${DIR_CMSIS}/DSP/Include
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
#set(CMAKE_C_FLAGS_RELEASE "-Os") # size optimisation
set(CMAKE_C_FLAGS_RELEASE "-O2") # speed optimisation

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
    ${DIR_CMSIS}/DSP/Lib/GCC/libarm_cortexM4lf_math.a
)

#--------------------------------------#
#             APPLICATION              #
#--------------------------------------#
file(GLOB APP_AUTOTUNE CONFIGURE_DEPENDS
    "${DIR_SRC}/autotune/*.c"
)

#--------------------------------------#
#               DRIVERS                #
#--------------------------------------#
set(BOARD_STARTUP "${DIR_BOARD_RAPTOR_L431CBTx}/system/startup_stm32l431cbtx.s")

file(GLOB BOARD_API CONFIGURE_DEPENDS
    "${DIR_BOARD_RAPTOR_L431CBTx}/device_api.c"
)
file(GLOB BOARD_SETUP CONFIGURE_DEPENDS
    "${DIR_BOARD_RAPTOR_L431CBTx}/setup/*.c"
)
file(GLOB BOARD_SYSTEM CONFIGURE_DEPENDS
    "${DIR_BOARD_RAPTOR_L431CBTx}/system/*.c"
)
file(GLOB BOARD_DEBUG CONFIGURE_DEPENDS
    "${DIR_BOARD_RAPTOR_L431CBTx}/debug/*.c"
)

#--------------------------------------#
#             STM32L4 HAL              #
#--------------------------------------#
file(GLOB HAL_SOURCES CONFIGURE_DEPENDS
    "${DIR_L4HAL}/Src/*.c"
)

#--------------------------------------#
#            SOURCE LIST                #
#--------------------------------------#
set(SOURCES
    ${DIR_SRC}/main.c
    ${APP_AUTOTUNE}
    ${BOARD_STARTUP}
    ${BOARD_API}
    ${BOARD_SETUP}
    ${BOARD_SYSTEM}
    ${BOARD_DEBUG}
    ${HAL_SOURCES}
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})