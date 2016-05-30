#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WindowProcedure(HWND,UINT,WPARAM,LPARAM);

#define MAX_NAME_LEN 40
#define MIN_NAME_LEN 2

HWND hTXT_USER,
	 hTXT_PASS;

bool isAlphabet;

short int szWeirdPool[513];
char szNumberPool[513];
char szName[41];
char szCaapass[17]="caapass"; // 17 needed because the process xORs the left over zeros
char szSerial[17];

short int szNameStore[41];
short int szCaapassStore[17];
short int letter;

int iLoopCount,
	eax,
	VAR3,
	reset,
	iNameLen,
	iCaapassLen;

unsigned char hexPool[] = {
	0x2E,0x29,0x95,0x45,
	0xE7,0x20,0x7B,0x36,
	0x1C,0x61,0xF8,0x29,
	0x88,0x29,0x4E,0xA9
};

void GoGoGo() {
	memset(szSerial,0,sizeof szSerial);
	memset(szName,0,sizeof szName);
	memset(szNameStore,0,sizeof szNameStore);
	memset(szCaapassStore,0,sizeof szCaapassStore);
	Edit_GetText(hTXT_USER,szName,sizeof szName);

	iNameLen=strlen(szName);

	for(iLoopCount=0; iLoopCount<iNameLen; iLoopCount++) {
		isAlphabet=false;
		for(letter='a'; letter<='z'; letter++) {
			if(letter==szName[iLoopCount]) {
				isAlphabet=true;
			}
		}

		if(!isAlphabet) {
			Edit_SetText(hTXT_PASS,"alphabet only");
			return;
		}
	}

	if(iNameLen==0) {
		Edit_SetText(hTXT_PASS,"");
		return;
	} else if(szName[MIN_NAME_LEN-1] == '\0') {
		Edit_SetText(hTXT_PASS,"too short");
		return;
	} else if(szName[MAX_NAME_LEN] != '\0') {
		Edit_SetText(hTXT_PASS,"too long");
		return;
	}

	VAR3=0; // VAR3 is each letter of name and caapass added together
	reset=0;
	letter=0;
	for(iLoopCount=0; iLoopCount<iNameLen; iLoopCount++) {
		letter=(szName[iLoopCount]+szNameStore[reset])^18;

		szNameStore[reset]=letter;

		reset++;

		VAR3+=letter;

		switch(iLoopCount+1) {
			case  8: reset=0; break;
			case 16: reset=0; break;
			case 24: reset=0; break;
			case 32: reset=0; break;
			case 40: reset=0; break; // not really needed since name has max of 40 chars
		}
	}

	for(iLoopCount=0; iLoopCount<iCaapassLen; iLoopCount++) {
		VAR3+=(szCaapassStore[reset]=szCaapass[iLoopCount]^25);

		reset++;
		switch(reset) {
			case 8: reset=0; break; // cases 8,16,32,40 needed if capass was longer
		}
	}

	VAR3&=511;

	for(iLoopCount=0; iLoopCount<16; iLoopCount++) {
		szCaapassStore[iLoopCount]=szCaapassStore[iLoopCount]^hexPool[iLoopCount];

		eax=((szNameStore[iLoopCount]^szCaapassStore[iLoopCount])&511)-VAR3;

		if(eax<0) {
			eax=(eax^0xFFFFFFFF)-0xFFFFFFFF;
		} else {
			eax=(eax^0x00000000)-0x00000000;
		}

		static char szBuffer[1];
		itoa(szNumberPool[eax],szBuffer,10);
		szSerial[iLoopCount]=szBuffer[0];
	}

	Edit_SetText(hTXT_PASS,szSerial);
}

int WINAPI WinMain(HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nCmdShow) {
	MSG messages;
	WNDCLASSEX wincl;

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = "METH3KEYGEN";
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = 0;
	wincl.cbSize = sizeof(WNDCLASSEX);

	wincl.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL,IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH)16;

	if(!RegisterClassEx(&wincl))
		return 0;

	int cx = GetSystemMetrics(SM_CXSCREEN)/3;
	int cy = GetSystemMetrics(SM_CYSCREEN)/3;

	HWND hwnd = CreateWindowEx(0,wincl.lpszClassName,"Methodus 3 Keygen",(WS_OVERLAPPEDWINDOW | WS_SYSMENU) & ~(WS_MAXIMIZEBOX | WS_THICKFRAME),cx,cy,260,105,HWND_DESKTOP,NULL,hThisInstance,NULL);

	hTXT_USER = CreateWindowEx(WS_EX_CLIENTEDGE,"edit","",ES_LEFT | ES_LOWERCASE | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE,40,10,200,24,hwnd,(HMENU)(103),(HINSTANCE) GetWindowLong (hwnd,GWL_HINSTANCE),NULL);
	hTXT_PASS = CreateWindowEx(WS_EX_CLIENTEDGE,"edit","",ES_READONLY | ES_LEFT | WS_CHILD | WS_VISIBLE,40,45,200,24,hwnd,(HMENU)(104),(HINSTANCE) GetWindowLong (hwnd,GWL_HINSTANCE),NULL);

	Edit_LimitText(hTXT_USER,40);

	iCaapassLen=strlen(szCaapass);

	memset(szWeirdPool,0,sizeof szWeirdPool);
	memset(szNumberPool,0,sizeof szNumberPool);
	iLoopCount=0;

	// Start of First Loop for Number Pool creation
	for(iLoopCount=0; iLoopCount<=512; iLoopCount++) {
		szWeirdPool[iLoopCount]=iLoopCount;
	}
	// End of First Loop for Number Pool creation

	int VAR4=0;
	int VAR5=0;
	// Start of Second Loop for Number Pool creation
	for(iLoopCount=0; iLoopCount<=512; iLoopCount++) {
		(VAR4+=iLoopCount)&=255;

		VAR5=szWeirdPool[iLoopCount];
		szWeirdPool[iLoopCount]=szWeirdPool[VAR4];
		szWeirdPool[VAR4]=VAR5;
	}
	// End of Second Loop for Number Pool creation

	short int Local_58=0;
	// Start of Third Loop for Number Pool creation
	for(iLoopCount=0; iLoopCount<=512; iLoopCount++) {
		szNumberPool[szWeirdPool[iLoopCount]]=Local_58;

		Local_58++;

		if(Local_58==10) {
			Local_58=0;
		}
	}
	// End of Third Loop for Number Pool creation

	ShowWindow(hwnd,nCmdShow);

	while(GetMessage(&messages,NULL,0,0)) {
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam) {
	switch(message) {
		case WM_COMMAND:
			if(HIWORD(wParam) == EN_CHANGE && LOWORD(wParam)==103) {
				GoGoGo();
			} break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hwnd,&ps);
			SetBkMode(hdc,TRANSPARENT);
			TextOut(hdc,10,15,"ID",2);
			TextOut(hdc,5,48,"Key",3);
			EndPaint(hwnd,&ps);
		break;
		default:
			return DefWindowProc(hwnd,message,wParam,lParam);
	}

	return 0;
}
