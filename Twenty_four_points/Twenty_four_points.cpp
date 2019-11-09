#include<iostream>
#include<math.h>
#include<iomanip>
#include<string>
#include<sstream>
using namespace std;

bool hasResult = true;
char signal[3];
static int element[4];
static int sig[5] = { 0,0,0,0,0 };
static int card[4];
string poke[4];
double cache;
int phase = 0;
int ph = 0;
int loopstate = 0;
int lastPhase;

bool cutstring() {
	string str;
	getline(cin, str);
	int cardio = 0;
	for (unsigned int length = 0; length < str.length(); length++) {
		//Iterate every character in the input string.
		if (str[length] == '1') {
			if (str[length + 1] == '0') {
				card[cardio] = 10;
				cardio++;
			}
			else {
				cout << "Invalid input." << endl;
				return false;
			}
		}
		else if (str[length] == 'j' || str[length] == 'J') {
			card[cardio] = 11;
			cardio++;
		}
		else if (str[length] == 'q' || str[length] == 'Q') {
			card[cardio] = 12;
			cardio++;
		}
		else if (str[length] == 'k' || str[length] == 'K') {
			card[cardio] = 13;
			cardio++;
		}
		else if (str[length] == 'a' || str[length] == 'A') {
			card[cardio] = 1;
			cardio++;
		}
		else if (str[length] == ' ' || str[length] == '0') {
			//Empty space will be ignored. And if the number is ten, the zero will have been used, so it will be ignored as well.
			//After all, 08 == 8. [doge]
		}
		else if (int(str[length] - '0') < 10) {
			card[cardio] = int(str[length] - '0');
			cardio++;
		}
		else {
			cout << "Invalid input." << endl;
			return false;
		}
	}
	for (int i = 0; i <= 3; i++) {
		if (card[i] == 0) {
			//Make sure that the string is able to make every element non-zero.
			cout << "Invalid input." << endl;
			return false;
		}
	}
	return true;
}

void transition(int element, int nu) {
	stringstream stream;
	switch (element) {
	case 11: {
		stream << 'J';
		break;
	}
	case 12: {
		stream << 'Q';
		break;
	}
	case 13: {
		stream << 'K';
		break;
	}
	case 1: {
		stream << 'A';
		break;
	}
	default: {
		stream << element;
		break;
	}
	}
	poke[nu] = stream.str();
}

void outputResult() {
	if (hasResult) {
		for (int i = 0; i <= 3; i++) {
			transition(element[i], i);
		}
		cout << "The final result is as follows:" << endl;
		cout << "24 = ";
		switch (lastPhase) {
		case 0: {
			cout << poke[0] << signal[0] << "(" << poke[1] << signal[1] << "(" << poke[2] << signal[2] << poke[3] << "))" << endl;
			break;
		}
		case 1: {
			cout << poke[0] << signal[0] << "((" << poke[1] << signal[2] << poke[2] << ")" << signal[1] << poke[3] << ")" << endl;
			break;
		}
		case 2: {
			cout << "((" << poke[0] << signal[2] << poke[1] << ")" << signal[1] << poke[2] << ")" << signal[0] << poke[3] << endl;
			break;
		}
		case 3: {
			cout << "(" << poke[0] << signal[1] << "(" << poke[1] << signal[2] << poke[2] << "))" << signal[0] << poke[3] << endl;
			break;
		}
		case 4: {
			cout << "(" << poke[0] << signal[2] << poke[1] << ")" << signal[0] << "(" << poke[2] << signal[1] << poke[3] << ")" << endl;
			break;
		}
		}
	}
	else {
		cout << "404 Not Found" << endl;
	}
}

void sorting(int a, int b, int c, int d) {
	static int numb = 0;
	element[numb / 6] = a;
	element[3 - (numb / 2) % 4] = b;
	element[(1 + numb % 3 + numb / 6) % 4] = c;
	element[6 - numb / 6 - (3 - (numb / 2) % 4) - ((1 + numb % 3 + numb / 6) % 4)] = d;
	numb = (numb + 1) % 24;
}

double attempt(double x, double y, int n) {
	double i = 0.0;
	switch (sig[n]) {
	case 0: {
		i = x + y;
		signal[n] = '+';
		break;
	}
	case 1: {
		i = x - y;
		signal[n] = '-';
		break;
	}
	case 2: {
		i = x * y;
		signal[n] = '*';
		break;
	}
	case 3: {
		i = x / y;
		signal[n] = '/';
		break;
	}
	}
	if (sig[n + 1] == 0 && sig[n + 2] == 0) {
		sig[n] = (sig[n] + 1) % 4;
	}
	return i;
}

void blockphase() {
	//Iterate the five possible situations of the pattern of the expression.
	switch (ph) {
	case 0: {
		cache = attempt(element[0], attempt(element[1], attempt(element[2], element[3], 2), 1), 0);
		break;
	}
	case 1: {
		cache = attempt(element[0], attempt(attempt(element[1], element[2], 2), element[3], 1), 0);
		break;
	}
	case 2: {
		cache = attempt(attempt(attempt(element[0], element[1], 2), element[2], 1), element[3], 0);
		break;
	}
	case 3: {
		cache = attempt(attempt(element[0], attempt(element[1], element[2], 2), 1), element[3], 0);
		break;
	}
	case 4: {
		cache = attempt(attempt(element[0], element[1], 2), attempt(element[2], element[3], 1), 0);
		break;
	}
	}
	lastPhase = ph;
	ph = ((++phase) / 64) % 5;
	phase = phase % 320;
}

int main() {
	cout << "This is a mathematic game \"24 Points\"." << endl << "Input number 2~10 or A,J,Q,K in one line and then enter." << endl;
	while (!cutstring()) {}
	int loopss = 0;
	do {
		sorting(card[0], card[1], card[2], card[3]);
		do {
			blockphase();
			loopstate = (loopstate + 1) % 320;
		} while (loopstate != 0 && cache != 24.0);
		if (loopss == 24) {
			hasResult = false;
		}
	} while (cache != 24.0 && loopss != 24);
	outputResult();
	return 0;
}
