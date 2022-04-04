#ifndef ALU_H_
#define ALU_H_

#include <iostream>
#include <systemc.h>

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

template<int N>
SC_MODULE(ALU) {
	// Declare ports
	sc_in<sc_uint<3>> operation;
	sc_in<sc_uint<N>> register_output;
	sc_in<sc_uint<N>> memory_data_output;
	sc_out<sc_uint<N>> register_input;
	sc_out<bool> flag_negative;
	sc_out<bool> flag_zero;

	// Main process
	void operate() {
		if ((operation.read() >= ALU_ADD) && (operation.read() <= ALU_LDA)) {
			std::cout << "@" << sc_time_stamp() << " " << this->name()
					<< " :: ALU operation:" << std::endl;
			std::cout << "\t operation = ";

			switch (operation.read()) {
			// ALU ADD operation
			case ALU_ADD:
				register_input.write(
						register_output.read() + memory_data_output.read());

				std::cout << "ALU_ADD" << std::endl;
				std::cout << "\t register_output = "
						<< static_cast<sc_int<N>>(register_output.read())
						<< std::endl;
				std::cout << "\t memory_data_output = "
						<< static_cast<sc_int<N>>(memory_data_output.read())
						<< std::endl;
				std::cout << "\t result = "
						<< static_cast<sc_int<N>>(register_output.read()
								+ memory_data_output.read()) << std::endl;

				flag_negative.write(
						(static_cast<sc_uint<N>>(register_output.read()
								+ memory_data_output.read()) & (1 << (N - 1)))
								>> (N - 1));

				std::cout << "\t flag_negative = "
						<< ((static_cast<sc_uint<N>>(register_output.read()
								+ memory_data_output.read()) & (1 << (N - 1)))
								>> (N - 1)) << std::endl;

				if (static_cast<sc_uint<N>>(register_output.read()
						+ memory_data_output.read())) {
					flag_zero.write(false);
					std::cout << "\t flag_zero = " << false << std::endl;
				} else {
					flag_zero.write(true);
					std::cout << "\t flag_zero = " << true << std::endl;
				}

				break;
			// ALU AND operation
			case ALU_AND:
				register_input.write(
						register_output.read() & memory_data_output.read());

				std::cout << "ALU_AND" << std::endl;
				std::cout << "\t register_output = " << (register_output.read())
						<< std::endl;
				std::cout << "\t memory_data_output = "
						<< (memory_data_output.read()) << std::endl;
				std::cout << "\t result = "
						<< static_cast<sc_uint<N>>(register_output.read()
								& memory_data_output.read()) << std::endl;

				flag_negative.write(
						(static_cast<sc_uint<N>>(register_output.read()
								& memory_data_output.read()) & (1 << (N - 1)))
								>> (N - 1));

				std::cout << "\t flag_negative = "
						<< ((static_cast<sc_uint<N>>(register_output.read()
								& memory_data_output.read()) & (1 << (N - 1)))
								>> (N - 1)) << std::endl;

				if (static_cast<sc_uint<N>>(register_output.read()
						&& memory_data_output.read())) {
					flag_zero.write(false);
					std::cout << "\t flag_zero = " << false << std::endl;
				} else {
					flag_zero.write(true);
					std::cout << "\t flag_zero = " << true << std::endl;
				}

				break;
			// ALU OR operation
			case ALU_OR:
				register_input.write(
						register_output.read() | memory_data_output.read());

				std::cout << "ALU_OR" << std::endl;
				std::cout << "\t register_output = " << (register_output.read())
						<< std::endl;
				std::cout << "\t memory_data_output = "
						<< (memory_data_output.read()) << std::endl;
				std::cout << "\t result = "
						<< static_cast<sc_uint<N>>(register_output.read()
								| memory_data_output.read()) << std::endl;

				flag_negative.write(
						(static_cast<sc_uint<N>>(register_output.read()
								| memory_data_output.read()) & (1 << (N - 1)))
								>> (N - 1));

				std::cout << "\t flag_negative = "
						<< ((static_cast<sc_uint<N>>(register_output.read()
								| memory_data_output.read()) & (1 << (N - 1)))
								>> (N - 1)) << std::endl;

				if (static_cast<sc_uint<N>>(register_output.read()
						|| memory_data_output.read())) {
					flag_zero.write(false);
					std::cout << "\t flag_zero = " << false << std::endl;
				} else {
					flag_zero.write(true);
					std::cout << "\t flag_zero = " << true << std::endl;
				}
				break;
			// ALU NOT operation
			case ALU_NOT:
				register_input.write(~register_output.read());

				std::cout << "ALU_NOT" << std::endl;
				std::cout << "\t register_output = " << (register_output.read())
						<< std::endl;
				std::cout << "\t result = "
						<< static_cast<sc_uint<N>>(~register_output.read())
						<< std::endl;

				flag_negative.write(
						(static_cast<sc_uint<N>>(~register_output.read())
								& (1 << (N - 1))) >> (N - 1));

				std::cout << "\t flag_negative = "
						<< ((static_cast<sc_uint<N>>(~register_output.read())
								& (1 << (N - 1))) >> (N - 1)) << std::endl;

				if (static_cast<sc_uint<N>>(~register_output.read())) {
					flag_zero.write(false);
					std::cout << "\t flag_zero = " << false << std::endl;
				} else {
					flag_zero.write(true);
					std::cout << "\t flag_zero = " << true << std::endl;
				}

				break;
			// ALU LDA operation
			case ALU_LDA:
				register_input.write(memory_data_output.read());

				std::cout << "ALU_LDA" << std::endl;
				std::cout << "\t memory_data_output = "
						<< (memory_data_output.read()) << std::endl;
				std::cout << "\t result = " << (memory_data_output.read())
						<< std::endl;

				flag_negative.write(
						(static_cast<sc_uint<N>>(memory_data_output.read())
								& (1 << (N - 1)) >> (N - 1)));

				std::cout << "\t flag_negative = "
						<< ((static_cast<sc_uint<N>>(memory_data_output.read())
								& (1 << (N - 1)) >> (N - 1))) << std::endl;

				if (static_cast<sc_uint<N>>(memory_data_output.read())) {
					flag_zero.write(false);
					std::cout << "\t flag_zero = " << false << std::endl;
				} else {
					flag_zero.write(true);
					std::cout << "\t flag_zero = " << true << std::endl;
				}

				break;
			}
			std::cout << "\t previous result = " << (register_input.read())
					<< std::endl;
		}
	}

	SC_CTOR(ALU) {
		SC_METHOD(operate);
		sensitive << operation;
	}

// Unit testbench
	static int testbench(const char *trace_file) {
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);

		//Signals used in testbench
		sc_signal<sc_uint<3>> operation;
		sc_signal<sc_uint<N>> register_output;
		sc_signal<sc_uint<N>> memory_data_output;
		sc_signal<sc_uint<N>> register_input;
		sc_signal<bool> flag_negative;
		sc_signal<bool> flag_zero;

		//Include signals to trace
		sc_trace(tf, operation, "operation");
		sc_trace(tf, register_output, "register_output");
		sc_trace(tf, memory_data_output, "memory_data_output");
		sc_trace(tf, register_input, "register_input");
		sc_trace(tf, flag_negative, "flag_negative");
		sc_trace(tf, flag_zero, "flag_zero");

		//Component initialization
		ALU<8> alu("ALU");

		// Signal Connections
		alu.operation(operation);
		alu.register_output(register_output);
		alu.memory_data_output(memory_data_output);
		alu.register_input(register_input);
		alu.flag_negative(flag_negative);
		alu.flag_zero(flag_zero);

		// Generate Stimuli
		// @formatter:off
		std::cout << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "                   ALU Testbench!                 " << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << std::endl;
		// @formatter:on

		// Initialize all inputs
		register_output = 1; // Initial value of register_output
		memory_data_output = 0; // Initial value of memory_data_output
		operation = ALU_ADD; // Initial value of operation
		sc_start(4, SC_NS);

		operation = ALU_NOP; // Changing value of operation
		sc_start(2, SC_NS);

		register_output = 127; // Changing value of register_output
		memory_data_output = -127; // Changing value of memory_data_output
		operation = ALU_ADD; // Changing value of operation
		sc_start(4, SC_NS);

		operation = ALU_NOP; // Changing value of operation
		sc_start(2, SC_NS);

		memory_data_output = -128; // Changing value of memory_data_output
		operation = ALU_ADD; // Changing value of operation
		sc_start(4, SC_NS);

		operation = ALU_NOP; // Changing value of operation
		sc_start(2, SC_NS);

		register_output = 1; // Changing value of register_output
		memory_data_output = 0; // Changing value of memory_data_output
		operation = ALU_ADD; // Changing value of operation
		sc_start(4, SC_NS);

		register_output = 127; // Changing value of register_output
		memory_data_output = 2; // Changing value of memory_data_output
		operation = ALU_AND; // Changing value of operation
		sc_start(4, SC_NS);

		memory_data_output = 0; // Changing value of memory_data_output
		operation = ALU_OR; // Changing value of operation
		sc_start(4, SC_NS);

		register_output = 0; // Changing value of register_output
		operation = ALU_NOT; // Changing value of operation
		sc_start(4, SC_NS);

		memory_data_output = 42; // Changing value of memory_data_output
		operation = ALU_LDA; // Changing value of operation
		sc_start(4, SC_NS);

		register_output = 55; // Changing value of register_output
		memory_data_output = 0; // Changing value of memory_data_output
		operation = ALU_NOP; // Changing value of operation
		sc_start(8, SC_NS);

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}
};

#endif /* ALU_H_ */
