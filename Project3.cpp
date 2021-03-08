// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350


#include <string>
#include <iostream>
#include <getopt.h>
#include "database.h"

using namespace std;


int main(int argc, char* argv[]) {
	ios_base::sync_with_stdio(false);

	DataBase data;
	string junk;

	data.setArguments(argc, argv);

	if (data.getHelp()) {
		cout << "This program is meant to create a data base that can perform many operations on it.\n";
		cout << "The operations are: CREATE, INSERT INTO, DELETE FROM, GENERATE INDEX, PRINT FROM, JOIN,\n";
		cout << "REMOVE, and QUIT.\n";
		return 0;
	}

	cout << "% ";

	string command;

	while (cin >> command) {
		switch (command[0]) {
		case 'C':
			data.create();
			break;
		case 'I':
			cin >> junk;
			data.insert_into();
			break;
		case 'D':
			cin >> junk;
			data.delete_from();
			break;
		case 'G':
			cin >> junk;
			data.generate_index();
			break;
		case 'P':
			cin >> junk;
			data.print();
			break;
		case 'J':
			data.join();
			break;
		case 'R':
			data.remove();
			break;
		case 'Q':
			data.quit();
			return 0;
		case '#':
			getline(cin, junk);
			break;
		default:
			cout << "Error: unrecognized command\n";
			return 0;
		}
		
		cout << "% ";
	}


}