#include <tentacle.h>

// See page 2-4
const unsigned int reg_index[16][4] = {
	{ R0,      R0,      R0,      R0      },
	{ R1,      R1,      R1,      R1      },
	{ R2,      R2,      R2,      R2      },
	{ R3,      R3,      R3,      R3      },
	{ R4,      R4,      R4,      R4      },
	{ R5,      R5,      R5,      R5      },
	{ R6,      R6,      R6,      R6      },
	{ R7,      R7,      R7,      R7      },
	{ R8,      R8_FIQ,  R8,      R8      },
	{ R9,      R9_FIQ,  R9,      R9      },
	{ R10,     R10_FIQ, R10,     R10     },
	{ R11,     R11_FIQ, R11,     R11     },
	{ R12,     R12_FIQ, R12,     R12     },
	{ R13,     R13_FIQ, R13_IRQ, R13_SVC },
	{ R14,     R14_FIQ, R14_IRQ, R14_SVC },
	{ R15,     R15,     R15,     R15     }
};

void tentacle_reset(tentacle_t *t)
{
	// NZCVIF'ProgramCounter'M1:M0
	t->reg[R15] = 0b000011'000000000000000000000000'11; // 0x0c000003 in hex
	// I and F bits set to prevent interrupts, processor in SVC mode (supervisor)
	t->wait = 4;
}

void tentacle_step(tentacle_t *t)
{
	t->wait--;

	if (t->wait > 2) {
		return;
	}

	t->execute = t->decode;
	if (!t->wait) {
		// execute instruction!
	}

	t->decode = t->fetch;

	t->fetch = t->reg[R15] & 0b000000'11111111'11111111'11111111'00;
	t->reg[R15] =	(t->reg[R15] & 0b111111'00000000'00000000'00000000'11) |
					((t->fetch + 0b100) & 0b000000'11111111'11111111'11111111'00);
}
