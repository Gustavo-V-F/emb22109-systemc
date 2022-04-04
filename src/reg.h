#ifndef REG_H_
#define REG_H_

#include <iostream>
#include <systemc.h>

template<int N>
SC_MODULE(Reg) {
	// Declare ports
	sc_in_clk clock;
	sc_in<sc_uint<N>> input;
	sc_in<bool> clock_enable;
	sc_in<bool> asynchronous_clear;
	sc_out<sc_uint<N>> output;

	// Main process
	void store() {
		// Asserted asynchronous_clear
		if (asynchronous_clear) {
			output.write(0);

			std::cout << "@" << sc_time_stamp() << " " << this->name()
					<< " :: Asynchronous reset!" << std::endl;
			std::cout << "\t input = " << (input.read()) << std::endl;
			std::cout << "\t clock_enable = " << clock_enable.read()
					<< std::endl;
			std::cout << "\t asynchronous_clear = " << asynchronous_clear.read()
					<< std::endl;
			std::cout << "\t output = " << 0 << std::endl;
			std::cout << "\t previous output = " << (output.read())
					<< std::endl;
		// Asserted clock_enable
		} else if (clock_enable) {
			output.write(input.read());

			std::cout << "@" << sc_time_stamp() << " " << this->name()
					<< " :: Register input data stored!" << std::endl;
			std::cout << "\t input = " << (input.read()) << std::endl;
			std::cout << "\t clock_enable = " << clock_enable.read()
					<< std::endl;
			std::cout << "\t asynchronous_clear = " << asynchronous_clear.read()
					<< std::endl;
			std::cout << "\t output = " << input.read() << std::endl;
			std::cout << "\t previous output = " << (output.read())
					<< std::endl;
		}
	}

	SC_CTOR(Reg) {
		SC_METHOD(store);
		sensitive << asynchronous_clear.pos() << clock.pos();
	}

	// Unit testbench
	static int testbench(const char *trace_file) {
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);

		//Signals used in testbench
		sc_clock clock("clk", sc_time(2, SC_NS), 0.5, sc_time(1, SC_NS), false);
		sc_signal<sc_uint<8>> input;
		sc_signal<bool> clock_enable;
		sc_signal<bool> asynchronous_clear;
		sc_signal<sc_uint<8>> output;

		//Include signals to trace
		sc_trace(tf, clock, "clock");
		sc_trace(tf, input, "input");
		sc_trace(tf, clock_enable, "clock_enable");
		sc_trace(tf, asynchronous_clear, "asynchronous_clear");
		sc_trace(tf, output, "output");

		//Component initialization
		Reg<8> reg("Reg");

		// Signal Connections
		reg.clock(clock);
		reg.input(input);
		reg.clock_enable(clock_enable);
		reg.asynchronous_clear(asynchronous_clear);
		reg.output(output);

		// Generate Stimuli
		// @formatter:off
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "              Register Testbench!                 " << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		// @formatter:on

		// Initialize all inputs
		input = 0xFF; // Initial value of input
		clock_enable = 0; // Initial value of clock_enable
		asynchronous_clear = 1; // Initial value of asynchronous_clear
		sc_start(3, SC_NS);

		clock_enable = 1; // Asserting the clock_enable
		asynchronous_clear = 0; // Deasserting the asynchronous_clear
		sc_start(2, SC_NS);

		clock_enable = 0; // Deasserting the clock_enable
		sc_start(4, SC_NS);

		input = 0x0F; // Changing value of input
		clock_enable = 1; // Asserting the clock_enable
		sc_start(2, SC_NS);

		clock_enable = 0; // Deasserting the clock_enable
		asynchronous_clear = 1; // Asserting the asynchronous_clear
		sc_start(4, SC_NS);

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}
};

#endif /* REG_H_ */
