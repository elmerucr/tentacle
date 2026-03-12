#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <tentacle.h>

uint8_t *ram;

uint8_t read8(uint32_t address);
uint32_t read32(uint32_t address);
void write8(uint32_t address, uint8_t value);
void write32(uint32_t address, uint32_t value);

int main()
{
	ram = (uint8_t *)malloc(0x4000000); // 64mb

	tentacle_t cpu;
	tentacle_reset(&cpu);

	write32(0, 0xdeadbeef);

	printf("%02x\n", read8(0));
	printf("%02x\n", read8(1));
	printf("%02x\n", read8(2));
	printf("%02x\n", read8(3));


	free(ram);
	return 0;
}

uint8_t read8(uint32_t address)
{
	return ram[address & 0x3ffffff];
}

uint32_t read32(uint32_t address)
{
	address &= 0x3fffffc;
	return
		(ram[address + 3] << 24) |
		(ram[address + 2] << 16) |
		(ram[address + 1] <<  8) |
		(ram[address + 0] <<  0) ;
}

void write8(uint32_t address, uint8_t value)
{
	ram[address & 0x3ffffff] = value;
}

void write32(uint32_t address, uint32_t value)
{
	ram[(address & 0x3fffffc) + 0] = (value >>  0) & 0xff;
	ram[(address & 0x3fffffc) + 1] = (value >>  8) & 0xff;
	ram[(address & 0x3fffffc) + 2] = (value >> 16) & 0xff;
	ram[(address & 0x3fffffc) + 3] = (value >> 24) & 0xff;
}
