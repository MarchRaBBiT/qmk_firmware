#include "ch.h"
#include "hal.h"
#include "quantum.h"
#include "cat24m01.h"
#include "mcp23017.h"

I2CDriver i2c_drv;
I2CConfig i2c_cfg = {
    STM32_TIMINGR_PRESC(15U) |
    STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
    STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
  0,
  0
};

void keyboard_pre_init_kb(void)
{
    expander_init(&I2CD1, &i2c_cfg);
    keyboard_pre_init_user();
}

#ifdef RAW_ENABLE
#include <raw_hid.h>
void raw_hid_receive( uint8_t *data, uint8_t length ) {
	uint8_t *command_id = &(data[0]);
	uint8_t *command_data = &(data[1]);
	switch ( *command_id )
	{
		case id_get_protocol_version:
		{
			command_data[0] = PROTOCOL_VERSION >> 8;
			command_data[1] = PROTOCOL_VERSION & 0xFF;
			break;
		}
		case id_get_keyboard_value:
		{
      switch(command_data[0])
      {
        case id_uptime:
        {
          uint32_t value = timer_read32();
          command_data[1] = (value >> 24 ) & 0xFF;
          command_data[2] = (value >> 16 ) & 0xFF;
          command_data[3] = (value >> 8 ) & 0xFF;
          command_data[4] = value & 0xFF;
          break;
        }
        case id_oled_default_mode:
        {
          uint8_t default_oled = eeprom_read_byte((uint8_t*)DYNAMIC_KEYMAP_DEFAULT_OLED);
          command_data[1] = default_oled;
          break;
        }
        case id_oled_mode:
        {
          command_data[1] = oled_mode;
          break;

        }
        case id_encoder_modes:
        {
          command_data[1] = enabled_encoder_modes;
          break;
        }
        case id_encoder_custom:
        {
          // uint8_t custom_encoder_idx = command_data[1];
          // command_data[2] = 0x00;
          // command_data[3] = 0x00;
          // command_data[4] = 0x00;
          // command_data[5] = 0x00;
          // command_data[6] = 0x00;
          // command_data[7] = 0x00;
          break;
        }
        default:
        {
          *command_id = id_unhandled;
          break;
        }
      }
			break;
    }
#ifdef DYNAMIC_KEYMAP_ENABLE
    case id_set_keyboard_value:
    {
      switch(command_data[0]){
        case id_oled_default_mode:
        {
          eeprom_update_byte((uint8_t*)DYNAMIC_KEYMAP_DEFAULT_OLED, command_data[1]);
          break;
        }
        case id_oled_mode:
        {
          oled_mode = command_data[1];
          draw_ui();
          break;
        }
        case id_encoder_modes:
        {
          enabled_encoder_modes = command_data[1];
          eeprom_update_byte((uint8_t*)DYNAMIC_KEYMAP_ENABLED_ENCODER_MODES, enabled_encoder_modes);
          break;
        }
        case id_encoder_custom:
        {
          // uint8_t custom_encoder_idx = command_data[1];
          break;
        }
        default:
        {
          *command_id = id_unhandled;
          break;
        }
      }
      break;
    }
		case id_dynamic_keymap_get_keycode:
		{
			uint16_t keycode = dynamic_keymap_get_keycode( command_data[0], command_data[1], command_data[2] );
			command_data[3] = keycode >> 8;
			command_data[4] = keycode & 0xFF;
			break;
		}
		case id_dynamic_keymap_set_keycode:
		{
			dynamic_keymap_set_keycode( command_data[0], command_data[1], command_data[2], ( command_data[3] << 8 ) | command_data[4] );
			break;
		}
		case id_dynamic_keymap_reset:
		{
			dynamic_keymap_reset();
			break;
		}
		case id_dynamic_keymap_macro_get_count:
		{
			command_data[0] = dynamic_keymap_macro_get_count();
			break;
		}
		case id_dynamic_keymap_macro_get_buffer_size:
		{
			uint16_t size = dynamic_keymap_macro_get_buffer_size();
			command_data[0] = size >> 8;
			command_data[1] = size & 0xFF;
			break;
		}
		case id_dynamic_keymap_macro_get_buffer:
		{
			uint16_t offset = ( command_data[0] << 8 ) | command_data[1];
			uint16_t size = command_data[2]; // size <= 28
			dynamic_keymap_macro_get_buffer( offset, size, &command_data[3] );
			break;
		}
		case id_dynamic_keymap_macro_set_buffer:
		{
			uint16_t offset = ( command_data[0] << 8 ) | command_data[1];
			uint16_t size = command_data[2]; // size <= 28
			dynamic_keymap_macro_set_buffer( offset, size, &command_data[3] );
			break;
		}
		case id_dynamic_keymap_macro_reset:
		{
			dynamic_keymap_macro_reset();
			break;
		}
		case id_dynamic_keymap_get_layer_count:
		{
			command_data[0] = dynamic_keymap_get_layer_count();
			break;
		}
		case id_dynamic_keymap_get_buffer:
		{
			uint16_t offset = ( command_data[0] << 8 ) | command_data[1];
			uint16_t size = command_data[2]; // size <= 28
			dynamic_keymap_get_buffer( offset, size, &command_data[3] );
			break;
		}
		case id_dynamic_keymap_set_buffer:
		{
			uint16_t offset = ( command_data[0] << 8 ) | command_data[1];
			uint16_t size = command_data[2]; // size <= 28
			dynamic_keymap_set_buffer( offset, size, &command_data[3] );
			break;
		}
#endif // DYNAMIC_KEYMAP_ENABLE
		case id_eeprom_reset:
		{
			eeprom_reset();
			break;
		}
		case id_bootloader_jump:
		{
			// Need to send data back before the jump
			// Informs host that the command is handled
			raw_hid_send( data, length );
			// Give host time to read it
			wait_ms(100);
			bootloader_jump();
			break;
		}
		default:
		{
			// Unhandled message.
			*command_id = id_unhandled;
			break;
		}
	}

	// Return same buffer with values changed
	raw_hid_send( data, length );

}

#endif
