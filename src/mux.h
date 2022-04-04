#ifndef MUX_H_
#define MUX_H_

#include <iostream>
#include <systemc.h>

template<int N>
SC_MODULE(Mux) {
	// Declare ports
	sc_in<sc_uint<N>> input_a;
	sc_in<sc_uint<N>> input_b;
	sc_in<bool> select;
	sc_out<sc_uint<N>> output;

	// Main process
	void selection() {
		// Deasserted select
		if (!select.read()) {
			output.write(input_a.read());

			std::cout << "@" << sc_time_stamp() << " " << this->name()
					<< " :: Output changed to input_a!" << std::endl;
			std::cout << "\t input_a = " << (input_a.read()) << std::endl;
			std::cout << "\t input_b = " << (input_b.read()) << std::endl;
			std::cout << "\t select = " << select.read() << std::endl;
			std::cout << "\t output = " << (input_a.read()) << std::endl;
			std::cout << "\t previous output = " << (output.read())
					<< std::endl;
		// Asserted select
		} else {
			output.write(input_b.read());

			std::cout << "@" << sc_time_stamp() << " " << this->name()
					<< " :: Output changed to input_b!" << std::endl;
			std::cout << "\t input_a = " << (input_a.read()) << std::endl;
			std::cout << "\t input_b = " << (input_b.read()) << std::endl;
			std::cout << "\t select = " << select.read() << std::endl;
			std::cout << "\t output = " << (input_b.read()) << std::endl;
			std::cout << "\t previous output = " << (output.read())
					<< std::endl;
		}
	}

	SC_CTOR(Mux) {
		SC_METHOD(selection);
		sensitive << input_a << input_b << select;
	}

	// Unit testbench
	static int testbench(const char *trace_file) {
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);

		//Signals used in testbench
		sc_signal<sc_uint<8>> input_a;
		sc_signal<sc_uint<8>> input_b;
		sc_signal<bool> select;
		sc_signal<sc_uint<8>> output;

		//Include signals to trace
		sc_trace(tf, input_a, "input_a");
		sc_trace(tf, input_b, "input_b");
		sc_trace(tf, select, "select");
		sc_trace(tf, output, "output");

		//Component initialization
		Mux<8> mux("Mux");

		// Signal Connections
		mux.input_a(input_a);
		mux.input_b(input_b);
		mux.select(select);
		mux.output(output);

		// Generate Stimuli
		// @formatter:off
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "                   Mux Testbench!                 " << std::endl;
		std::cout << "                                                  " << std::endl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		// @formatter:on

		// Initialize all inputs
		input_a = 0xAA; // Initial value of input_a
		input_b = 0xBB; // Initial value of input_b
		select = 0; // Initial value of select
		sc_start(4, SC_NS);

		select = 1; // Asserting the select
		sc_start(4, SC_NS);

		select = 0; // Deasserting the select
		sc_start(4, SC_NS);

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}
};

#endif /* MUX_H_ */
