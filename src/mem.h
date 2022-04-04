#ifndef MEM_H_
#define MEM_H_

#include <iostream>
#include <algorithm>
#include <systemc.h>

// Memory defines
#define MEMORY_WORD 8
#define MEMORY_SIZE 256

SC_MODULE(Mem) {
	// Declare ports
	sc_in_clk clock;
	sc_in<sc_uint<MEMORY_WORD>> address_input;
	sc_in<sc_uint<MEMORY_WORD>> data_input;
	sc_in<bool> read;
	sc_in<bool> write;
	sc_out<sc_uint<MEMORY_WORD>> data_output;

	// Main process
	void read_write() {
		// Asserted write
		if (write.read()) {
			_bank[address_input.read()] = data_input.read();

			std::cout << "@" << sc_time_stamp() << " " << this->name()
					<< " :: Writing to memory!" << std::endl;
			std::cout << "\t address_input = " << (address_input.read())
					<< std::endl;
			std::cout << "\t data_input = " << (data_input.read()) << std::endl;
			std::cout << "\t previous data_input = "
					<< (_bank[address_input.read()]) << std::endl;
		// Asserted read
		} else if (read.read()) {
			data_output.write(_bank[address_input.read()]);

			std::cout << "@" << sc_time_stamp() << " " << this->name()
					<< " :: Reading from memory!" << std::endl;
			std::cout << "\t address_input = " << (address_input.read())
					<< std::endl;
			std::cout << "\t data_output = " << (_bank[address_input.read()])
					<< std::endl;
			std::cout << "\t previous data_output = " << (data_output.read())
					<< std::endl;
		}
	}

	SC_CTOR(Mem) :
			_bank( { 0 }) {
		SC_METHOD(read_write);
		sensitive << clock.pos();
	}

	//For trace/tests purposes. Prints memory contents in stdout
	void dump(unsigned int start = 0, unsigned int end = MEMORY_SIZE - 1) {
		std::cout << "@" << sc_time_stamp() << " " << this->name()
				<< " :: Dumping memory to stdout!" << std::endl;
		for (uint i = start; i <= end; i++) {
			std::cout << i << ": " << (_bank[i]) << std::endl;

		}
	}

	void dump_signed(unsigned int start = 0,
			unsigned int end = MEMORY_SIZE - 1) {
		std::cout << "@" << sc_time_stamp() << " " << this->name()
				<< " :: Dumping memory with signed data to stdout!"
				<< std::endl;
		for (uint i = start; i <= end; i++) {
			std::cout << i << ": " << static_cast<sc_int<MEMORY_WORD>>(_bank[i])
					<< std::endl;

		}
	}

	void load(sc_uint<MEMORY_WORD> *memory, unsigned int start = 0,
			unsigned int end = MEMORY_SIZE - 1, unsigned int address = 0) {
		std::cout << "@" << sc_time_stamp() << " " << this->name()
				<< " :: Loading data to memory bank!" << std::endl;
		std::copy(memory + start, memory + end, _bank + address);
	}

// Unit testbench
	static int testbench(const char *trace_file) {
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);

		//Signals used in testbench
		sc_clock clock("clk", sc_time(2, SC_NS), 0.5, sc_time(1, SC_NS), false);
		sc_signal<sc_uint<MEMORY_WORD>> address_input;
		sc_signal<sc_uint<MEMORY_WORD>> data_input;
		sc_signal<bool> read;
		sc_signal<bool> write;
		sc_signal<sc_uint<MEMORY_WORD>> data_output;

		//Include signals to trace
		sc_trace(tf, clock, "clock");
		sc_trace(tf, address_input, "address_input");
		sc_trace(tf, data_input, "data_input");
		sc_trace(tf, read, "read");
		sc_trace(tf, write, "write");
		sc_trace(tf, data_output, "data_output");

		//Component initialization
		Mem mem("Mem");

		// Signal Connections
		mem.clock(clock);
		mem.address_input(address_input);
		mem.data_input(data_input);
		mem.read(read);
		mem.write(write);
		mem.data_output(data_output);

		// Generate Stimuli
		// @formatter:off
		std::cout << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "                Memory Testbench!                 " << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << std::endl;
		// @formatter:on

		// Initialize all inputs
		read = 0; // Initial value of read
		write = 0; // Initial value of write
		sc_start(1, SC_NS);

		write = 1; // Asserting write

		for (uint i = 0; i < 12; i++) {
			address_input = i; // Iterating over address_input
			data_input = i + 10; // Changing the value of data_input
			sc_start(2, SC_NS);
		}

		write = 0; // Deasserting write
		sc_start(2, SC_NS);

		read = 1; // Asserting read

		for (uint i = 0; i < 14; i++) {
			address_input = i; // Iterating over address_input
			sc_start(2, SC_NS);
		}

		read = 0; // Deasserting read
		sc_start(4, SC_NS);

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}

private:
	// Internal Signals
	sc_uint<MEMORY_WORD> _bank[MEMORY_SIZE];
};

#endif /* MEM_H_ */
