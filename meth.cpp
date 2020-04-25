#include <cstdio>
#include <cstring>

int main(int argc,char **argv) {
	char inputID[40];
	memset(inputID,0,sizeof inputID);
	if(argc==2) strcat(inputID,argv[1]);

	int idLength=strlen(inputID);
	if(idLength < 2 || idLength > 40) {
		printf("\tError:\tinvalid ID - bad length - keep between 2 & 40 characters\n");
		return 0;
	}

	for(int i=0; i<idLength; i++) {
		if(inputID[i] < 'a' || inputID[i] > 'z') {
			printf("\tError:\tinvalid ID - lowercase alpha only\n");
			return 0;
		}
	}

	short int inputID2[40];
	short int Seed2[16];

	unsigned int hexPool1[512];
	unsigned int hexPool2[512];
	unsigned char hexPool3[16] = {
		0x2E,0x29,0x95,0x45,
		0xE7,0x20,0x7B,0x36,
		0x1C,0x61,0xF8,0x29,
		0x88,0x29,0x4E,0xA9
	};

	char Seed1[16];
	char Result[16];

	memset(hexPool2,0,sizeof hexPool2);
	memset(Seed2,0,sizeof Seed2);
	memset(hexPool1,0,sizeof hexPool1);
	memset(Result,0,sizeof Result);
	memset(inputID2,0,sizeof inputID2);
	memset(Seed1,0,sizeof Seed1);

	strcat(Seed1,"caapass");

	int loopCount;
	for(loopCount=0; loopCount<512; loopCount++) {
		hexPool1[loopCount]=loopCount;
	}

	int Var1=0;
	int Var2=0;
	for(loopCount=0; loopCount<512; loopCount++) {
		Var1=Var1+loopCount;
		Var1=Var1&0xFF;
		Var2=hexPool1[loopCount];
		hexPool1[loopCount]=hexPool1[Var1];
		hexPool1[Var1]=Var2;
	}

	int Resetter=0;
	for(loopCount=0; loopCount<512; loopCount++) {
		hexPool2[hexPool1[loopCount]]=Resetter;

		Resetter++;

		if(Resetter==10) {
			Resetter=0;
		}
	}

	Resetter=0;
	int Var3=0;
	int Var4=0;
	for(loopCount=0; loopCount<idLength; loopCount++) {
		Var4=inputID[loopCount]+inputID2[Resetter];
		Var4=Var4^0x12;

		inputID2[Resetter]=Var4;

		Resetter++;

		Var3+=Var4;

		switch(loopCount) {
			case  7: Resetter=0; break;
			case 15: Resetter=0; break;
			case 23: Resetter=0; break;
			case 31: Resetter=0; break;
			case 39: Resetter=0; break;
		}
	}

	int seedLength=strlen(Seed1);
	for(loopCount=0; loopCount<seedLength; loopCount++) {
		Seed2[Resetter]=Seed1[loopCount]^0x19;

		Var3=Var3+Seed2[Resetter];

		Resetter++;

		switch(Resetter) {
			case 8: Resetter=0; break;
		}
	}

	Var3=Var3&0x1FF;

	int Var5=0;
	for(loopCount=0; loopCount<16; loopCount++) {
		Seed2[loopCount]=Seed2[loopCount]^hexPool3[loopCount];

		Var5=inputID2[loopCount]^Seed2[loopCount];
		Var5=Var5&0x1FF;
		Var5=Var5-Var3;

		if(Var5<0x0) {		// checks if number is negative
			Var5=-Var5; // inverts negative number into positive
		} else {
			Var5=Var5^0x0;
		}

		Result[loopCount]=(hexPool2[Var5]+48); // +48 moves the hex offset to ascii numbers
	}

	printf("\tSuccess:\t%.*s\n",16,Result);
    return 0;
}
