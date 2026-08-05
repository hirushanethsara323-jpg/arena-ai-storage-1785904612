#include "ata.h"

static int ata_wait_bsy(void) {
    int timeout = 100000;
    while(timeout--) {
        uint8_t status = inb(ATA_PRIMARY_STATUS);
        if(!(status & ATA_STATUS_BSY)) return 0;
    }
    return -1; // timeout
}

static int ata_wait_drq(void) {
    int timeout = 100000;
    while(timeout--) {
        uint8_t status = inb(ATA_PRIMARY_STATUS);
        if(status & ATA_STATUS_ERR) return -1;
        if(status & ATA_STATUS_DRQ) return 0;
        if(status & ATA_STATUS_BSY) continue;
    }
    return -1;
}

void ata_init(void) {
    // Soft reset and check
    outb(ATA_PRIMARY_DRIVE, 0xA0);
    io_wait();
}

int ata_read_sector(uint32_t lba, uint8_t* buffer) {
    if(ata_wait_bsy() != 0) return -1;

    outb(ATA_PRIMARY_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_ERROR, 0x00);
    outb(ATA_PRIMARY_SECCOUNT, 1);
    outb(ATA_PRIMARY_LBA_LOW, lba & 0xFF);
    outb(ATA_PRIMARY_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_PRIMARY_COMMAND, 0x20); // READ SECTORS

    if(ata_wait_drq() != 0) return -1;

    for(int i=0;i<256;i++) {
        uint16_t data = inb(ATA_PRIMARY_DATA) | (inb(ATA_PRIMARY_DATA+1) << 8);
        ((uint16_t*)buffer)[i] = data;
        io_wait();
    }
    return 0;
}

int ata_write_sector(uint32_t lba, uint8_t* buffer) {
    if(ata_wait_bsy() != 0) return -1;

    outb(ATA_PRIMARY_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_SECCOUNT, 1);
    outb(ATA_PRIMARY_LBA_LOW, lba & 0xFF);
    outb(ATA_PRIMARY_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_PRIMARY_COMMAND, 0x30); // WRITE SECTORS

    if(ata_wait_drq() != 0) return -1;

    for(int i=0;i<256;i++) {
        uint16_t data = ((uint16_t*)buffer)[i];
        outb(ATA_PRIMARY_DATA, data & 0xFF);
        outb(ATA_PRIMARY_DATA+1, (data >> 8) & 0xFF);
        io_wait();
    }
    // flush cache
    outb(ATA_PRIMARY_COMMAND, 0xE7);
    ata_wait_bsy();
    return 0;
}

int ata_identify(void) {
    outb(ATA_PRIMARY_DRIVE, 0xA0);
    outb(ATA_PRIMARY_SECCOUNT, 0);
    outb(ATA_PRIMARY_LBA_LOW, 0);
    outb(ATA_PRIMARY_LBA_MID, 0);
    outb(ATA_PRIMARY_LBA_HIGH, 0);
    outb(ATA_PRIMARY_COMMAND, 0xEC); // IDENTIFY

    uint8_t status = inb(ATA_PRIMARY_STATUS);
    if(status == 0) return -1; // no drive

    while(1) {
        status = inb(ATA_PRIMARY_STATUS);
        if(status & ATA_STATUS_ERR) return -1;
        if(!(status & ATA_STATUS_BSY) && (status & ATA_STATUS_DRQ)) break;
    }
    // read 256 words to clear
    for(int i=0;i<256;i++) { inb(ATA_PRIMARY_DATA); inb(ATA_PRIMARY_DATA+1); }
    return 0;
}
