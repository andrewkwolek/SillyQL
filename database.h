// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "TableEntry.h"

using namespace std;

class Compare {
private:
	char op;
	int col;
	TableEntry comp;

public:
	Compare(char op_in, int col_in, TableEntry comp_in) 
	: op(op_in), col(col_in), comp(comp_in) { }

	bool operator()(const vector<TableEntry>& in) const {
		switch (op) {
		case '<':
			return in[col] < comp;
		case '>':
			return in[col] > comp;
		case '=':
			return in[col] == comp;
		}
		return false;
	}
};

class DataBase {
public:
	class Table {
	public:

		Table()
			: entries(), cols(), types(), hash_table(), bst(), name(), hash_col(-1), bst_col(-1) { }

		~Table() { }

	private:
		vector<vector<TableEntry>> entries;
		vector<string> cols;
		vector<char> types;
		unordered_map<TableEntry, vector<int>> hash_table;
		map<TableEntry, vector<int>> bst;
		string name;
		int hash_col = -1;
		int bst_col = -1;

		friend class DataBase;

	public:

		void Insert(int rows) {
			size_t startN = entries.size();
			size_t endN = startN + rows - 1;

			entries.reserve(entries.size() + (size_t)rows - 1);

			for (size_t i = startN; i < endN + 1; ++i) {
				vector<TableEntry> new_vec;
				for (size_t j = 0; j < types.size(); ++j) {
					switch (types[j]) {
					case 'd': {
						double d;
						cin >> d;
						TableEntry in(d);
						new_vec.push_back(in);
						if ((int)j == hash_col) {
							hash_table.insert({ in, vector<int>() });
							hash_table[in].push_back((int)i);
						}
						if ((int)j == bst_col) {
							bst.insert({ in, vector<int>() });
							bst[in].push_back((int)i);
						}
						break;
					}

					case 'i': {
						int id;
						cin >> id;
						TableEntry in(id);
						new_vec.push_back(in);
						if ((int)j == hash_col) {
							hash_table.insert({ in, vector<int>() });
							hash_table[in].push_back((int)i);
						}
						if ((int)j == bst_col) {
							bst.insert({ in, vector<int>() });
							bst[in].push_back((int)i);
						}
						break;
					}
					case 'b': {
						string b;
						cin >> b;
						switch (b[0]) {
						case 't': {
							TableEntry in(true);
							new_vec.push_back(in);
							if ((int)j == hash_col) {
								hash_table.insert({ in, vector<int>() });
								hash_table[in].push_back((int)i);
							}
							if ((int)j == bst_col) {
								bst.insert({ in, vector<int>() });
								bst[in].push_back((int)i);
							}
							break;
						}
						case 'f': {
							TableEntry in(false);
							new_vec.push_back(in);
							if ((int)j == hash_col) {
								hash_table.insert({ in, vector<int>() });
								hash_table[in].push_back((int)i);
							}
							if ((int)j == bst_col) {
								bst.insert({ in, vector<int>() });
								bst[in].push_back((int)i);
							}
							break;
						}
						}
						break;
					}

					case 's': {
						string s;
						cin >> s;
						TableEntry in(s);
						new_vec.push_back(in);
						if ((int)j == hash_col) {
							hash_table.insert({ in, vector<int>() });
							hash_table[in].push_back((int)i);
						}
						if ((int)j == bst_col) {
							bst.insert({ in, vector<int>() });
							bst[in].push_back((int)i);
						}
						break;
					}
					}
				} //for inner
				entries.push_back(new_vec);
			} //for outer

			cout << "Added " << rows << " rows to " << name << " from position " << startN << " to " << endN << "\n";
		}

	};

private:
	unordered_map<string, Table> tables;
	bool help;
	bool quiet;

	


public:

	//Constructor
	DataBase()
		:tables(), help(false), quiet(false) { }

	//Destructor
	~DataBase() { }

	void create() {
		string name;
		int cols;
		Table tab;
		char type;
		string col_name;
		string junk;



		cin >> name;

		if (tables.find(name) != tables.end()) {
			cout << "Error: Cannot create already existing table " << name << "\n";
			return;
		}

		tab.name = name;

		cin >> cols;



		for (int i = 0; i < cols; ++i) {
			cin >> type;
			cin >> junk;
			tab.types.push_back(type);
		}

		for (int i = 0; i < cols; ++i) {
			cin >> col_name;
			tab.cols.push_back(col_name);
		}

		tables.insert({ name, tab });

		cout << "New table " << name << " with column(s) ";

		for (int i = 0; i < cols; ++i) {
			cout << tab.cols[i] << " ";
		}

		cout << "created\n";

		return;
	}

	//Table needs helper from error down and needs to know its own name
	//Go back and resize
	void insert_into() {
		string name;
		int rows;
		string junk;

		cin >> name;
		
		if (tables.find(name) == tables.end()) {
			cout << "Error: " << name << " does not name a table in the database\n";
			return;
		}

		cin >> rows;
		cin >> junk;

		tables[name].Insert(rows);
	}

	//check to optimize switches and loops
	void delete_from() {
		string name;
		string junk;
		string colname;
		char OP;
		int count = 0;

		cin >> name;
		
		if (tables.find(name) == tables.end()) {
			cout << "Error: " << name << " does not name a table in the database\n";
			return;
		}

		cin >> junk;
		cin >> colname;
		
		int colNum = -1;
		for (size_t i = 0; i < tables[name].cols.size(); ++i) {
			if (tables[name].cols[i] == colname) {
				colNum = (int)i;
				break;
			}
		}

		if (colNum == -1) {
			cout << "Error: " << colname << " does not name a column in " << name << "\n";
			return;
		}

		cin >> OP;

		switch (tables[name].types[colNum]) {
		case 'd': {
			double d;
			cin >> d;
			TableEntry in(d);
			Compare com(OP, colNum, in);
			int before = (int)tables[name].entries.size();
			tables[name].entries.erase(remove_if(tables[name].entries.begin(), tables[name].entries.end(), com), tables[name].entries.end());
			if (tables[name].hash_col != -1)
				rebuild_data(name, tables[name].hash_col, 'h');
			if (tables[name].bst_col != -1)
				rebuild_data(name, tables[name].bst_col, 'b');
			count = before - (int)tables[name].entries.size();
			break;
		}

		case 'i': {
			int id;
			cin >> id;
			TableEntry in(id);
			Compare com(OP, colNum, in);
			int before = (int)tables[name].entries.size();
			tables[name].entries.erase(remove_if(tables[name].entries.begin(), tables[name].entries.end(), com), tables[name].entries.end());
			if (tables[name].hash_col != -1)
				rebuild_data(name, tables[name].hash_col, 'h');
			if (tables[name].bst_col != -1)
				rebuild_data(name, tables[name].bst_col, 'b');
			count = before - (int)tables[name].entries.size();
			break;
		}
		case 'b': {
			string b;
			cin >> b;
			switch (b[0]) {
			case 't': {
				TableEntry in(true);
				Compare com(OP, colNum, in);
				int before = (int)tables[name].entries.size();
				tables[name].entries.erase(remove_if(tables[name].entries.begin(), tables[name].entries.end(), com), tables[name].entries.end());
				if (tables[name].hash_col != -1)
					rebuild_data(name, tables[name].hash_col, 'h');
				if (tables[name].bst_col != -1)
					rebuild_data(name, tables[name].bst_col, 'b');
				count = before - (int)tables[name].entries.size();
				break;
			}
			case 'f': {
				TableEntry in(false);
				Compare com(OP, colNum, in);
				int before = (int)tables[name].entries.size();
				tables[name].entries.erase(remove_if(tables[name].entries.begin(), tables[name].entries.end(), com), tables[name].entries.end());
				if (tables[name].hash_col != -1)
					rebuild_data(name, tables[name].hash_col, 'h');
				if (tables[name].bst_col != -1)
					rebuild_data(name, tables[name].bst_col, 'b');
				count = before - (int)tables[name].entries.size();
				break;
			}
			}
			
			break;
		}

		case 's': {
			string s;
			cin >> s;
			TableEntry in(s);
			Compare com(OP, colNum, in);
			int before = (int)tables[name].entries.size();
			tables[name].entries.erase(remove_if(tables[name].entries.begin(), tables[name].entries.end(), com), tables[name].entries.end());
			//check hash and bst
			if (tables[name].hash_col != -1)
				rebuild_data(name, tables[name].hash_col, 'h');
			if (tables[name].bst_col != -1)
				rebuild_data(name, tables[name].bst_col, 'b');
			count = before - (int)tables[name].entries.size();
			break;
		}
		}

		
		
		cout << "Deleted " << count << " rows from " << name << "\n";
	}

	void generate_index() {
		string name;
		string index_type;
		string junk;
		string colname;

		cin >> name;
		
		if (tables.find(name) == tables.end()) {
			cout << "Error: " << name << " does not name a table in the database\n";
			return;
		}

		cin >> index_type;
		cin >> junk;
		cin >> junk;
		cin >> colname;

		int colNum = -1;
		for (size_t i = 0; i < tables[name].cols.size(); ++i) {
			if (tables[name].cols[i] == colname) {
				colNum = (int)i;
				break;
			}
		}

		if (colNum == -1) {
			cout << "Error: " << colname << " does not name a column in " << name << "\n";
			return;
		}

		switch (index_type[0]) {
		case 'h': {
			tables[name].hash_table.clear();
			tables[name].hash_col = colNum;
			for (size_t i = 0; i < tables[name].entries.size(); ++i) {
				TableEntry temp(tables[name].entries[i][colNum]);
				tables[name].hash_table.insert({ temp, vector<int>() });
				tables[name].hash_table[temp].push_back((int)i);
			}
			break;
		}
		case 'b': {
			tables[name].bst.clear();
			tables[name].bst_col = colNum;
			for (size_t i = 0; i < tables[name].entries.size(); ++i) {
				TableEntry temp(tables[name].entries[i][colNum]);
				tables[name].bst.insert({ temp, vector<int>() });
				tables[name].bst[temp].push_back((int)i);
			}
			break;
		}
		}

		cout << "Created " << index_type << " index for table " << name << " on column " << colname << "\n";
	}

	void print() {
		string name;
		int num_cols;
		string colname;
		vector<int> colIns;
		string next;
		string colSpec;
		string junk;
		char OP;
		int count = 0;

		cin >> name;

		cout << boolalpha;

		if (tables.find(name) == tables.end()) {
			getline(cin, junk);
			cout << "Error: " << name << " does not name a table in the database\n";
			return;
		}

		cin >> num_cols;

		//make vector of inputted columns
		for (int i = 0; i < num_cols; ++i) {
			cin >> colname;
			bool inTable = false;
			for (size_t j = 0; j < tables[name].cols.size(); ++j) {
				if (tables[name].cols[j] == colname) {
					colIns.push_back((int)j);
					inTable = true;
					break;
				}
			}

			if (!inTable) {
				getline(cin, junk);
				cout << "Error: " << colname << " does not name a column in " << name << "\n";
				return;
			}
		}
		

		cin >> next;

		//check for WHERE or ALL
		switch (next[0]) {
		case 'A': {
			if (!quiet) {
				for (size_t i = 0; i < colIns.size(); ++i) {
					cout << tables[name].cols[colIns[i]] << " ";
				}
				cout << "\n";

				for (size_t x = 0; x < tables[name].entries.size(); ++x) {
					for (size_t k = 0; k < colIns.size(); ++k) {
						cout << tables[name].entries[x][colIns[k]] << " ";
					}
					cout << "\n";
				}
			}
			count = count + (int)tables[name].entries.size();
			break;
		}
		case 'W': {
			int colNum = -1;
			cin >> colSpec;
			cin >> OP;

			for (size_t t = 0; t < tables[name].cols.size(); ++t) {
				if (tables[name].cols[t] == colSpec) {
					colNum = (int)t;
					break;
				}
			}

			if (colNum == -1) {
				cin >> junk;
				cout << "Error: " << colSpec << " does not name a column in " << name << "\n";
				return;
			}

			if (!quiet) {
				for (size_t i = 0; i < colIns.size(); ++i) {
					cout << tables[name].cols[colIns[i]] << " ";
				}
				cout << "\n";
			}

			switch (tables[name].types[colNum]) {
			case 'd': {
				double d;
				cin >> d;
				TableEntry in(d);
				switch (OP) {
				case '<': {
					if (colNum == tables[name].bst_col) {
						for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.begin(); 
							iter != tables[name].bst.lower_bound(in); ++iter) {
							if (!quiet) {
								for (size_t i = 0; i < iter->second.size(); ++i) {
									for (size_t j = 0; j < colIns.size(); ++j) {
										cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
									}
									cout << "\n";
								}
							}
							count = count + (int)iter->second.size();
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] < in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				case '>': {
					if (colNum == tables[name].bst_col) {
						for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.upper_bound(in);
							iter != tables[name].bst.end(); ++iter) {
								if (!quiet) {
									for (size_t i = 0; i < iter->second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)iter->second.size();
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] > in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				case '=': {
					if (colNum == tables[name].hash_col) {
						for (auto key : tables[name].hash_table) {
							if (key.first == in) {
								if (!quiet) {
									for (size_t i = 0; i < key.second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)key.second.size();
							}
						}
					}
					else if (colNum == tables[name].bst_col) {
						for (auto key : tables[name].bst) {
							if (key.first == in) {
								if (!quiet) {
									for (size_t i = 0; i < key.second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)key.second.size();
							}
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] == in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				}
				break;

			}
			case 'i': {
				int IN;
				cin >> IN;
				TableEntry in(IN);
				switch (OP) {
				case '<': {
					if (colNum == tables[name].bst_col) {
						for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.begin();
							iter != tables[name].bst.lower_bound(in); ++iter) {
								if (!quiet) {
									for (size_t i = 0; i < iter->second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)iter->second.size();
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] < in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				case '>': {
					if (colNum == tables[name].bst_col) {
						for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.upper_bound(in);
							iter != tables[name].bst.end(); ++iter) {
								if (!quiet) {
									for (size_t i = 0; i < iter->second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)iter->second.size();
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] > in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				case '=': {
					if (colNum == tables[name].hash_col) {
						for (auto key : tables[name].hash_table) {
							if (key.first == in) {
								if (!quiet) {
									for (size_t i = 0; i < key.second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)key.second.size();
							}
						}
					}
					else if (colNum == tables[name].bst_col) {
						for (auto key : tables[name].bst) {
							if (key.first == in) {
								if (!quiet) {
									for (size_t i = 0; i < key.second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)key.second.size();
							}
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] == in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				}
				break;
			}
			case 's': {
				string s;
				cin >> s;
				TableEntry in(s);
				switch (OP) {
				case '<': {
					if (colNum == tables[name].bst_col) {
						for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.begin();
							iter != tables[name].bst.lower_bound(in); ++iter) {
								if (!quiet) {
									for (size_t i = 0; i < iter->second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)iter->second.size();
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] < in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				case '>': {
					if (colNum == tables[name].bst_col) {
						for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.upper_bound(in);
							iter != tables[name].bst.end(); ++iter) {
								if (!quiet) {
									for (size_t i = 0; i < iter->second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)iter->second.size();
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] > in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				case '=': {
					if (colNum == tables[name].hash_col) {
						for (auto key : tables[name].hash_table) {
							if (key.first == in) {
								if (!quiet) {
									for (size_t i = 0; i < key.second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)key.second.size();
							}
						}
					}
					else if (colNum == tables[name].bst_col) {
						for (auto key : tables[name].bst) {
							if (key.first == in) {
								if (!quiet) {
									for (size_t i = 0; i < key.second.size(); ++i) {
										for (size_t j = 0; j < colIns.size(); ++j) {
											cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
										}
										cout << "\n";
									}
								}
								count = count + (int)key.second.size();
							}
						}
					}
					else {
						for (size_t q = 0; q < tables[name].entries.size(); ++q) {
							if (tables[name].entries[q][colNum] == in) {
								if (!quiet) {
									for (size_t k = 0; k < colIns.size(); ++k) {
										cout << tables[name].entries[q][colIns[k]] << " ";
									}
									cout << "\n";
								}
								count++;
							}
						}
					}
					break;
				}
				}
				break;
			}
			case 'b': {
				string b;
				cin >> b;
				switch (b[0]) {
				case 't': {
					TableEntry in(true);
					switch (OP) {
					case '<': {
						if (colNum == tables[name].bst_col) {
							for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.begin();
								iter != tables[name].bst.lower_bound(in); ++iter) {
									if (!quiet) {
										for (size_t i = 0; i < iter->second.size(); ++i) {
											for (size_t j = 0; j < colIns.size(); ++j) {
												cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
											}
											cout << "\n";

										}
									}
									count = count + (int)iter->second.size();
							}
						}
						else {
							for (size_t q = 0; q < tables[name].entries.size(); ++q) {
								if (tables[name].entries[q][colNum] < true) {
									if (!quiet) {
										for (size_t k = 0; k < colIns.size(); ++k) {
											cout << tables[name].entries[q][colIns[k]] << " ";
										}
										cout << "\n";
									}
									count++;
								}
							}
						}
						break;
					}
					case '>': {
						if (colNum == tables[name].bst_col) {
							for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.upper_bound(in);
								iter != tables[name].bst.end(); ++iter) {
									if (!quiet) {
										for (size_t i = 0; i < iter->second.size(); ++i) {
											for (size_t j = 0; j < colIns.size(); ++j) {
												cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
											}
											cout << "\n";
										}
									}
									count = count + (int)iter->second.size();
							}
						}
						else {
							for (size_t q = 0; q < tables[name].entries.size(); ++q) {
								if (tables[name].entries[q][colNum] > true) {
									if (!quiet) {
										for (size_t k = 0; k < colIns.size(); ++k) {
											cout << tables[name].entries[q][colIns[k]] << " ";
										}
										cout << "\n";
									}
									count++;
								}
							}
						}
						break;
					}
					case '=': {
						if (colNum == tables[name].hash_col) {
							for (auto key : tables[name].hash_table) {
								if (key.first == true) {
									if (!quiet) {
										for (size_t i = 0; i < key.second.size(); ++i) {
											for (size_t j = 0; j < colIns.size(); ++j) {
												cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
											}
											cout << "\n";
										}
									}
									count = count + (int)key.second.size();
								}
							}
						}
						else if (colNum == tables[name].bst_col) {
							for (auto key : tables[name].bst) {
								if (key.first == true) {
									if (!quiet) {
										for (size_t i = 0; i < key.second.size(); ++i) {
											for (size_t j = 0; j < colIns.size(); ++j) {
												cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
											}
											cout << "\n";
										}
									}
									count = count + (int)key.second.size();
								}
							}
						}
						else {
							for (size_t q = 0; q < tables[name].entries.size(); ++q) {
								if (tables[name].entries[q][colNum] == true) {
									if (!quiet) {
										for (size_t k = 0; k < colIns.size(); ++k) {
											cout << tables[name].entries[q][colIns[k]] << " ";
										}
										cout << "\n";
									}
									count++;
								}
							}
						}
						break;
					}
					}
					break;
				}
				case 'f': {
					TableEntry in(false);
					switch (OP) {
					case '<': {
						if (colNum == tables[name].bst_col) {
							for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.begin();
								iter != tables[name].bst.lower_bound(in); ++iter) {
									if (!quiet) {
										for (size_t i = 0; i < iter->second.size(); ++i) {
											for (size_t j = 0; j < colIns.size(); ++j) {
												cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
											}
											cout << "\n";
										}
									}
									count = count + (int)iter->second.size();
							}
						}
						else {
							for (size_t q = 0; q < tables[name].entries.size(); ++q) {
								if (tables[name].entries[q][colNum] < false) {
									if (!quiet) {
										for (size_t k = 0; k < colIns.size(); ++k) {
											cout << tables[name].entries[q][colIns[k]] << " ";
										}
										cout << "\n";
									}
									count++;
								}
							}
						}
						break;
					}
					case '>': {
						if (colNum == tables[name].bst_col) {
							for (std::map<TableEntry, vector<int>>::iterator iter = tables[name].bst.upper_bound(in);
								iter != tables[name].bst.end(); ++iter) {
									if (!quiet) {
										for (size_t i = 0; i < iter->second.size(); ++i) {
											for (size_t j = 0; j < colIns.size(); ++j) {
												cout << tables[name].entries[iter->second[i]][colIns[j]] << " ";
											}
											cout << "\n";
										}
									}
									count = count + (int)iter->second.size();
							}
						}
						else {
							for (size_t q = 0; q < tables[name].entries.size(); ++q) {
								if (tables[name].entries[q][colNum] > false) {
									if (!quiet) {
										for (size_t k = 0; k < colIns.size(); ++k) {
											cout << tables[name].entries[q][colIns[k]] << " ";
										}
										cout << "\n";
									}
									count++;
								}
							}
						}
						break;
					}
					case '=': {
						if (colNum == tables[name].hash_col) {
							for (auto key : tables[name].hash_table) {
								if (key.first == false) {
									if (!quiet) {
										for (size_t i = 0; i < key.second.size(); ++i) {
											for (size_t j = 0; j < colIns.size(); ++j) {
												cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
											}
											cout << "\n";
										}
									}
									count = count + (int)key.second.size();
								}
							}
						}
						else if (colNum == tables[name].bst_col) {
							for (auto key : tables[name].bst) {
								if (key.first == false) {
									if (!quiet) {
										for (size_t i = 0; i < key.second.size(); ++i) {
											for (size_t j = 0; j < colIns.size(); ++j) {
												cout << tables[name].entries[key.second[i]][colIns[j]] << " ";
											}
											cout << "\n";
										}
									}
									count = count + (int)key.second.size();
								}
							}
						}
						else {
							for (size_t q = 0; q < tables[name].entries.size(); ++q) {
								if (tables[name].entries[q][colNum] == false) {
									if (!quiet) {
										for (size_t k = 0; k < colIns.size(); ++k) {
											cout << tables[name].entries[q][colIns[k]] << " ";
										}
										cout << "\n";
									}
									count++;
								}
							}
						}
						break;
					}
					}
				}

				}
				break;
			}
			}
		}
		}
	
		cout << "Printed " << count << " matching rows from " << name << "\n";

		return;
	}

	void join() {
		string name_one;
		string name_two;
		string junk;
		string colname_one;
		string colname_two;
		int cols;
		int count = 0;
		int colNum_one = -1;
		int colNum_two = -1;
		struct col_tab {
			string name;
			int table;
			int colNum;
		};
		vector<col_tab> join_cols;

		cin >> name_one;

		if (tables.find(name_one) == tables.end()) {
			cout << "Error: " << name_one << " does not name a table in the database\n";
			return;
		}

		cin >> junk;
		cin >> name_two;

		if (tables.find(name_two) == tables.end()) {
			cout << "Error: " << name_two << " does not name a table in the database\n";
			return;
		}

		cout << boolalpha;

		cin >> junk;

		cin >> colname_one;
		cin >> junk;
		cin >> colname_two;

		for (size_t t = 0; t < tables[name_one].cols.size(); ++t) {
			if (tables[name_one].cols[t] == colname_one) {
				colNum_one = (int)t;
				break;
			}
		}
		for (size_t t = 0; t < tables[name_two].cols.size(); ++t) {
			if (tables[name_two].cols[t] == colname_two) {
				colNum_two = (int)t;
				break;
			}
		}
		
		if (colNum_one == -1) {
			cout << "Error: " << colname_one << " does not name a column in " << name_one << "\n";
			return;
		}
		if (colNum_two == -1) {
			cout << "Error: " << colname_two << " does not name a column in " << name_two << "\n";
			return;
		}

		cin >> junk;
		cin >> junk;

		cin >> cols;

		for (int i = 0; i < cols; ++i) {
			col_tab temp;
			cin >> temp.name;
			cin >> temp.table;

			bool inTable = false;
			if (temp.table == 1) {
				for (size_t j = 0; j < tables[name_one].cols.size(); ++j) {
					if (temp.name == tables[name_one].cols[j]) {
						inTable = true;
						temp.colNum = (int)j;
						break;
					}
				}
				if (!inTable) {
					cout << "Error: " << temp.name << " does not name a column in " << name_one << "\n";
					return;
				}
			}
			else if (temp.table == 2) {
				for (size_t j = 0; j < tables[name_two].cols.size(); ++j) {
					if (temp.name == tables[name_two].cols[j]) {
						inTable = true;
						temp.colNum = (int)j;
						break;
					}
				}
				if (!inTable) {
					cout << "Error: " << temp.name << " does not name a column in " << name_two << "\n";
					return;
				}
			}

			join_cols.push_back(temp);

		}

		if (!quiet) {
			for (size_t c = 0; c < join_cols.size(); ++c) {
				cout << join_cols[c].name << " ";
			}
			cout << "\n";
		}

		if (tables[name_two].hash_col != colNum_two) {
			unordered_map<TableEntry, vector<int>> temp;
			temp.clear();
			for (size_t i = 0; i < tables[name_two].entries.size(); ++i) {
				TableEntry in(tables[name_two].entries[i][colNum_two]);
				temp.insert({ in, vector<int>() });
				temp[in].push_back((int)i);
			}

			for (size_t i = 0; i < tables[name_one].entries.size(); ++i) {
				if (temp.find(tables[name_one].entries[i][colNum_one]) != temp.end()) {
					if (!quiet) {
						for (size_t j = 0; j < temp[tables[name_one].entries[i][colNum_one]].size(); ++j) {
							for (size_t k = 0; k < join_cols.size(); ++k) {
								if (join_cols[k].table == 1) {
									cout << tables[name_one].entries[i][join_cols[k].colNum] << " ";
								}
								else {
									cout << tables[name_two].entries[temp[tables[name_one].entries[i][colNum_one]][j]][join_cols[k].colNum] << " ";
								}
							}
							cout << "\n";
						}
					}
					count = count + (int)temp[tables[name_one].entries[i][colNum_one]].size();
				}
			}
		}
		else {
			for (size_t i = 0; i < tables[name_one].entries.size(); ++i) {
				if (tables[name_two].hash_table.find(tables[name_one].entries[i][colNum_one]) != tables[name_two].hash_table.end()) {
					if (!quiet) {
						for (size_t j = 0; j < tables[name_two].hash_table[tables[name_one].entries[i][colNum_one]].size(); ++j) {
							for (size_t k = 0; k < join_cols.size(); ++k) {
								if (join_cols[k].table == 1) {
									cout << tables[name_one].entries[i][join_cols[k].colNum] << " ";
								}
								else {
									cout << tables[name_two].entries[tables[name_two].hash_table[tables[name_one].entries[i][colNum_one]][j]][join_cols[k].colNum] << " ";
								}
							}
							cout << "\n";
						}
					}
					count = count + (int)tables[name_two].hash_table[tables[name_one].entries[i][colNum_one]].size();
				}
			}
		}

		cout << "Printed " << count << " rows from joining " << name_one << " to " << name_two << "\n";
	}

	void remove() {
		string name;

		cin >> name;

		if (tables.find(name) == tables.end()) {
			cout << "Error: " << name << " does not name a table in the database\n";
			return;
		}

		tables[name].cols.clear();
		tables[name].entries.clear();
		tables[name].types.clear();

		tables.erase(name);

		cout << "Table " << name << " deleted\n";

		return;
	}

	void quit() {
		tables.clear();
		cout << "Thanks for being silly!\n";
		return;
	}

	void setArguments(int argc, char* argv[]) {

		opterr = true;
		int choice;
		int option_index = 0;
		option long_options[] =
		{
			{"help", no_argument, nullptr, 'h'},
			{"quiet", no_argument, nullptr, 'q'},
			{nullptr, 0, nullptr, '\0'}
		};

		while ((choice = getopt_long(argc, argv, "hq",
			long_options, &option_index)) != -1) {
			switch (choice) {
			case 'h':
				help = true;
				break;
			case 'q':
				quiet = true;
				break;
			}
		}
	}

	void rebuild_data(string name, int colNum, char type) {
		if (type == 'h') {
			tables[name].hash_table.clear();
			tables[name].hash_col = colNum;
			for (size_t i = 0; i < tables[name].entries.size(); ++i) {
				TableEntry temp(tables[name].entries[i][colNum]);
				tables[name].hash_table.insert({ temp, vector<int>() });
				tables[name].hash_table[temp].push_back((int)i);
			}
		}
		else if (type == 'b') {
			tables[name].bst.clear();
			tables[name].bst_col = colNum;
			for (size_t i = 0; i < tables[name].entries.size(); ++i) {
				TableEntry temp(tables[name].entries[i][colNum]);
				tables[name].bst.insert({ temp, vector<int>() });
				tables[name].bst[temp].push_back((int)i);
			}
		}
	}


	bool getHelp() {
		return help;
	}

};