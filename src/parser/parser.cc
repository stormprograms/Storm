/*
    parser.cc - Parse and Compile the program
    Copyright (C) 2020 Ethan Onstott

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../storm.h"
#include "parser.h"

// evaluate number of args given
int numArgsGiven(std::vector<std::string>::iterator chunk) {
	int arg = 1;
	bool inquotes = 0;

	do {	
		if (*(chunk) == "," && inquotes == 0)
			arg++;

		chunk++;
	} while (*chunk != "]");

	return arg;
}

// check if number of args is accurate
void checkargs(std::vector<std::string>::iterator chunk, int required, std::string kw) {
	if (*chunk != "[" || numArgsGiven(chunk) != 2) {
		// trigger error if not correct number of args
		std::cerr << "Compilation error: write requires 2 arguments\n";
		exit(EXIT_FAILURE);
	}
}

void getData(std::vector<std::string> splicedProgram) {
	for (auto chunk = splicedProgram.begin(); chunk != splicedProgram.end(); chunk++) {
		std::string kw = *chunk;
		if (kw == "write") {
			parser.text.push_back(0x0E); // move
			parser.text.push_back(0x0F); // veax

			parser.text.push_back(0x41);

			chunk++;
			checkargs(chunk, 2, kw);
			addArgsToData(chunk, 2, {STRING, STRING});
			do { chunk++; } while (*chunk != ";");
		}
		else if ((chunk != splicedProgram.end()) && (*(chunk + 1) == "=")) {
			declare(chunk);
			chunk++;
		}
	}
}

void compile(std::vector<std::string> splicedProgram) {
	getData(splicedProgram);
	
	// push compiled code to file
	std::string compiledFileName = parser.outfile;
	std::vector<uint8_t> fullprogram;
	
	// start formatting data section
	fullprogram.insert(fullprogram.end(),
		parser.data.begin(), parser.data.end());

	// format text
	fullprogram.insert(fullprogram.end(),
		parser.text.begin(), parser.text.end());

	std::ofstream stormc;
	stormc.open(compiledFileName);

	for (uint8_t byte : fullprogram) {
		stormc << byte;
	}
}
// bytecode is similar to nasm