#ifndef TENTACLE_H
#define TENTACLE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TENTACLE_MAJOR_VERSION  0
#define TENTACLE_MINOR_VERSION  0
#define TENTACLE_RELEASE_DATE   20260314

typedef uint8_t (*tentacle_read8)(uint32_t addr);
typedef uint32_t (*tentacle_read32)(uint32_t addr);
typedef void(*tentacle_write8)(uint32_t address, uint8_t value);
typedef void(*tentacle_write32)(uint32_t address, uint32_t value);

// Page 5-2
enum processor_modes {
	USR_MODE = 0b00,
	FIQ_MODE = 0b01,
	IRQ_MODE = 0b10,
	SVC_MODE = 0b11
};

enum register_names {
	R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
	R8_FIQ, R9_FIQ, R10_FIQ, R11_FIQ, R12_FIQ, R13_FIQ, R14_FIQ,
	R13_IRQ, R14_IRQ,
	R13_SVC, R14_SVC
};

typedef struct {
	uint32_t reg[27];

    uint32_t pc;    // for convenience
    bool branch;

	//uint8_t idle;

	bool fetch_valid;
	bool decode_valid;
	bool execute_valid;

	uint32_t fetch;
	uint32_t decode;
	uint32_t execute;

	tentacle_read8 r8;
	tentacle_read32 r32;
	tentacle_write8 w8;
	tentacle_write32 w32;

} tentacle_t;

tentacle_t *tentacle_new(tentacle_read8 tr8, tentacle_read32 tr32, tentacle_write8 tw8, tentacle_write32 tw32);
void tentacle_destroy(tentacle_t *t);
void tentacle_reset(tentacle_t *t);
void tentacle_tick(tentacle_t *t);
void tentacle_execute(tentacle_t *t);

#ifdef __cplusplus
}
#endif

#endif
