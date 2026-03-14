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
	t->reg[R15] = 0x0c000003;
    // 0b000011'000000000000000000000000'11 in binary, NZCVIF'ProgramCounter'M1:M0
	// I and F bits set to prevent interrupts, processor in SVC mode (supervisor)
    // all other registers are undefined

	t->execute_valid = false;
	t->decode_valid = false;
	t->fetch_valid = false;

	//t->idle = 4;
}

void tentacle_tick(tentacle_t *t)
{
	// if (t->idle) {
	// 	t->idle--;
	// 	return;
	// }

	t->pc = t->reg[15] & 0x03fffffc;

	// Fetch
	t->fetch = t->r32(t->pc);
    t->fetch_valid = true;
	printf("pc:%08x  fetched:%08x  ", t->pc, t->fetch); // HACK!

	// Decode
	if (t->decode_valid) {
        // yes, decode... does this actually do something in an emulator?
        printf("decode:%08x  ", t->decode);
    } else {
        printf("decode:--------  ");
    }

    // reset branch check
    t->branch = false;

	// Execute
	if (t->execute_valid) {
        printf("execute: %08x\n", t->execute);
        tentacle_execute(t);
		// if b or bl, invalidate decode and fetch
	} else {
        printf("execute: --------\n");
    }

    // only if not branch instr!
    if (t->branch == false) {
	    t->reg[R15] = (t->reg[R15] & 0xfc000003) | ((t->pc + 4) & 0x03fffffc);
    }

    // Move everything in the pipeline
	t->execute = t->decode;
	t->execute_valid = t->decode_valid;

    t->decode = t->fetch;
    t->decode_valid = t->fetch_valid;
}

void tentacle_execute(tentacle_t *t)
{
    if (((t->execute & 0x0e000000) >> 25) == 0b101) {
        t->branch = true;
        t->pc += ((t->execute & 0xffffff) << 2);
	    t->reg[R15] = (t->reg[R15] & 0xfc000003) | (t->pc & 0x03fffffc);
        t->fetch_valid = false;
        t->decode_valid = false;
    }
}
