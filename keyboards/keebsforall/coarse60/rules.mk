# MCU name
MCU = STM32F072

# Bootloader selection
BOOTLOADER = stm32-dfu

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes        # Console for debug
COMMAND_ENABLE = yes        # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = yes      # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = yes       # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output
WS2812_DRIVER = spi
ENCODER_ENABLE = no
TAP_DANCE_ENABLE = no
LEADER_ENABLE = yes

# Enter lower-power sleep mode when on the ChibiOS idle thread
# OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

LAYOUTS = alice alice_split_bs
