#pragma once

#include <array>
#include <cstdint>

#include "sim_i2c_base.hpp"

// This class represents a simulated external EEPROM communicating with the main
// MCU via i2c.
class I2cEeprom : public SimAvrI2CComponent {
 public:
  I2cEeprom(avr_t *avr, bool a0, bool a1);

 private:
  // Handle a message fragment.
  virtual void HandleI2CMessage(avr_twi_msg_t msg) override;

  // Reset the state of this device.
  virtual void Reset() override;

  // Storage for the data in this EEPROM.
  std::array<uint8_t, 4096> buffer_;

  // The current state of the EEPROM. Since device addressing for the 22LC512
  // takes three bytes (and therefore three messages), we need to keep track of
  // the state of the device as it progresses through the device addressing
  // sequence.
  enum EepromState {
    STOPPED,
    ADDRESSING_HIGH,
    ADDRESSING_LOW,
    STARTED,
  };
  EepromState state_;

  // The read/write flag is sent in the control byte, and sets the mode of the
  // device for subsequent messages. We need to persist it for the duration of
  // the operation.
  I2CMode mode_;

  // The address of the current operation. This is only fully valid when
  // state_ == STARTED.
  uint16_t operation_address_;

  // Maintain the address of the last word accessed.
  uint8_t operation_address_counter_;
};