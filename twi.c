#include "rush_header.h"

void TWI_init_master() {
    TWSR = TWI_PRESCALE_SET(TWI_PRESCALE_VALUE);					// 22.9.3 : (Status Register) set prescaler
    TWBR = ((F_CPU / TWI_FREQ) - 16) / (2 * TWI_PRESCALE_VALUE);	// 22.9.1 : (Bit Rate Register) set SCL frequency (formula from datasheet, 22.5.2)
}

void TWI_start() {
    TWCR = SEND_START_CONDITION;									// 22.9.2 : (Control Register) send Start condition (22.7.1) ! writting 1 to TWINT clears it (set it to 0)
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the START condition has been transmitted

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// p225 example code : Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
    if (status != TW_START && status != TW_REP_START) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

void TWI_write_addr(uint8_t addr_w) {
    TWDR = addr_w;													// 22.9.4 : (Data Register) load data into TWDR register
    TWCR = SEND_CONTINUE_TRANSMISSION;								// p225 example code : Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
    if (status != TW_MT_SLA_ACK) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

void TWI_write(uint8_t data) {
    TWDR = data;													// 22.9.4 : (Data Register) load data into TWDR register
    TWCR = SEND_CONTINUE_TRANSMISSION;								// p225 example code : Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// p225 example code : Check value of TWI Status Register. Mask prescaler bits. If status different from MT_DATA_ACK go to ERROR
    if (status != TW_MT_DATA_ACK) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

void TWI_stop() {
    TWCR = SEND_STOP_CONDITION;
}

void send_one_byte_data(uint8_t data) {
    TWI_start();
    TWI_write_addr((SLAVE_ADDRESS << 1) | TW_WRITE);				// Send Slave address with Write bit
    TWI_write(data);												// Send data byte
    TWI_stop();
}