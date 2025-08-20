#=============================================================================#
# Board template (copy to boards/<BOARD>/<BOARD>.cmake and fill in)
# REQUIRED:  SRC_BOARD, INC_BOARD, FILE_LINKER
# OPTIONAL:  DEFS_BOARD, LIBS_BOARD, FILE_FLASH_CFG, MCU/flags
#=============================================================================#

#------------------ DEFINES (optional) ------------------#
set(DEFS_BOARD
    # e.g. MY_MCU_DEFINE
    # e.g. USE_HAL_DRIVER
)

#------------------ PATHS (optional helpers) ------------------#
set(DIR_BRD_ROOT   "${DIR_BOARDS}/${BOARD}")
# set other board-local dirs if you want, or use absolute paths below

#------------------ INCLUDES (REQUIRED) ------------------#
list(APPEND INC_BOARD
    # put include dirs here
    # e.g. ${DIR_BRD_ROOT}/include
)

#------------------ LIBS (optional) ------------------#
set(LIBS_BOARD
    # e.g. c m nosys <other-libs>
)

#------------------ SOURCES (REQUIRED) ------------------#
# (use explicit files or GLOB patterns as you prefer)
# file(GLOB BRD_SETUP   CONFIGURE_DEPENDS "<path>/setup/*.c")
# file(GLOB BRD_SYSTEM  CONFIGURE_DEPENDS "<path>/system/*.c")
# file(GLOB BRD_DEBUG   CONFIGURE_DEPENDS "<path>/debug/*.c")
# file(GLOB HAL_SOURCES CONFIGURE_DEPENDS "<path>/hal/*.c")

set(SRC_BOARD
    # e.g. ${DIR_BRD_ROOT}/system/startup_<mcu>.s
    # ${BRD_SETUP}
    # ${BRD_SYSTEM}
    # ${BRD_DEBUG}
    # ${HAL_SOURCES}
)

#------------------ LINKER/CFG (REQUIRED/optional) ------------------#
set(FILE_LINKER    "<path-to-linker-script>.ld")     # REQUIRED
set(FILE_FLASH_CFG "<path-to-openocd-cfg>.cfg")      # OPTIONAL (only if you add flash target)

#------------------ COMPILER/LINKER FLAGS (optional) ------------------#
# (Keep if you want board-global flags; otherwise move to top-level as target_*)
set(CPU        "-mcpu=<cortex-mX>")
set(FPU        "-mfpu=<fpvX-...>")
set(FLOAT_ABI  "-mfloat-abi=<hard|softfp|soft>")
set(MCU        "${CPU} -mthumb ${FPU} ${FLOAT_ABI}")

set(CMAKE_C_FLAGS         "${MCU} -Wall -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS_DEBUG   "-Og -g -gdwarf-2")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_ASM_FLAGS       "${MCU} -Wall -fdata-sections -ffunction-sections")

set(CMAKE_EXE_LINKER_FLAGS
    "${MCU} -specs=nano.specs -T${FILE_LINKER} -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections"
)

#------------------ FLASH TARGET (optional) ------------------#
# Define only if FILE_FLASH_CFG is set and you want a flash rule.
# add_custom_target(flash
#   COMMAND openocd -f "${FILE_FLASH_CFG}"
#                   -c "program $<TARGET_FILE:${PROJECT_NAME}> verify reset exit"
#   DEPENDS ${PROJECT_NAME}
#   USES_TERMINAL VERBATIM
# )