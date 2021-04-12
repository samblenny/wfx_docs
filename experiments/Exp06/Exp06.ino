#include "sl_wfx.h"

/**
 * Method to use wfx-fullMAC-driver as an Arduino library:
 * 1. Clone the driver:
 *    cd ~/Arduino
 *    git clone https://github.com/samblenny/wfx-fullMAC-driver.git --branch include-paths --depth 1
 * 2. Symlink (or copy) the wfx_fmac_driver directory into ~/Arduino/libraries/
 *    cd libraries
 *    ln -s ../wfx-fullMAC-driver/wfx_fmac_driver wfx_fmac_driver
 * 3. Copy wfx_fmac_driver/bus/* into wfx_fmac_driver/
 *    cd wfx_fmac_driver
 *    cp bus/* .
 * 4. If using SPI bus, remove the copy of the sl_wfx_bus_sdio files:
 *    # From wfx_fmac_driver/
 *    rm sl_wfx_bus_sdio*
 *
 * Steps 3-4 are a work-around to make wfx-fullMAC-driver link properly using
 * the Arduino 1.0.x IDE library spec. The old 1.0.x library spec does not
 * handle recursive compiliation in library subdirectories. But, modifying
 * wfx-fullMAC-driver to work with the Arduino 1.5.x library spec would
 * probably cause problems with using Silicon Laboratories Simplicity Studio.
 * See https://arduino.github.io/arduino-cli/latest/library-specification/
 */

extern "C" {
  sl_wfx_context_t context;
}

void setup() {
  sl_status_t status;
  Serial.begin(9600);
  while (!Serial) {;}
  status = sl_wfx_init(&context);
}

void loop() {
}

sl_status_t sl_wfx_host_init(void)
{
    Serial.print("init\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_get_firmware_data(const uint8_t **data, uint32_t data_size)
{
    Serial.print("get_firmware\n");
    Serial.flush();
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_firmware_size(uint32_t *firmware_size)
{
    Serial.print("get_firmware_size\n");
    Serial.flush();
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_pds_data(const char **pds_data, uint16_t index)
{
    Serial.print("get_pds_data\n");
    Serial.flush();
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_pds_size(uint16_t *pds_size)
{
    Serial.print("get_pds_size\n");
    Serial.flush();
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_deinit(void)
{
    Serial.print("deinit\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_reset_chip(void)
{
    Serial.print("reset_chip\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_set_wake_up_pin(uint8_t state)
{
    Serial.print("set_wake_up_pin\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait_for_wake_up(void)
{
    Serial.print("wait_for_wake_up\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_sleep_grant(
    sl_wfx_host_bus_transfer_type_t type,
    sl_wfx_register_address_t address,
    uint32_t length)
{
    Serial.print("sleep_grant\n");
    Serial.flush();
    return SL_STATUS_WIFI_SLEEP_NOT_GRANTED;
}

sl_status_t sl_wfx_host_setup_waited_event(uint8_t event_id)
{
    Serial.print("setup_waited_event\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait_for_confirmation(
    uint8_t confirmation_id,
    uint32_t timeout_ms,
    void **event_payload_out)
{
    Serial.print("wait_for_confirmation\n");
    Serial.flush();
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_wait(uint32_t wait_ms)
{
    Serial.print("wait ");
    Serial.print(wait_ms, DEC);
    Serial.print("ms; ");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_post_event(sl_wfx_generic_message_t *event_payload)
{
    Serial.print("post_event\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_allocate_buffer(
    void **buffer,
    sl_wfx_buffer_type_t type,
    uint32_t buffer_size)
{
    Serial.print("allocate_buffer\n");
    Serial.flush();
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_free_buffer(void *buffer, sl_wfx_buffer_type_t type)
{
    Serial.print("free_buffer\n");
    Serial.flush();
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_transmit_frame(void *frame, uint32_t frame_len)
{
    Serial.print("transmit_frame\n");
    Serial.flush();
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_lock(void)
{
    Serial.print("lock\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_unlock(void)
{
    Serial.print("unlock\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_init_bus(void)
{
    Serial.print("init_bus\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_deinit_bus(void)
{
    Serial.print("deinit_bus\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_enable_platform_interrupt(void)
{
    Serial.print("irq=on\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_disable_platform_interrupt(void)
{
    Serial.print("irq=off\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_assert(void)
{
    Serial.print("cs=on; ");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_deassert(void)
{
    Serial.print("cs=off\n");
    Serial.flush();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_transfer_no_cs_assert(
    sl_wfx_host_bus_transfer_type_t type,
    uint8_t *header,
    uint16_t header_length,
    uint8_t *buffer,
    uint16_t buffer_length)
{
    // Debug print the raw byte array arguments
    Serial.print("spi_transfer 0x");
    for(int i=0; i < header_length; i++) {
        if (header[i] < 0x10) {
          Serial.print("0");
        }
        Serial.print(header[i], HEX);
        Serial.flush();
    }
    Serial.print(" 0x");
    for(int i=0; i < buffer_length; i++) {
        if (buffer[i] < 0x10) {
          Serial.print("0");
        }
        Serial.print(buffer[i], HEX);
        Serial.flush();
    }
    // All transfers should start with a 16-bit command word (the header), and
    // they often have a transfer length of two 16-bit words, (buffer_length ==
    // 4). The order in which bytes get arranged in the data buffer varries
    // based on the word mode.
    //
    // This attempts to interpret what was transferred.
    if(header_length == 2 && buffer_length == 4) {
        uint16_t cmd = ((uint16_t)(header[0]))<<8 | (uint16_t)(header[1]);
        char cmd_rw = (cmd>>15 == 1) ? 'R' : 'W';
        uint16_t cmd_addr = cmd>>12 & 0x7;
        uint16_t cmd_len = cmd & 0x0fff;
        uint32_t buf = 0;
        if(cmd_addr == SL_WFX_CONFIG_REG_ID) {
            // Config reg at address 0 always uses word Mode0 (B1,B0,B3,B2)
            buf = ((uint32_t)(buffer[2]))<<24 | ((uint32_t)(buffer[3]))<<16;
            buf |= ((uint32_t)(buffer[0]))<<8 | (uint32_t)(buffer[1]);
        } else {
            // sl_wfx_bus_spi.c:sl_wfx_init_bus() sets word Mode2, which means
            // bytes get transferred in little endian order: B0,B1,B2,B3
            buf = ((uint32_t)(buffer[3]))<<24 | ((uint32_t)(buffer[2]))<<16;
            buf |= ((uint32_t)(buffer[1]))<<8 | (uint32_t)(buffer[0]);
        }
        Serial.print(" -> ");
        Serial.print(cmd_rw);
        Serial.print("(addr:");
        Serial.print(cmd_addr, HEX);
        Serial.flush();
        Serial.print(",len:");
        Serial.print(cmd_len, HEX);
        Serial.flush();
        Serial.print(",buf:");
        if (buf < 0x10) {
          Serial.print("0000000");
        } else if (buf < 0x100) {
          Serial.print("000000");
        } else if (buf < 0x1000) {
          Serial.print("00000");
        } else if (buf < 0x10000) {
          Serial.print("0000");
        } else if (buf < 0x100000) {
          Serial.print("000");
        } else if (buf < 0x1000000) {
          Serial.print("00");
        } else if (buf < 0x1000000) {
          Serial.print("0");
        }
        Serial.print(buf, HEX);
        Serial.print(")");
        Serial.flush();
        bool addr_is_control_reg = cmd_addr == SL_WFX_CONTROL_REG_ID;
        if(addr_is_control_reg) {
            // When sl_wfx_init_chip() writes control with WUP bit set, it
            // expects to see responses with RDY set, or else it goes into a
            // big polling loop. So, claim to be always RDY.
            buf |= SL_WFX_CONT_RDY_BIT;
            // Re-pack buffer (Mode2) with the output value of buf
            buffer[3] = (uint8_t)(buf>>24);
            buffer[2] = (uint8_t)((buf<<8)>>24);
            buffer[1] = (uint8_t)((buf<<16)>>24);
            buffer[0] = (uint8_t)((buf<<24)>>24);
            // Debug print the packed output value of buffer
            Serial.print(" 0x");
            for(int i=0; i < buffer_length; i++) {
              if (buffer[i] < 0x10) {
                Serial.print("0");
              }
              Serial.print(buffer[i], HEX);
              Serial.flush();
            }
        }
    }
    Serial.print("; ");
    Serial.flush();
    return SL_STATUS_OK;
}
