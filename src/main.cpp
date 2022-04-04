#include <iostream>
#include <systemc.h>
#include "neander.h"

int sc_main(int argc, char *argv[]) {
	sc_report_handler::set_verbosity_level(0);

	// Bellow are the testbench units for each module. They can't
	// be run simultaneously in systemC, meaning that only one of
	// the units must be uncommented each time.

	// @formatter:off
	std::cout << std::endl;
	std::cout << "***********************************" << std::endl;
	std::cout << "*                                 *" << std::endl;
	std::cout << "*       Starting Simulation!      *" << std::endl;
	std::cout << "*                                 *" << std::endl;
	std::cout << "***********************************" << std::endl;
	std::cout << std::endl;
	// @formatter:on

	//Mux<8>::testbench("mux_trace");
	//Reg<8>::testbench("reg_trace");
	//Counter<8>::testbench("cnt_trace");
	//ALU<8>::testbench("alu_trace");
	//Mem::testbench("mem_trace");
	Neander::testbench("neander");

	// @formatter:off
	std::cout << std::endl;
	std::cout << "***********************************" << std::endl;
	std::cout << "*                                 *" << std::endl;
	std::cout << "*       Ending Simulation!        *" << std::endl;
	std::cout << "*                                 *" << std::endl;
	std::cout << "***********************************" << std::endl;
	std::cout << std::endl;
	// @formatter:on
	return 0;
}
;
