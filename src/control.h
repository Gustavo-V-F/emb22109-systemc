#ifndef CONTROL_H_
#define CONTROL_H_

#include <iostream>
#include <systemc.h>
#include "reg.h"
#include "alu.h"

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

SC_MODULE(Control) {
	// Declare ports
	sc_in_clk clock;
	sc_in<sc_uint<8>> instruction;
	sc_in<bool> reset;
	sc_in<bool> n_flag;
	sc_in<bool> z_flag;
	sc_out<sc_uint<3>> alu_op;
	sc_out<bool> pc_incr;
	sc_out<bool> pc_load;
	sc_out<bool> ac_load;
	sc_out<bool> mem_addr_src;
	sc_out<bool> mem_read;
	sc_out<bool> mem_write;

	Reg<8>& instruction_decode() {
		return _instruction_decode_register;
	}

	// Main process and a Moore FSM
	void update_state() {
		std::cout << "@" << sc_time_stamp() << " " << this->name()
				<< " :: Current state:" << std::endl;
		std::cout << "\t state = " << StateString(_state) << std::endl;
		// Synchronous reset
		if (reset.read()) {
			pc_incr = false;
			pc_load = false;
			ac_load = false;
			mem_addr_src = false;
			mem_read = false;
			mem_write = false;
			alu_op.write(0);
			_cycles = 0;
			_inst_count = 0;

			std::cout << "\t pc_incr = " << false << std::endl;
			std::cout << "\t pc_load = " << false << std::endl;
			std::cout << "\t ac_load = " << false << std::endl;
			std::cout << "\t mem_addr_src = " << false << std::endl;
			std::cout << "\t mem_read = " << false << std::endl;
			std::cout << "\t mem_write = " << false << std::endl;
			std::cout << "@" << sc_time_stamp() << " " << this->name()
					<< " :: Synchronous reset!" << std::endl;
			std::cout << "\t cycles = " << 0 << std::endl;
			std::cout << "\t instruction_count = " << 0 << std::endl;
			_state = CS_FETCH_INST;
		} else {
			switch (_state) {
			// Fetch instruction
			case CS_FETCH_INST:
				pc_incr = true;
				pc_load = false;
				ac_load = false;
				mem_addr_src = false;
				mem_read = true;
				mem_write = false;
				_clock_enable = false;
				_inst_count++;

				std::cout << "\t pc_incr = " << true << std::endl;
				std::cout << "\t pc_load = " << false << std::endl;
				std::cout << "\t ac_load = " << false << std::endl;
				std::cout << "\t mem_addr_src = " << false << std::endl;
				std::cout << "\t mem_read = " << true << std::endl;
				std::cout << "\t mem_write = " << false << std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				_state = CS_MEM_READ_INST;
				break;
			// Decode instruction
			case CS_MEM_READ_INST:
				pc_incr = false;
				mem_read = false;
				_clock_enable = true;

				std::cout << "\t pc_incr = " << false << std::endl;
				std::cout << "\t mem_read = " << false << std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				_state = CS_DECODE_INST;
				break;
			case CS_DECODE_INST:
				_clock_enable = false;

				std::cout << "\t instruction = " << (instruction.read())
						<< std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				if (instruction.read() == NOP)
					_state = CS_FETCH_INST;
				else if (instruction.read() == HLT)
					_state = CS_HLT;
				else if (instruction.read() == NOT) {
					_state = CS_ALU_EXEC;
				} else {
					_state = CS_LOAD_OP_ADDR;
				}
				break;
			case CS_LOAD_OP_ADDR:
				pc_incr = true;
				mem_read = true;

				std::cout << "\t pc_incr = " << true << std::endl;
				std::cout << "\t mem_read = " << true << std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				if (_current_instruction.read() == STA)
					_state = CS_MEM_WRITE;
				else if (_current_instruction.read() >= JMP)
					_state = CS_JMP_EXEC;
				else
					_state = CS_LOAD_OP_DATA;
				break;
			case CS_LOAD_OP_DATA:
				pc_incr = false;
				mem_addr_src = true;
				mem_read = true;

				std::cout << "\t pc_incr = " << false << std::endl;
				std::cout << "\t mem_addr_src = " << true << std::endl;
				std::cout << "\t mem_read = " << true << std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				_state = CS_MEM_READ_DATA;
				break;
			case CS_MEM_READ_DATA:
				mem_addr_src = false;
				mem_read = false;

				std::cout << "\t mem_addr_src = " << false << std::endl;
				std::cout << "\t mem_read = " << false << std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				_state = CS_ALU_EXEC;
				break;
				// Execute instruction
			case CS_ALU_EXEC:
				if ((_current_instruction.read().range(6, 4) >= ALU_ADD)
						&& (_current_instruction.read().range(6, 4) <= ALU_LDA)) {
					alu_op.write(_current_instruction.read().range(6, 4));
				}

				std::cout << "\t alu_op = "
						<< (_current_instruction.read().range(6, 4))
						<< std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				_state = CS_ALU_AC_WRITE;
				break;
			case CS_ALU_AC_WRITE:
				ac_load = true;
				alu_op.write(0);

				std::cout << "\t ac_load = " << true << std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				_state = CS_FETCH_INST;
				break;
			case CS_MEM_WRITE:
				pc_incr = false;
				mem_addr_src = true;
				mem_read = false;
				mem_write = true;

				std::cout << "\t pc_incr = " << false << std::endl;
				std::cout << "\t mem_addr_src = " << true << std::endl;
				std::cout << "\t mem_read = " << false << std::endl;
				std::cout << "\t mem_write = " << true << std::endl;
				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				_state = CS_FETCH_INST;
				break;
			case CS_JMP_EXEC:
				pc_incr = false;

				std::cout << "\t pc_incr = " << false << std::endl;

				if (_current_instruction.read() == JMP) {
					pc_load = true;
					std::cout << "\t pc_load = " << true << std::endl;
				} else if (_current_instruction.read() == JN && n_flag) {
					pc_load = true;
					std::cout << "\t pc_load = " << true << std::endl;
				} else if (_current_instruction.read() == JZ && z_flag) {
					pc_load = true;
					std::cout << "\t pc_load = " << true << std::endl;
				} else
					std::cout << "\t pc_load = " << false << std::endl;

				std::cout << "\t cycles = " << (_cycles) << std::endl;
				std::cout << "\t instruction_count = " << (_inst_count)
						<< std::endl;

				_state = CS_FETCH_INST;
				break;
				// Halt
			case CS_HLT:
				_cycles--;
				break;
			}

			_cycles++;
		}
	}

	SC_CTOR(Control) :
			_instruction_decode_register("INST_DEC"), _cycles(0), _inst_count(
					0), _current_instruction(0) {
		//Connect Signals
		_instruction_decode_register.clock(clock);
		_instruction_decode_register.input(instruction);
		_instruction_decode_register.clock_enable(_clock_enable);
		_instruction_decode_register.asynchronous_clear(reset);
		_instruction_decode_register.output(_current_instruction);

		SC_METHOD(update_state);
		sensitive << clock.pos();
	}

	unsigned int cycles() {
		return _cycles;
	}
	unsigned int instruction_count() {
		return _inst_count;
	}

private:
	// Internal Components
	Reg<8> _instruction_decode_register;

	// States Declaration
	sc_signal<StateType> _state;

	// Internal Signals
	unsigned int _cycles;
	unsigned int _inst_count;

	sc_signal<sc_uint<8>> _current_instruction;
	sc_signal<bool> _clock_enable;
};

#endif /* CONTROL_H_ */
