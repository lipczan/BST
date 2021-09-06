using namespace std;
#include <stdio.h>
#include <iostream>
#include <bitset>
#include <unordered_map>
#include <math.h>
#include <fstream>
#include <string>
#include <windows.h>

string round_keys[16];

// Parity bit drop table
int pc1[56] = { 57, 49, 41, 33, 25, 17, 9,
				 1, 58, 50, 42, 34, 26, 18,
				 10, 2, 59, 51, 43, 35, 27,
				 19, 11, 3, 60, 52, 44, 36,
				 63, 55, 47, 39, 31, 23, 15,
				 7, 62, 54, 46, 38, 30, 22,
				 14, 6, 61, 53, 45, 37, 29,
				 21, 13, 5, 28, 20, 12, 4 };

// The PC2 table
int pc2[48] = { 14,17,11,24,1,5,
				3,28,15,6,21,10,
				23,19,12,4,26,8,
				16,7,27,20,13,2,
				41,52,31,37,47,55,
				30,40,51,45,33,48,
				44,49,39,56,34,53,
				46,42,50,36,29,32 };

// The initial permutation table 
int initial_permutation[64] = {
58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,
64,56,48,40,32,24,16,8,
57,49,41,33,25,17,9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7
};
// The expansion table
int expansion_table[48] = {
32,1,2,3,4,5,4,5,
6,7,8,9,8,9,10,11,
12,13,12,13,14,15,16,17,
16,17,18,19,20,21,20,21,
22,23,24,25,24,25,26,27,
28,29,28,29,30,31,32,1
};

// The substitution boxes
int substition_boxes[8][4][16] =
{ {
	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
},
{
	15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
	3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
	0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
	13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
},
{
	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
	13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
	13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
	1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
},
{
	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
	13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
	10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
	3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
},
{
	2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
	14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
	4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
	11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
},
{
	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
	10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
	9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
	4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
},
{
	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
	13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
	1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
	6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
},
{
	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
	1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
	7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
	2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
} };

// The permutation table
int permutation_tab[32] = {
16,7,20,21,29,12,28,17,
1,15,23,26,5,18,31,10,
2,8,24,14,32,27,3,9,
19,13,30,6,22,11,4,25
};

// The inverse permutation table
int inverse_permutation[64] = {
40,8,48,16,56,24,64,32,
39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,
37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,
35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58,26,
33,1,41,9,49,17,57,25
};

//funtions//////////////////////
bool checkTextInput(string toCheck) {
	int size = toCheck.size();
	for (int i = 0; i < size; i++) {
		if (toCheck[i] < 32 && 127 < toCheck[i]) {
			return false;
		}
	}
	return true;
}

bool checkHexInput(string toCheck) {
	int size = toCheck.size();
	/*if (size != 16) {
		return false;
	}*/
	for (int i = 0; i < size; i++) {
		if ((64 < toCheck[i] && 71 > toCheck[i]) || ((47 < toCheck[i] && 58 > toCheck[i]))) {

		}
		else {
			return false;
		}
	}
	return true;
}

void printMenu() {
	cout << "For more details check readme.txt\n";
	cout << "DES Menu: \n";
	cout << "1. -> TEXT \n";
	cout << "   -> TEXT KEY\n";
	cout << "2. -> HEX \n";
	cout << "   -> HEX KEY\n";
	cout << "3. -> INPUT FROM text_data.txt (only 64 bit text and key!)\n";
	cout << "4. -> INPUT HEX FROM hex_data.txt (only 64 bit text and key!)\n";
	cout << "Choice: ";
}

string convertDecimalToBinary(int decimal) {
	string binary;
	while (decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary;
		decimal = decimal / 2;
	}
	while (binary.length() < 4) {
		binary = "0" + binary;
	}
	return binary;
}

unsigned long long convertBinaryToDecimal(string s) {
	unsigned long long value = bitset<64>(s).to_ullong();
	return value;
}

//int convertBinaryToDecimal(string n) {
//	string num = n;
//	int dec_value = 0;
//	int base = 1;
//	int len = num.length();
//
//	for (int i = len - 1; i >= 0; i--) {
//		while (num[i] > 0) {
//			if (num[i] == '1') {
//				dec_value += base;
//			}
//			base = base * 2;
//		}
//
//	}
//	return dec_value;
//}

string strToBinary(string s) {
	string bin = "";
	for (char& _char : s) {
		bin += bitset < 8 >(_char).to_string();
		//bin += " ";
	}
	return bin;
}

string hexToBin(string s) {
	// hexadecimal to binary conversion
	unordered_map < char, string > mp;
	mp['0'] = "0000";
	mp['1'] = "0001";
	mp['2'] = "0010";
	mp['3'] = "0011";
	mp['4'] = "0100";
	mp['5'] = "0101";
	mp['6'] = "0110";
	mp['7'] = "0111";
	mp['8'] = "1000";
	mp['9'] = "1001";
	mp['A'] = "1010";
	mp['B'] = "1011";
	mp['C'] = "1100";
	mp['D'] = "1101";
	mp['E'] = "1110";
	mp['F'] = "1111";
	string bin = "";
	for (int i = 0; i < s.size(); i++)
	{
		bin += mp[s[i]];
		//bin+=" ";
	}
	return bin;
}

string bin2hex(string s) {
	// binary to hexadecimal conversion
	unordered_map<string, string> mp;
	mp["0000"] = "0";
	mp["0001"] = "1";
	mp["0010"] = "2";
	mp["0011"] = "3";
	mp["0100"] = "4";
	mp["0101"] = "5";
	mp["0110"] = "6";
	mp["0111"] = "7";
	mp["1000"] = "8";
	mp["1001"] = "9";
	mp["1010"] = "A";
	mp["1011"] = "B";
	mp["1100"] = "C";
	mp["1101"] = "D";
	mp["1110"] = "E";
	mp["1111"] = "F";
	string hex = "";
	for (int i = 0; i < s.length(); i += 4) {
		string ch = "";
		ch += s[i];
		ch += s[i + 1];
		ch += s[i + 2];
		ch += s[i + 3];
		hex += mp[ch];
	}
	return hex;
}

string permute(string key, int* table, int size) {
	string permuted = "";
	for (int i = 0; i < size; i++) {
		permuted += key[table[i] - 1];
	}
	return permuted;
}

string shift_left_once(string key_chunk) {
	string shifted = "";
	for (int i = 1; i < 28; i++) {
		shifted += key_chunk[i];
	}
	shifted += key_chunk[0];
	return shifted;
}

string shift_left_twice(string key_chunk) {
	string shifted = "";
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j < 28; j++) {
			shifted += key_chunk[j];
		}
		shifted += key_chunk[0];
		key_chunk = shifted;
		shifted = "";
	}
	return key_chunk;
}

string xorStrings(string a, string b) {
	string result = "";
	for (int i = 0; i < b.size(); i++) {
		if (a[i] != b[i]) {
			result += "1";
		}
		else {
			result += "0";
		}
	}
	return result;
}

void generateKey(string binaryKey) {
	//cout << "\nAfter permute: \n";
	string permutedKey = "";
	permutedKey = permute(binaryKey, pc1, 56);
	//cout << permutedKey;

	// Splitting
	string left = permutedKey.substr(0, 28);
	string right = permutedKey.substr(28, 28);
	//cout << "\nLeft:  " << left << "\nRight: " << right << endl;

	// Generating 16 keys
	for (int i = 0; i < 16; i++) {
		if (i == 0 || i == 1 || i == 8 || i == 15) {
			left = shift_left_once(left);
			right = shift_left_once(right);
		}
		else {
			left = shift_left_twice(left);
			right = shift_left_twice(right);
		}

		// Merge keys
		string combined_key = left + right;

		round_keys[i] = permute(combined_key, pc2, 48);

		//cout << "\nKey " << i + 1 << ": " << round_keys[i] << endl;
	}
}

string encrypt(string inputText) {
	string permInputText = permute(inputText, initial_permutation, 64);
	//Splitting
	string left = permInputText.substr(0, 32);
	string right = permInputText.substr(32, 32);

	for (int i = 0; i < 16; i++) {
		string right_expanded = "";
		right_expanded = permute(right, expansion_table, 48);

		string xored = xorStrings(round_keys[i], right_expanded);
		string divided_result = "";

		for (int k = 0; k < 8; k++) {
			string row1 = xored.substr(k * 6, 1) + xored.substr(k * 6 + 5, 1);
			int row = convertBinaryToDecimal(row1);
			string col1 = xored.substr(k * 6 + 1, 1) + xored.substr(k * 6 + 2, 1) + xored.substr(k * 6 + 3, 1) + xored.substr(k * 6 + 4, 1);;
			int col = convertBinaryToDecimal(col1);
			int val = substition_boxes[k][row][col];
			divided_result += convertDecimalToBinary(val);
		}

		string secondPerm = permute(divided_result, permutation_tab, 32);

		xored = xorStrings(secondPerm, left);

		left = xored;
		if (i < 15) {
			string temp = right;
			right = xored;
			left = temp;
		}
	}

	string combined_text = left + right;
	string encryptedText = permute(combined_text, inverse_permutation, 64);

	return encryptedText;
}

string fill(string s) {
	int size = s.size();
	int rest = size % 64;
	int to_add = 64 - rest;

	if (rest != 0) {
		for (int i = 0; i < to_add; i++) {
			s += "0";
		}
	}
	return s;
}

int main() {
	int choiceNumber;

	printMenu();
	cin >> choiceNumber;

	switch (choiceNumber) {
	case 1: {
		cout << "\nInput text: ";
		string inputText;
		cin >> inputText;
		//inputText = "123456789";

		cout << "\nKey: ";
		string inputKey;
		cin >> inputKey;
		//inputKey = "abcd";
		cout << "\n";

		if (checkTextInput(inputText) && checkTextInput(inputKey) && inputKey.size() <= 8) {
			cout << "Input -> bin: \n";
			string binaryInputText = strToBinary(inputText);
			string filled_text = fill(binaryInputText);
			cout << filled_text;

			cout << "\nKey -> bin: \n";
			string binaryKey = strToBinary(inputKey);
			string filled_key = fill(binaryKey);
			cout << filled_key << endl;

			//cout << filled_key << " "<<filled_key.size();

			generateKey(filled_key);

			string encrypted_bin = "";
			unsigned long long encrypted_dec = 0;
			string encrypted_hex = "";

			double size = filled_text.size();
			double jump = ceil(size / 64);

			for (int i = 0; i < jump; i++) {
				string divided = filled_text.substr(i * 64, 64);
				//cout << divided << endl;
				string encrypted = encrypt(divided);

				encrypted_bin += encrypted;
				//encrypted_dec += convertBinaryToDecimal(encrypted);
				encrypted_hex += bin2hex(encrypted);
			}

			cout << "Encrypted (BIN): " << encrypted_bin << endl;
			//cout << "Encrypted (DEC): " << convertBinaryToDecimal(encrypted_bin) << endl;
			cout << "Encrypted (HEX): " << encrypted_hex << endl;
		}
		else {
			cout << "Wrong inputs!" << endl;
		}
		break;
	}
	case 2: {
		cout << "\nInput text: ";
		string hexInputText;
		//hexInputText = "1234AABBCCAAAAAAAAA";
		cin >> hexInputText;

		cout << "\nKey: ";
		string hexKey;
		//hexKey = "AABBCCDD";
		cin >> hexKey;
		cout << "\n";

		if (checkHexInput(hexInputText) && checkHexInput(hexKey) && hexKey.size() <= 16) {
			cout << "Input -> bin: \n";
			string binaryInputText = hexToBin(hexInputText);
			string filled_text = fill(binaryInputText);
			cout << filled_text;

			cout << "\nKey -> bin: \n";
			string binaryKey = hexToBin(hexKey);
			string filled_key = fill(binaryKey);
			cout << filled_key << endl;

			//cout << filled_key << " "<<filled_key.size();

			generateKey(filled_key);

			string encrypted_bin = "";
			//unsigned long long encrypted_dec = 0;
			string encrypted_hex = "";

			double size = filled_text.size();
			double jump = ceil(size / 64);

			for (int i = 0; i < jump; i++) {
				string divided = filled_text.substr(i * 64, 64);
				cout << divided << endl;
				string encrypted = encrypt(divided);

				encrypted_bin += encrypted;
				//encrypted_dec += convertBinaryToDecimal(encrypted);
				encrypted_hex += bin2hex(encrypted);
			}

			cout << "Encrypted (BIN): " << encrypted_bin << endl;
			//cout << "Encrypted (DEC): " << convertBinaryToDecimal(encrypted_bin) << endl;
			cout << "Encrypted (HEX): " << encrypted_hex << endl;
		}
		else {
			cout << "Wrong inputs!" << endl;
		}
		break;
	}
		  //input from file
	case 3: {
		ifstream MyFile("text_data.txt");
		string myLine;
		cout << endl;
		while (getline(MyFile, myLine)) {
			string inputText = myLine.substr(0, 8);
			string inputKey = myLine.substr(9, 16);

			if (checkTextInput(inputText) && checkTextInput(inputKey) && inputKey.size() <= 8) {
				cout << "\nInput text: ";
				cout << inputText;

				cout << "\nKey: ";
				cout << inputKey << endl;

				cout << "Input -> bin: \n";
				string filled = fill(inputText);
				string binaryInputText = strToBinary(filled);
				cout << binaryInputText;

				cout << "\nKey -> bin: \n";
				string binaryKey = strToBinary(inputKey);
				cout << binaryKey;

				cout << endl;
				generateKey(binaryKey);
				cout << endl;
				string encrypted = encrypt(binaryInputText);
				cout << "Encrypted (BIN): " << encrypted << endl;
				cout << "Encrypted (DEC): " << convertBinaryToDecimal(encrypted) << endl;
				cout << "Encrypted (HEX): " << bin2hex(encrypted) << endl;
				cout << endl;
				cout << "=======================================================================";
				cout << endl;
			}
			else {
				cout << "Wrong inputs!" << endl;
			}
		}
		MyFile.close();
		break;
	}
		  //input HEX from file
	case 4: {
		ifstream MyFile("hex_data.txt");
		string myLine;
		cout << endl;

		while (getline(MyFile, myLine)) {
			string hexInputText = myLine.substr(0, 16);
			string hexKey = myLine.substr(17, 32);

			if (checkHexInput(hexInputText) && checkHexInput(hexKey) && hexKey.size() <= 16) {
				cout << "Input -> HEX: \n";
				cout << hexInputText;

				cout << "\nKey -> HEX: \n";
				cout << hexKey;

				cout << endl;
				string binaryKey = hexToBin(hexKey);
				generateKey(binaryKey);
				cout << endl;
				string binaryInputText = hexToBin(hexInputText);
				string encrypted = encrypt(binaryInputText);
				cout << "Encrypted (BIN): " << encrypted << endl;
				cout << "Encrypted (DEC): " << convertBinaryToDecimal(encrypted) << endl;
				cout << "Encrypted (HEX): " << bin2hex(encrypted) << endl;
				cout << endl;
				cout << "=======================================================================";
				cout << endl;
			}
			else {
				cout << "Wrong inputs!" << endl;
			}
		}
		MyFile.close();


		break;
	}
	default: {
		cout << "Choose right number." << endl;
		break;
	}
		   break;
	}

	return 0;
}