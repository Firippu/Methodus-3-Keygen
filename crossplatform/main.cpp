#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

#define MAX_ID_LEN 40
#define MIN_ID_LEN 2

unsigned char ucSmallHexArray[] = {
	0x2E,0x29,0x95,0x45,
	0xE7,0x20,0x7B,0x36,
	0x1C,0x61,0xF8,0x29,
	0x88,0x29,0x4E,0xA9
};

bool bIDInvalid(char *szID,int iIDLen) {
	bool bValidLetter;
	for(int iLoopCount=0; iLoopCount<iIDLen; iLoopCount++) {
		bValidLetter=false;
		for(char cLetter='a'; cLetter<='z'; cLetter++) {
			if(cLetter==szID[iLoopCount]) {
				bValidLetter=true;
			}
		}

		if(!bValidLetter) {
			return true;
		}
	}

	return false;
}

int main() {
	char szNumberArray[513]={0};

	int szBigHexArray[513]={0};

	char szKey[17]={0};

	char szID[41]={0};
	const char szSeed[17]="caapass"; // size 17 is needed because the process xORs the left over zeros

	short int szIDAltered[41]={0};

	short int szSeedAltered[17]={0};

	int iLoopCount;
	for(iLoopCount=0; iLoopCount<=512; iLoopCount++) {
		szBigHexArray[iLoopCount]=iLoopCount;
	}

	int iIDK1=0;
	int iIDK2=0;
	for(iLoopCount=0; iLoopCount<=512; iLoopCount++) {
		(iIDK1+=iLoopCount)&=255;

		iIDK2=szBigHexArray[iLoopCount];
		szBigHexArray[iLoopCount]=szBigHexArray[iIDK1];
		szBigHexArray[iIDK1]=iIDK2;
	}

	int iReset=0;
	for(iLoopCount=0; iLoopCount<=512; iLoopCount++) {
		szNumberArray[szBigHexArray[iLoopCount]]=iReset;

		iReset++;

		if(iReset==10) {
			iReset=0;
		}
	}

	printf("Enter ID: ");
	cin >> szID;

	int iIDLen=strlen(szID);

	if(iIDLen==0) {
		cout << "no ID provided";
	} else if(szID[MIN_ID_LEN-1] == '\0') {
		cout << "too short";
	} else if(szID[MAX_ID_LEN] != '\0') {
		printf("too long");
	} else if(bIDInvalid(szID,iIDLen)) {
		printf("lowercase alphabet only");
	} else {
		int iLettersCombined=0;
		iReset=0;
		int iLetter=0;
		for(iLoopCount=0; iLoopCount<iIDLen; iLoopCount++) {
			iLetter=(szID[iLoopCount]+szIDAltered[iReset])^18;

			szIDAltered[iReset]=iLetter;

			iReset++;

			iLettersCombined+=iLetter;

			switch(iLoopCount+1) {
				case  8: iReset=0; break;
				case 16: iReset=0; break;
				case 24: iReset=0; break;
				case 32: iReset=0; break;
				case 40: iReset=0; break;
			}
		}

		int iSeedLen=strlen(szSeed);
		for(iLoopCount=0; iLoopCount<iSeedLen; iLoopCount++) {
			iLettersCombined+=(szSeedAltered[iReset]=szSeed[iLoopCount]^25);

			iReset++;
			switch(iReset) {
				case 8: iReset=0; break;
			}
		}

		iLettersCombined&=511;

		int iNumberChooser=0;
		for(iLoopCount=0; iLoopCount<16; iLoopCount++) {
			szSeedAltered[iLoopCount]^=ucSmallHexArray[iLoopCount];

			iNumberChooser=((szIDAltered[iLoopCount]^szSeedAltered[iLoopCount])&511)-iLettersCombined;

			if(iNumberChooser<0) {
				(iNumberChooser^=-1)-=-1;
			} else {
				iNumberChooser^=0;
			}

			szKey[iLoopCount]=szNumberArray[iNumberChooser];
		}

		printf("Your Key: ");
		for(iLoopCount=0; iLoopCount<16; iLoopCount++) {
			printf("%d",szKey[iLoopCount]);
		}
	}

    return 0;
}
