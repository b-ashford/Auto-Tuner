#--------------------------------------#
#             DIRECTORIES              #
#--------------------------------------#
set(DIR_ROOT ${CMAKE_SOURCE_DIR})
set(DIR_BUILD ${DIR_ROOT}/build)

# CMake
set(DIR_CMAKE ${DIR_ROOT}/cmake)

# Application
set(DIR_APP ${DIR_ROOT}/app)
set(DIR_APP_AUTOTUNE ${DIR_APP}/autotune)
#set(DIR_APP_UTILITIES ${DIR_APP}/utilities)

# Drivers
set(DIR_HW ${DIR_ROOT}/hw)
set(DIR_BOARDS ${DIR_HW}/boards)
set(DIR_DISPLAY ${DIR_HW}/display)

# External libraries
set(DIR_EXT ${DIR_ROOT}/external)
set(DIR_CUBEL4 ${DIR_EXT}/STM32CubeL4)
set(DIR_CMSIS ${DIR_CUBEL4}/CMSIS)
set(DIR_CMSIS_CORE ${DIR_CMSIS}/Core)
set(DIR_L4HAL ${DIR_CUBEL4}/STM32L4xx_HAL_Driver)

#--------------------------------------#
#              TOOLCHAINS              #
#--------------------------------------#
set(TOOLCHAIN_ARM_NONE_EABI "${DIR_CMAKE}/arm-none-eabi-toolchain.cmake")

#--------------------------------------#
#           SUPPORTED BOARDS           #
#--------------------------------------#
set(BOARD_RAPTOR "Raptor_L431CBTx")