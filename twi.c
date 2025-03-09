#include "rush_header.h"

volatile uint8_t received_data = 0;

void twi_init_slave(void) {
    TWAR = (SLAVE_ADDRESS << 1);
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);					// Enable TWI, enable ACK, and enable TWI interrupt
    SREG |= (1 << SREG_I);
}
void twi_stop_slave(void) {
    // TWCR = 0x00;													// Disable TWI interrupts and TWI module
    // TWAR = 0x00;													// Optionally, clear the TWI address register
}

void twi_init_master() {
    TWSR = TWI_PRESCALE_SET(TWI_PRESCALE_VALUE);					// 22.9.3 : (Status Register) set prescaler
    TWBR = ((F_CPU / TWI_FREQ) - 16) / (2 * TWI_PRESCALE_VALUE);	// 22.9.1 : (Bit Rate Register) set SCL frequency (formula from datasheet, 22.5.2)
}

void twi_start() {
    TWCR = SEND_START_CONDITION;									// 22.9.2 : (Control Register) send Start condition (22.7.1) ! writting 1 to TWINT clears it (set it to 0)
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the START condition has been transmitted

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// p225 example code : Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
    if (status != TW_START && status != TW_REP_START) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

void twi_send_addr(uint8_t addr_r_w) {
    TWDR = addr_r_w;												// 22.9.4 : (Data Register) load data into TWDR register
    TWCR = SEND_CONTINUE_TRANSMISSION;								// p225 example code : Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
    if (status != TW_MT_SLA_ACK) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

void twi_write(uint8_t data) {
    TWDR = data;													// 22.9.4 : (Data Register) load data into TWDR register
    TWCR = SEND_CONTINUE_TRANSMISSION;								// p225 example code : Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// p225 example code : Check value of TWI Status Register. Mask prescaler bits. If status different from MT_DATA_ACK go to ERROR
    if (status != TW_MT_DATA_ACK) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

uint8_t twi_read(uint8_t ack) {
    if (ack) {
        TWCR = SEND_ACKNOWLEDGE;                                    // Send ACK to request more data
    } else {
        TWCR = SEND_NACKNOWLEDGE;                                   // Send NACK to indicate last byte read
    }
    while (!(TEST(TWCR, TWINT)));                                   // Wait for data reception

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;
    if (status != TW_MR_DATA_ACK && status != TW_MR_DATA_NACK) {
        TWCR = SEND_STOP_CONDITION;
        return 0;
    }
    return TWDR;
}

void twi_stop() {
    TWCR = SEND_STOP_CONDITION;
}

void write_one_byte_data(uint8_t data) {
    twi_start();
    twi_send_addr((SLAVE_ADDRESS << 1) | TW_WRITE);				// Send Slave address with Write bit
    twi_write(data);											// write data byte
    twi_stop();
}
uint8_t read_one_byte_data() {
    twi_start();
    twi_send_addr((SLAVE_ADDRESS << 1) | TW_READ);				// Send Slave address with Write bit
    uint8_t data = twi_read(TRUE);                                 // Read data with NACK (only one byte expected)
    twi_stop();
    return data;
}