#include <tentacle.h>
#include <stdlib.h>
#include <stdio.h>

// See page 2-4
const unsigned int reg_index[16][4] = {
	{ R0,  R0,      R0,      R0      },
	{ R1,  R1,      R1,      R1      },
	{ R2,  R2,      R2,      R2      },
	{ R3,  R3,      R3,      R3      },
	{ R4,  R4,      R4,      R4      },
	{ R5,  R5,      R5,      R5      },
	{ R6,  R6,      R6,      R6      },
	{ R7,  R7,      R7,      R7      },
	{ R8,  R8_FIQ,  R8,      R8      },
	{ R9,  R9_FIQ,  R9,      R9      },
	{ R10, R10_FIQ, R10,     R10     },
	{ R11, R11_FIQ, R11,     R11     },
	{ R12, R12_FIQ, R12,     R12     },
	{ R13, R13_FIQ, R13_IRQ, R13_SVC },
	{ R14, R14_FIQ, R14_IRQ, R14_SVC },
	{ R15, R15,     R15,     R15     }
};

tentacle_t *tentacle_new(tentacle_read8 tr8, tentacle_read32 tr32, tentacle_write8 tw8, tentacle_write32 tw32)
{
	tentacle_t *t = (tentacle_t *)malloc(sizeof(tentacle_t));
	t->r8 = tr8;
	t->r32 = tr32;
	t->w8 = tw8;
	t->w32 = tw32;

	return t;
}

void tentacle_destroy(tentacle_t *t)
{
	free(t);
}

void tentacle_reset(tentacle_t *t)
{
	// NZCVIF'ProgramCounter'M1:M0
	t->reg[R15] = 0b000011'000000000000000000000000'11; // 0x0c000003 in hex
	// I and F bits set to prevent interrupts, processor in SVC mode (supervisor)

	t->execute_valid = false;
	t->decode_valid = false;
	t->fetch_valid = false;

	t->idle = 4;
}

void tentacle_tick(tentacle_t *t)
{
	if (t->idle) {
		t->idle--;
		return;
	}

	/*
	 * Execute
	 */
	if (t->execute_valid) {
		// execute t->decode
		// if b or bl, invalidate decode and fetch
	}
	t->execute = t->decode;
	t->execute_valid = t->decode_valid;

	/*
	 * Decode
	 */
	t->decode = t->fetch;
	t->decode_valid = t->fetch_valid;

	/*
	 * Fetch
	 */
	uint32_t pc = t->reg[15] & 0x03fffffc;
	t->fetch = t->r32(pc);
	t->fetch_valid = true;

	// Increase pc (R15)
	t->reg[R15] =	(t->reg[R15] & 0b111111'00000000'00000000'00000000'11) |
					((pc + 4) & 0x03fffffc);

	printf("%08x\n", t->fetch); // hack
}
