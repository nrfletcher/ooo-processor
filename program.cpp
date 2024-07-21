#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include "program.h"
#include "processor.h"

void memread(uint32_t addr, uint32_t num_bytes) {
	switch(num_bytes) {
		case 1:
			std::cout << std::hex << "0x" << read_byte(addr) << std::endl;
			break;
		case 2:
			std::cout << std::hex << "0x" << read_2_bytes(addr) << std::endl;
			break;
		case 3:
			std::cout << std::hex << "0x" << read_3_bytes(addr) << std::endl;
			break;
		case 4:
			std::cout << std::hex << "0x" << read_4_bytes(addr) << std::endl;
			break;
		default:
			std::cout << "Invalid number of bytes to read provided: " << num_bytes << std::endl;
			break;
	}
}

void memwrite(uint32_t addr, uint32_t num_bytes, uint32_t data) {
	switch(num_bytes) {
		case 1:
			write_byte(addr, data);
			break;
		case 2:
			write_2_bytes(addr, data);
			break;
		case 3:
			write_3_bytes(addr, data);
			break;
		case 4:
			write_4_bytes(addr, data);
			break;
		default:
			std::cout << "Invalid number of bytes to write provided: " << num_bytes << std::endl;
			break;
	}
}

void display_memory(uint32_t addr, uint32_t num_32_blocks) {
	uint32_t curr_addr = addr;
	for(uint32_t i = 0; i < num_32_blocks; i++) {
		std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << curr_addr << ": ";
		for(int i = 0; i < 32; i++) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << read_byte(curr_addr) << " ";
			curr_addr++;
		}
		std::cout << std::endl;
	}
}

/* Loads a 4 byte value into a memory address for our program */
void load_instruction(const std::string& instruction, uint32_t address) {

}

/* Takes the first argument given when running program
 * and loads the binary file into memory. If an instruction
   is malformed the program ends. */
void load_program(const std::string& filename) {
	std::string filepath = "programs/" + filename + ".txt";
	std::ifstream MyReadFile(filepath);
	uint32_t curr_address = TEXT_SEGMENT_BEGIN;

	if(!MyReadFile.is_open()) {
		std::cerr << "Error: Unable to open file: " << filepath << std::endl;
		std::cerr << "Ensure that the program binary you wish to run is in /programs" << std::endl;
		exit(1);
	}

	std::string line;
	// For each line, parse the instruction and ensure it is a valid length string
	while(getline(MyReadFile, line)) {
		std::cout << line << std::endl;
		std::string instruction;

		for(char& c : line) {
			if(c == '#' && instruction.length() < 32) {
				std::cerr << "Error: Invalid or malformed instruction, found character " << c << " before parsing 32 bits" << std::endl;
				exit(1);
			} else if(c == '#' && instruction.length() >= 32) {
				break;
			}
			if(c == '0' || c == '1') instruction.push_back(c);
		}

		if(instruction.length() != 32) {
			std::cerr << "Error: Invalid instruction length: " << instruction.length() << std::endl;
			exit(1);
		}
		load_instruction(instruction, curr_address);
		curr_address += 4;
	}

	MyReadFile.close();
}

void execute_program(bool interactive_mode) {
	/* Interactive mode allows user to step through code 
	   Otherwise, we execute the entire program */
	if(interactive_mode) {

	} else {

	}
}

int main(int argc, char* argv[]) {
	/* User needs to provide a program file to run and a choice of execution mode. */
	if(argc < 3) {
		std::cerr << "Usage: " << argv[0] << " <filename> <interactive>" << std::endl;
		std::cerr << "Filename is binary you wish to run, interactive mode is 1 or 0." << std::endl;
		exit(1);
	}

	/* Currently supported modes: interactive and non-interactive. */
	bool interactive_mode = (bool) argv[2];
	if(interactive_mode != 0 && interactive_mode != 1) {
		std::cerr << "Invalid argument for interactive mode: " << interactive_mode << std::endl;
		exit(1);
	}

	std::string filename = argv[1];
	load_program(filename);

	init_registers(STACK_SEGMENT_BEGIN, TEXT_SEGMENT_BEGIN);
	execute_program(interactive_mode);

	return EXIT_SUCCESS;
}
