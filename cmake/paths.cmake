#--------------------------------------#
#                 SRC                  #
#--------------------------------------#
set(DIR_SRC "${CMAKE_SOURCE_DIR}/src")

#--------------------------------------#
#               DRIVERS                #
#--------------------------------------#
set(DIR_DRIVERS "${CMAKE_SOURCE_DIR}/drivers")

#-------------- BOARDS --------------#
set(DIR_BOARD "${DIR_DRIVERS}/board")
set(DIR_BOARD_RAPTOR_L431CBTx "${DIR_BOARD}/Raptor_L431CBTx")


#------------- DISPLAYS -------------#
set(DIR_DISPLAY "${DIR_DRIVERS}/display")

#--------------------------------------#
#               EXTERNAL               #
#--------------------------------------#
set(DIR_EXTERNAL "${CMAKE_SOURCE_DIR}/external")
set(DIR_CUBEL4 "${DIR_EXTERNAL}/STM32CubeL4")

#--------------- CMSIS ----------------#
set(DIR_CMSIS "${DIR_CUBEL4}/CMSIS")
set(DIR_CMSIS_CORE "${DIR_CMSIS}/STM32CubeL4/CMSIS/Core")

#---------- L4xx_HAL_Driver -----------#
set(DIR_L4HAL "${DIR_CUBEL4}/STM32L4xx_HAL_Driver")
