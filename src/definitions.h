/*
 * definitions.h
 * 
 * Copyright (c) 2021 Gustavo Vianna França
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  Created on: Dec 10, 2021
 *      Author: Gustavo Vianna França
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

// Neander and it's ALU operation codes
typedef enum OpType {
	ALU_NOP = 0,
	ALU_STA = 0,
	ALU_ADD = 1,
	ALU_AND,
	ALU_OR,
	ALU_NOT,
	ALU_LDA,
	ALU_JMP,
	ALU_JZ,
	ALU_JN = ALU_JZ,
	ALU_HLT = ALU_JZ,
	HAS_OP = 128,
	FLAG = 8,
	NOP = ALU_NOP << 4,
	NOT = ALU_NOT << 4,
	HLT = ALU_HLT << 4,
	STA = HAS_OP | (ALU_STA << 4),
	ADD = HAS_OP | (ALU_ADD << 4),
	AND = HAS_OP | (ALU_AND << 4),
	OR = HAS_OP | (ALU_OR << 4),
	LDA = HAS_OP | (ALU_LDA << 4),
	JMP = HAS_OP | (ALU_JMP << 4),
	JZ = HAS_OP | (ALU_JZ << 4),
	JN = HAS_OP | (ALU_JN << 4) | FLAG
} OpType;

// Neander's controller states
enum StateType {
	CS_FETCH_INST,
	CS_MEM_READ_INST,
	CS_DECODE_INST,
	CS_HLT,
	CS_LOAD_OP_ADDR,
	CS_LOAD_OP_DATA,
	CS_MEM_READ_DATA,
	CS_ALU_EXEC,
	CS_ALU_AC_WRITE,
	CS_MEM_WRITE,
	CS_JMP_EXEC
};

// Function to convert states to strings
static const char* StateString(StateType s) {
	static const char *strings[] = { "CS_FETCH_INST", "CS_MEM_READ_INST",
			"CS_DECODE_INST", "CS_HLT", "CS_LOAD_OP_ADDR", "CS_LOAD_OP_DATA",
			"CS_MEM_READ_DATA", "CS_ALU_EXEC", "CS_ALU_AC_WRITE",
			"CS_MEM_WRITE", "CS_JMP_EXEC" };

	return strings[s];
}

// Memory defines
#define MEMORY_WORD 8
#define MEMORY_SIZE 256

// Neander defines
#define NEANDER_TEXT 0
#define NEANDER_DATA 128

#endif /* DEFINITIONS_H_ */
