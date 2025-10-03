#ifndef _I2C_H
#define _I2C_H

#include <avr/io.h>
#include <util/twi.h>

#define I2C_OK (0)
#define I2C_ERR_BAD_SLA (0xd0)
#define I2C_ERR_BAD_RW (0xd1)

// initializes the spi interface
void i2c_init(void);

// send a start condition over i2c bus
void i2c_start(void);

// send a stop condition over i2c bus
void i2c_stop(void);

// checks the TWI status register for the status by masking the prescaler bits
// returns the status of an action with prescaler bits masked
uint8_t i2c_get_status(void);

// reads a byte of data from the bus expecting an ack
uint8_t i2c_read_ack();

// reads a byte of data from the bus without expecting an ack
uint8_t i2c_read_nack();

// send data to a previously selected address
void i2c_send_data(uint8_t data);

// addresses a slave at address "sla" with a r/w bit from "rw"
// returns 0 for success and non-zero for error
uint8_t i2c_sla_tx(uint8_t sla, uint8_t rw);

// waits on the TWINT to be set;
// useful for waiting on data transfer to complete or ack has been received
void i2c_wait_twint(void);

// sets an address for device and returns 0 for sucess and non-zero for failure
uint8_t i2c_set_address(uint8_t addr);

#endif // _I2C_H
