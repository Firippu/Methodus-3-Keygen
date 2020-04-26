#include <cstdio>
#include <cstring>
#include "methhex.h"

int main(int argc,char **argv) {
	char inputID[40];
	memset(inputID,0,sizeof inputID);

	if(argc==2) strncat(inputID,argv[1],40);

	int idLength=strlen(inputID);
	if(idLength < 2 || idLength > 40) {
		printf("Error: invalid ID - bad length - keep between 2 & 40 characters\n");
		return 0;
	}

	for(int i=0; i<idLength; i++) {
		if(inputID[i] < 'a' || inputID[i] > 'z') {
			printf("Error: invalid ID - lowercase alpha only\n");
			return 0;
		}
	}

	short int inputID2[40];
	memset(inputID2,0,sizeof inputID2);

	short int Seed2[16];
	memset(Seed2,0,sizeof Seed2);

	char Seed1[16];
	memset(Seed1,0,sizeof Seed1);
	strcat(Seed1,"caapass");

	char Result[16];
	memset(Result,0,sizeof Result);

	int ResetNum=0;
	int numSeed=0;
	int Var1=0;
	for(int i=0; i<idLength; i++) {
		Var1=inputID[i]+inputID2[ResetNum];
		Var1=Var1^0x12;

		inputID2[ResetNum]=Var1;

		ResetNum++;

		numSeed+=Var1;

		switch(i) {
			case  7: ResetNum=0; break;
			case 15: ResetNum=0; break;
			case 23: ResetNum=0; break;
			case 31: ResetNum=0; break;
			case 39: ResetNum=0; break;
		}
	}

	int seedLength=strlen(Seed1);
	for(int i=0; i<seedLength; i++) {
		Seed2[ResetNum]=Seed1[i]^0x19;

		numSeed=numSeed+Seed2[ResetNum];

		ResetNum++;

		switch(ResetNum) {
			case 8: ResetNum=0; break;
		}
	}

	numSeed=numSeed&0x1FF;

	int Var2=0;
	for(int i=0; i<16; i++) {
		Seed2[i]=Seed2[i]^hexPool3[i];
		Var2=inputID2[i]^Seed2[i];
		Var2=Var2&0x1FF;
		Var2=Var2-numSeed;

		if(Var2<0x0) {  // checks if number is negative
			Var2=-Var2; // inverts negative number into positive
		} else {
			Var2=Var2^0x0;
		}

		Result[i]=(hexPool2[Var2]+48); // +48 moves the hex offset to ascii numbers
	}

	printf("Success: %.*s\n",16,Result);
    return 0;
}
