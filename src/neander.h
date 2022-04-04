#ifndef NEANDER_H_
#define NEANDER_H_

#include <iostream>
#include <systemc.h>

#include "mux.h"
#include "alu.h"
#include "counter.h"
#include "reg.h"
#include "mem.h"
#include "control.h"

// Neander defines
#define NEANDER_TEXT 0
#define NEANDER_DATA 128

// Top-level
SC_MODULE(Neander) {
	// Input Ports
	sc_in_clk clock;
	sc_in<bool> reset;

	void process() {

	}

	Reg<8>& ac() {
		return _ac;
	}

	Counter<8>& pc() {
		return _pc;
	}

	Mux<8>& mux_addr() {
		return _mux_addr;
	}

	Mem& mem() {
		return _mem;
	}

	ALU<8>& alu() {
		return _alu;
	}

	Control& uc() {
		return _uc;
	}

	void execution_stats() {
		// @formatter:off
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "Instructions Executed: " << _uc.instruction_count() << std::endl;
		std::cout << "               Cycles: " << _uc.cycles() << std::endl;
		std::cout << "                  CPI: " << _uc.cycles() / (double) _uc.instruction_count() << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		// @formatter:on
	}

	SC_CTOR(Neander) :
			_ac("AC"), _pc("PC"), _mux_addr("MUX_Addr"), _mem("MEM"), _alu(
					"ALU"), _uc("UC") {
		//Connect Signals
		// UC
		_uc.clock(clock);
		_uc.instruction(_mem_data_out);
		_uc.reset(reset);
		_uc.n_flag(_alu_flagN);
		_uc.z_flag(_alu_flagZ);
		_uc.alu_op(_alu_op);
		_uc.pc_incr(_pc_incr);
		_uc.pc_load(_pc_load);
		_uc.ac_load(_ac_load);
		_uc.mem_addr_src(_mem_addr_sel);
		_uc.mem_read(_mem_rd);
		_uc.mem_write(_mem_wr);

		// PC
		_pc.clock(clock);
		_pc.input(_mem_data_out);
		_pc.clock_enable(_pc_load);
		_pc.asynchronous_clear(reset);
		_pc.increment(_pc_incr);
		_pc.output(_pc_data_out);

		// MUX_Addr
		_mux_addr.input_a(_pc_data_out);
		_mux_addr.input_b(_mem_data_out);
		_mux_addr.select(_mem_addr_sel);
		_mux_addr.output(_mem_addr_in);

		// MEM
		_mem.clock(clock);
		_mem.address_input(_mem_addr_in);
		_mem.data_input(_ac_data_out);
		_mem.read(_mem_rd);
		_mem.write(_mem_wr);
		_mem.data_output(_mem_data_out);

		// ALU
		_alu.operation(_alu_op);
		_alu.register_output(_ac_data_out);
		_alu.memory_data_output(_mem_data_out);
		_alu.register_input(_ac_data_in);
		_alu.flag_negative(_alu_flagN);
		_alu.flag_zero(_alu_flagZ);

		// AC
		_ac.clock(clock);
		_ac.input(_ac_data_in);
		_ac.clock_enable(_ac_load);
		_ac.asynchronous_clear(reset);
		_ac.output(_ac_data_out);

		//Declare process
		SC_METHOD(process);
		sensitive << reset.pos() << clock.pos();
	}

	// Unit testbench
	static int testbench(const char *trace_file) {
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);

		//Signals used in testbench
		sc_clock clock("clk", sc_time(2, SC_NS), 0.5, sc_time(1, SC_NS), false);
		sc_signal<bool> reset;

		sc_uint<8> text[] = { LDA, 128, ADD, 129, JZ, 8, ADD, 129, JN, 12, ADD,
				129, JN, 16, ADD, 128, AND, 130, OR, 131, STA, 132, NOT, JMP,
				27, JMP, 30, STA, 133, NOP, HLT };
		sc_uint<8> data[] = { 1, -1, 7, 14, 0, 0 };
		int text_size = *(&text + 1) - text;
		int data_size = *(&data + 1) - data;

		//Component initialization
		Neander neander("Neander");

		// Signal Connections
		neander.clock(clock);
		neander.reset(reset);

		//Include signals to trace
		sc_trace(tf, clock, "clock");
		sc_trace(tf, reset, "reset");
		sc_trace(tf, neander.pc().input, "pc_data_in");
		sc_trace(tf, neander.pc().clock_enable, "pc_load");
		sc_trace(tf, neander.pc().increment, "pc_incr");
		sc_trace(tf, neander.pc().output, "pc_data_out");
		sc_trace(tf, neander.mux_addr().select, "mem_addr_sel");
		sc_trace(tf, neander.mem().address_input, "mem_addr_in");
		sc_trace(tf, neander.mem().read, "mem_rd");
		sc_trace(tf, neander.mem().write, "mem_wr");
		sc_trace(tf, neander.mem().data_input, "mem_data_in");
		sc_trace(tf, neander.mem().data_output, "mem_data_out");
		sc_trace(tf, neander.ac().input, "ac_data_in");
		sc_trace(tf, neander.ac().clock_enable, "ac_load");
		sc_trace(tf, neander.ac().output, "ac_data_out");
		sc_trace(tf, neander.uc().instruction_decode().output,
				"instruction_decode");
		sc_trace(tf, neander.uc().n_flag, "alu_flagN");
		sc_trace(tf, neander.uc().z_flag, "alu_flagZ");
		sc_trace(tf, neander.uc().alu_op, "alu_op");

		// Generate Stimuli
		// @formatter:off
		std::cout << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "               Neander Testbench!                 " << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << std::endl;
		// @formatter:on

		// Initialize memory and all inputs
		neander.mem().load(text, 0, text_size, NEANDER_TEXT); // Initial text segment memory load
		neander.mem().load(data, 0, data_size, NEANDER_DATA); // Initial data segment memory load
		reset = 1; // Initial value of reset
		sc_start(2, SC_NS);

		reset = 0; // Deasserting reset
		sc_start(26, SC_NS);

		reset = 1; // Asserting reset
		sc_start(2, SC_NS);

		reset = 0; // Deasserting reset
		sc_start(170, SC_NS);

		// @formatter:off
		std::cout << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << std::endl;
		std::cout << ".TEXT" << std::endl;
		// @formatter:on

		neander.mem().dump(NEANDER_TEXT, text_size - 1);

		// @formatter:off
		std::cout << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << std::endl;
		std::cout << ".DATA" << std::endl;
		// @formatter:on

		neander.mem().dump(NEANDER_DATA, NEANDER_DATA + data_size - 1);

		// @formatter:off
		std::cout << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << std::endl;
		std::cout << ".DATA" << std::endl;
		// @formatter:on

		neander.mem().dump_signed(NEANDER_DATA, NEANDER_DATA + data_size - 1);

		// @formatter:off
		std::cout << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << std::endl;
		// @formatter:on

		neander.execution_stats();

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}

private:
	// Internal Components
	// @formatter:off
	Reg<8> 		_ac;
	Counter<8>  _pc;
	Mux<8> 		_mux_addr;
	Mem 		_mem;
	ALU<8> 		_alu;
	Control 	_uc;
	// @formatter:on

	// Internal Signals
	// PC
	sc_signal<bool> _pc_load;
	sc_signal<bool> _pc_incr;
	sc_signal<sc_uint<8>> _pc_data_out;

	// MEM
	sc_signal<sc_uint<8>> _mem_addr_in;
	sc_signal<bool> _mem_addr_sel;
	sc_signal<bool> _mem_rd;
	sc_signal<bool> _mem_wr;
	sc_signal<sc_uint<8>> _mem_data_out;

	// ALU
	sc_signal<bool> _alu_flagN;
	sc_signal<bool> _alu_flagZ;
	sc_signal<sc_uint<3>> _alu_op;

	// AC
	sc_signal<sc_uint<8>> _ac_data_in;
	sc_signal<bool> _ac_load;
	sc_signal<sc_uint<8>> _ac_data_out;
};

#endif /* NEANDER_H_ */
