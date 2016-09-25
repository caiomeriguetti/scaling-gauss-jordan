#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

string out_buffer = "";

string input_file = "in.txt";
string output_file = "out.txt";

void print_matrix(vector< vector<float> >& matrix) {
	out_buffer.append("\n");
	out_buffer.append("\n");

	for (int i = 0; i < matrix.size(); i ++) {
		vector<float> currentLine = matrix[i];
		for (int j = 0; j < currentLine.size(); j ++) {
			float value = currentLine[j];
			out_buffer.append(to_string(value) + " ");
		}

		out_buffer.append("\n");

	}

	out_buffer.append("\n");
	out_buffer.append("\n");
}

void exchange_lines(int a, int b, vector< vector<float> >& matrix) {
	vector<float> lineA = matrix[a];
	vector<float> lineB = matrix[b];

	matrix[b] = lineA;
	matrix[a] = lineB;

	out_buffer.append("Exchange Lines " + to_string(a) + " <-> " + to_string(b));

	print_matrix(matrix);
}

void multiply_line(int line, float k, vector< vector<float> >& matrix) {
	for(int i = 0; i < matrix[line].size(); i++) {
			matrix[line][i] = matrix[line][i]*k;
	}
	
	out_buffer.append("Multiply line " + to_string(line) + " by " + to_string(k));

	print_matrix(matrix);
}

void sum_lines(int a, float a_multip, int b, float b_multip, vector< vector<float> >& matrix) {
	for(int i = 0; i < matrix[a].size(); i++) {
			matrix[a][i] = a_multip*matrix[a][i] + b_multip*matrix[b][i];
	}
	out_buffer.append("Sum lines: " + to_string(a) + " = " + to_string(a_multip) + " * " + to_string(a) + " + " + to_string(b_multip) + " * " + to_string(b));
	print_matrix(matrix);
}

int count_zeros_end(int line, vector< vector<float> >& matrix) {
	int countedZeros = 0;
	for (int j = matrix[line].size() - 2; j >= 0; j --) {
		float value = matrix[line][j];

		if (value == 0) {
			countedZeros ++;
		} else {
			break;
		}
	}

	return countedZeros;
}

int count_zeros_begin(int line, vector< vector<float> >& matrix) {
	int countedZeros = 0;
	for (int j = 0; j < matrix[line].size(); j ++) {
		float value = matrix[line][j];

		if (value == 0) {
			countedZeros ++;
		} else {
			break;
		}
	}

	return countedZeros;
}

void zeros_below(int line, vector< vector<float> >& matrix) {
	multiply_line(line, 1/matrix[line][line], matrix);

	for(int i = line + 1; i < matrix.size(); i++) {
		if (matrix[i][line] != 0) {
			sum_lines(i, 1, line, -matrix[i][line], matrix);
		}
	}
}

void zeros_above(int line, vector< vector<float> >& matrix) {
	for(int i = line - 1; i >= 0; i--) {
		sum_lines(i, 1, line, -matrix[i][line], matrix);
	}
}

void save_out () {
	cout << out_buffer;
	std::ofstream out(output_file);
	out << out_buffer;
	out.close();
}

vector< vector<float> > parse_matrix(string matrixData) {

  string numBuffer = "";
  string space = " ";
  string newLine = "\n";

  vector< vector<float> > lines {};
  vector<float> * currentLine = new vector<float>();

	for(char& c : matrixData) {
			if (space == string(1,c) || string(1, c) == newLine) {
				
				double temp = atof(numBuffer.c_str());
				
				(*currentLine).push_back(temp);

				if (string(1, c) == newLine) {

					lines.push_back((*currentLine));

					currentLine = new vector<float>();
				}

				numBuffer = "";
			} else {
				numBuffer += c;
			}
	}

	return lines;
}

void gauss (vector< vector<float> >& matrix) {
	
	print_matrix(matrix);

	// gaus

	int line = 0;
	while (line < matrix.size()) {

		for (int i = line; i < matrix.size(); i ++) {

			int countedZeros = count_zeros_begin(i, matrix);

			if (line == countedZeros) {
				if (i != line) {
					exchange_lines(line, i, matrix);
				}

				zeros_below(line, matrix);

				break;
			}
		}

		line ++;
	}

	line = 0;
	while (line < matrix.size()) {
		
		int currentLine = matrix.size() - line - 1;

		for (int i = matrix.size() - 1; i >= 0; i --) {

			int countedZeros = count_zeros_end(i, matrix);

			if (line == countedZeros) {
				if (i != currentLine) {
					exchange_lines(currentLine, i, matrix);
				}

				zeros_above(currentLine, matrix);

				break;
			}
		}

		line ++;
	}

	out_buffer += "Done";

	print_matrix(matrix);
}

int main (int argc, char* argv[]) {


	if (argc >= 2) {
		input_file = argv[1];
	}

	if (argc >= 3) {
		output_file = argv[2];
	}

	ifstream ifs(argv[1]);
  string matrixData( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>() ) );
  
  vector< vector<float> > lines = parse_matrix(matrixData);

  gauss(lines);

	save_out();

}