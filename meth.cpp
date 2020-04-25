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

	short int inputIDAltered[40];
	short int SeedAltered[16];

	unsigned int hexPool1[512];
	unsigned int hexPool2[512];
	unsigned char hexPool3[16] = {
		0x2E,0x29,0x95,0x45,
		0xE7,0x20,0x7B,0x36,
		0x1C,0x61,0xF8,0x29,
		0x88,0x29,0x4E,0xA9
	};

	char Seed[16];
	char Result[16];

	memset(hexPool2,0,sizeof hexPool2);
	memset(SeedAltered,0,sizeof SeedAltered);
	memset(hexPool1,0,sizeof hexPool1);
	memset(Result,0,sizeof Result);
	memset(inputIDAltered,0,sizeof inputIDAltered);
	memset(Seed,0,sizeof Seed);

	strcat(Seed,"caapass");

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

	int iReset=0;
	for(loopCount=0; loopCount<512; loopCount++) {
		hexPool2[hexPool1[loopCount]]=iReset;

		iReset++;

		if(iReset==10) {
			iReset=0;
		}
	}

	int iLettersCombined=0;
	iReset=0;
	int iLetter=0;
	for(loopCount=0; loopCount<idLength; loopCount++) {
		iLetter=inputID[loopCount]+inputIDAltered[iReset];
		iLetter=iLetter^0x12;

		inputIDAltered[iReset]=iLetter;

		iReset++;

		iLettersCombined+=iLetter;

		switch(loopCount) {
			case  7: iReset=0; break;
			case 15: iReset=0; break;
			case 23: iReset=0; break;
			case 31: iReset=0; break;
			case 39: iReset=0; break;
		}
	}

	int iSeedLen=strlen(Seed);
	for(loopCount=0; loopCount<iSeedLen; loopCount++) {
		SeedAltered[iReset]=Seed[loopCount]^0x19;

		iLettersCombined=iLettersCombined+SeedAltered[iReset];

		iReset++;
		switch(iReset) {
			case 8: iReset=0; break;
		}
	}

	iLettersCombined=iLettersCombined&0x1FF;

	int iNumberChooser=0;
	for(loopCount=0; loopCount<16; loopCount++) {
		SeedAltered[loopCount]=SeedAltered[loopCount]^hexPool3[loopCount];

		iNumberChooser=inputIDAltered[loopCount]^SeedAltered[loopCount];
		iNumberChooser=iNumberChooser&0x1FF;
		iNumberChooser=iNumberChooser-iLettersCombined;

		if(iNumberChooser<0x0) {		// checks if number is negative
			iNumberChooser=-iNumberChooser; // inverts negative number into positive
		} else {
			iNumberChooser=iNumberChooser^0x0;
		}

		Result[loopCount]=(hexPool2[iNumberChooser]+48); // +48 moves the hex offset to ascii numbers
	}

	printf("\tSuccess:\t%.*s\n",16,Result);

    return 0;
}
