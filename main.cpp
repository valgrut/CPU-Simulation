#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

#define DEBUG
#undef DEBUG

using namespace std;

enum{INSTRUCTION, PARAM1, PARAM2};

// definice funkci
int toInt(char num);
int getInstruction(int address, int n);
void setRegister(int reg, int value);
void printRegisters();

// globalni RAM a REGISTER kvuli jednodussimu pouziti ve funkcich
int program[] = {299, 492, 495, 399, 492, 495, 399, 283, 279, 689, 78, 100, 000, 000, 000};
vector<int> RAM(1000, 000);
vector<int> REGISTER(10, 000);

int main()
{
	// program bude cten ze stdin (souboru)
	
	// nahrani programu z pole do RAM
	for(int i = 0; i < 15; i++)
	{
		RAM[i] = program[i];
	}

	// indikace ukonceni programu
	bool halt = false;

	// pocet provedenych instrukci
	unsigned int count = 0;

	// reprezentace instrukce
	unsigned int instruction = 0; 
	unsigned int param1      = 0;
	unsigned int param2      = 0;

	// adresa do RAM
	unsigned int address     = 0;
	
	// samotne provadeni programu
	while(halt == false)
	{
		// ziskam instrukci v RAM a parametry na adrese address
		assert(address < 1000);	

		#ifdef DEBUG
		cout << "Provadena instrukce: " << RAM[address] << endl;
		printRegisters();
		#endif

		//pro instrukce zacinajici 0 (078)
		if(RAM[address] < 100)
		{
			instruction = 0;
			param1	    = getInstruction(address, 0);
			param2      = getInstruction(address, 1);

		}
		else
		{
			instruction = getInstruction(address, INSTRUCTION);
			param1	    = getInstruction(address, PARAM1);
			param2      = getInstruction(address, PARAM2);
		}

		switch(instruction)
		{
			case 1:
				cout << "Program proveden" << endl;
				halt = true;
				count++;	
				break;

			case 2:
				assert(param2 < 10);
				REGISTER[param1] = param2;
				count++;	
				break;

			case 3:
				REGISTER[param1] += param2;
				// vsude, kde by mohlo dojit k preteceni 3 mistneho cisla, 
				// je pouzito modulo 1000
				REGISTER[param1] %= 1000;
				count++;	
				break;

			case 4:
				REGISTER[param1] *= param2;
				REGISTER[param1] %= 1000;
				count++;	
				break;

			case 5:
				REGISTER[param1] = REGISTER[param2];
				count++;	
				break;

			case 6:
				REGISTER[param1] += REGISTER[param2];
				REGISTER[param1] %= 1000;
				count++;	
				break;

			case 7:
				REGISTER[param1] *= REGISTER[param2];
				REGISTER[param1] %= 1000;
				count++;	
				break;

			case 8:
				REGISTER[param1] = RAM[REGISTER[param2]];
				count++;	
				break;

			case 9:
				RAM[REGISTER[param2]] = REGISTER[param1];
				count++;	
				break;

			case 0:
				count++;
				if(REGISTER[param2] != 0)
				{
					address = REGISTER[param1]; 
					continue;
				}
				break;
		}

		address++;
	}		

	#ifdef DEBUG
	printRegisters();
	#endif

	cout << "Celkovy pocet provedenych instrukci: " << count << endl;

	return 0;
}

void printRegisters()
{
	int i = 0;
	for(vector<int>::iterator iter = REGISTER.begin(); iter != REGISTER.end(); ++iter, i++)
	{
		cout << i << " = " << *iter << endl;
	}
}

void setRegister(int reg, int value)
{
	REGISTER[reg] = value;
}

int toInt(char num)
{
	if(isdigit(num))
	{
		return num - '0';
	}
	return -1;
}

int getInstruction(int address, int n)
{
	assert(n < 3);
	
	string scommand = to_string( RAM[address] );
	return toInt(scommand[n]);
}

