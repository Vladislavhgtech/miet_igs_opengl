

#include "stdafx.h"
#include <windows.h>     
#include "Resource.h"     

#include <math.h>
#include <stdio.h>
#include <tchar.h>


//������������� ����������
HINSTANCE hInstApp;


//������� ������ �����
int    Norder;
//����� ������� �����
int    Npoints;
bool cube = false;

int NOrder; // ����������� ����� �����
int NPoints;

BOOL CALLBACK DialProc(HWND, UINT, WPARAM, LPARAM);

//�������� ������� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void SplineDC(HWND);
void SplineLButtonDown();
void SplineLButtonUp();
void SplineCreate(HWND hWnd);
void SplineMouseMove(HWND hWnd, int x, int y);
void SplineMouseMove1(HWND hWnd, int x, int y);
void LineDC(HWND hwnd);
//void SplineDC(HWND);
void SplineDC1(HWND);
void LineDC1(HWND hwnd);



int WINAPI WinMain(HINSTANCE hInstance,       //������������� ����������
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdParam,
	int       nCmdShow)
{

	WCHAR cname[] = L"Spline";     // ��� ������ ���� 
	WCHAR title[] = L"  Interactive Control   to    Bezie Spline Curves";  // ��������� ����

	MSG msg;    // ��������� ��� ������ � �����������

	//����������� ������ ���� ����������
	//------------------------------------------------------------------------
	WNDCLASS wc;  // ��������� ��� ����������� ������ ���� ����������

	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;         //����� ������� ����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;       //������������� ����������
	wc.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszMenuName = (LPCTSTR)MENU1;
	wc.lpszClassName = cname;

	if (!RegisterClass(&wc))  //����������� ������ ���� ����������
		return 0;

	hInstApp = hInstance;

	//�������� ���� ����������
	//------------------------------------------------------------------------
	HWND hWnd;       // ���������� ���� ����������

	hWnd = CreateWindow(cname,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,         //������������� ����-��������     
		NULL,          //������������� ���� ��������� ����
		hInstance,     //������������� ����������
		NULL);

	if (!hWnd)
		return 0;



	// ������ ����. ��� ����� ����� ������� ShowWindow,
	// ��������  ����, �������� ������� UpdateWindows,
	// ���������� ��������� WM_PAINT � ������� ����
	//--------------------------------------------------------------------------
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// ��������� ���� ��������� ���������
	//-------------------------------------------------------------------------

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;


}
//������� ����������� ����
BOOL CALLBACK DialProc(HWND hwndDlg, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	char S[10];
	//TCHAR S[10];
	switch (uMsg)
	{
		//��������� ��� �������� ����������� ����
	case WM_INITDIALOG:
		SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), (LPCWSTR)"1");
		sprintf_s(S, "%d", 1);
		//swprintf_s(S, 20, _T("%d"), 1);
		SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), (LPCWSTR)S);
		break;

		//��������� ��� �������� ����������� ����
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		break;

		//���������, ���������� �� ��������� ����������� ����
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)  //������ ������
		{
			EndDialog(hwndDlg, 0);
		}
		if (LOWORD(wParam) == IDOK)  //������ ������
		{
			GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), (LPWSTR)S, 10);
			NOrder = atoi(S);
			//Norder = _wtoi(S);
			if (NOrder > 20)
				NOrder = 20;
			EndDialog(hwndDlg, 1);
		}
		break;
	}
	return 0;
}

int EndDial;

//������� ����, �������������� ���������
LRESULT CALLBACK  WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	//���������� �����
	int x, y;
	//�������� ����������� ����� �������� ���������� ����
	//	int EndDial;
	switch (message)
	{
		//��������� ��� �������� ����
	case WM_CREATE:
		SplineCreate(hWnd);  //����� �� ����� ���������
		break;

		//��������� ��� ������� ����� ������� �����
	case WM_LBUTTONDOWN:
		SplineLButtonDown();
		break;

		//��������� ��� ���������� ����� ������� �����
	case WM_LBUTTONUP:
		SplineLButtonUp();
		break;

		//��������� ��� �������� �����
	case WM_MOUSEMOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (cube){
			SplineMouseMove(hWnd, x, y);
		}
		else{
			SplineMouseMove1(hWnd, x, y);
		}
		break;

		//��������� ��� �������� ����
	case WM_DESTROY:
		PostQuitMessage(0);  //����� �� ����� ���������
		break;

		//��������� �� ������ ����
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EXIT:
			PostQuitMessage(0);  //����� �� ����� ���������
			break;

		case ID_CLEAN:
			InvalidateRect(hWnd, NULL, TRUE);  //�������� ��������� WM_PAINT
			break;



		case ID_CUBIC:
			Norder = 3;
			SplineDC(hWnd);
			cube = true;
			break;
		case ID_GENERAL:
			Norder = 3;
			cube = false;
			EndDial = DialogBox(hInstApp, (LPCTSTR)IDD_DIALOG1, hWnd, DialProc);
			if (EndDial)
				SplineDC1(hWnd);
			break;

		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;

}






struct POINT2 {
	double x, y;
};

//������ ������� �����
POINT2 V[4];
POINT2 Vt[20];

//������� ���� ������ � ������� ������� ���������
double xLeft, xRight, yBottom, yTop;
//������� ���� ������ � �������� �� ������ ��������
int    nLeft, nRight, mBottom, mTop;

//�����
int flagDown, flagUp, flagLock;
//����� ����������� ������� �����
int Nlock;


//������� �� x � ������� n
inline int xn(double x)
{
	return (int)((x - xLeft) / (xRight - xLeft)*(nRight - nLeft)) + nLeft;
}

//������� �� y � ������� m
inline int ym(double y)
{
	return (int)((y - yBottom) / (yTop - yBottom)*(mTop - mBottom)) + mBottom;
}


double Bezie(int i, double t)
{
	double b;
	switch (i)
	{
	case 0: b = (1 - t)*(1 - t)*(1 - t); break;
	case 1: b = 3 * t*(1 - t)*(1 - t);   break;
	case 2: b = 3 * t*t*(1 - t);       break;
	case 3: b = t*t*t;             break;
	}
	return b;
}


//���������� ����� ���������� ������
POINT2 splineBezie(int N, double t)
{
	POINT2 S;
	S.x = 0; S.y = 0;
	for (int i = 0; i <= N; i++)
	{
		double Bez = Bezie(i, t);
		S.x += V[i].x*Bez;
		S.y += V[i].y*Bez;
	}
	return S;
}

//������������ ��������� WM_LBUTTONDOWN
void SplineLButtonDown()
{
	flagUp = 0;
	flagDown = 1;
}

//������������ ��������� WM_LBUTTONUP
void SplineLButtonUp()
{
	flagUp = 1;
	flagDown = 0;
	flagLock = 0;
}



//������������ ��������� WM_CREATE
void SplineCreate(HWND hWnd)
{


	flagDown = 0; flagUp = 0; flagLock = 0;
}


//������� �� ������� n � x 
inline double nx(int n)
{
	return (n - nLeft)*(xRight - xLeft) / (nRight - nLeft) + xLeft;
}

//������� �� ������� m � y 
inline double my(int m)
{
	return (m - mBottom)*(yTop - yBottom) / (mTop - mBottom) + yBottom;
}


void SplineMouseMove(HWND hwnd, int x, int y)
{
	if (flagDown)
	{
		if (!flagLock)
			for (int i = 0; i<Npoints; i++)
				if (abs(x - xn(V[i].x)) + abs(y - ym(V[i].y))<10)
				{
					Nlock = i;
					flagLock = 1;
				}
		if (flagLock)
		{
			V[Nlock].x = nx(x);  V[Nlock].y = my(y);
			LineDC(hwnd);
		}
	}
}
void SplineMouseMove1(HWND hwnd, int x, int y)
{
	if (flagDown)
	{
		if (!flagLock)
			for (int i = 0; i<NPoints; i++)
				if (abs(x - xn(Vt[i].x)) + abs(y - ym(Vt[i].y))<10)
				{
					Nlock = i;
					flagLock = 1;
				}
		if (flagLock)
		{
			Vt[Nlock].x = nx(x);  Vt[Nlock].y = my(y);
			LineDC1(hwnd);
		}
	}
}





//������� �������������� ��������� WM_COMMON
void SplineDC(HWND hwnd)
{


	RECT rc;
	GetClientRect(hwnd, &rc); //������� ������� ���������� ������� ���� <hwnd>

	//������� ���� � ������� ����������� � � ��������
	xLeft = -10; xRight = 10; yBottom = -6; yTop = 6;
	nLeft = rc.left; nRight = rc.right; mBottom = rc.bottom; mTop = rc.top;

	Npoints = Norder + 1;
	//������ ������� ����� � ������� ���������� ��������� �����
	double kr = 0.75;
	for (int i = 0; i<Npoints; i++)
	{
		V[i].x = kr*(xLeft + rand()*(xRight - xLeft) / RAND_MAX);
		V[i].y = kr*(yBottom + rand()*(yTop - yBottom) / RAND_MAX);
	}




	HDC hdc;            //������������ ��������� ����������
	hdc = GetDC(hwnd);  //�������� �������� ���������� <hdc> ��� ���� <hwnd>


	HPEN hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	HPEN hpenOld = (HPEN)SelectObject(hdc, hpen1);

	int nb, ne, mb, me;

	//������ ��� OX
	nb = xn(xLeft); mb = ym(0);
	MoveToEx(hdc, nb, mb, NULL);
	ne = xn(xRight); me = ym(0);
	LineTo(hdc, ne, me);

	//������ ��� OY
	nb = xn(0); mb = ym(yBottom);
	MoveToEx(hdc, nb, mb, NULL);
	ne = xn(0); me = ym(yTop);
	LineTo(hdc, ne, me);



	HPEN hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, hpen2);
	//������ ������� ����� �� ������� ������

	MoveToEx(hdc, xn(V[0].x), ym(V[0].y), 0);
	for (int i = 1; i < Npoints; i++)
		LineTo(hdc, xn(V[i].x), ym(V[i].y));


	HPEN hpen3 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	SelectObject(hdc, hpen3);
	//������ ������ �����
	int Nt = 10 * Norder;
	double t, dt = 1.0 / (Nt - 1);
	MoveToEx(hdc, xn(splineBezie(Norder, 0).x), ym(splineBezie(Norder, 0).y), 0);
	for (int p = 1; p < Nt; p++)
	{
		t = dt*p;
		LineTo(hdc, xn(splineBezie(Norder, t).x), ym(splineBezie(Norder, t).y));
	}

	HPEN hpen4 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	SelectObject(hdc, hpen4);
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	//������ ������ ����� � ���� �������
	for (int i = 0; i < Npoints; i++){
		Ellipse(hdc, xn(V[i].x) - 4, ym(V[i].y) - 4, xn(V[i].x) + 4, ym(V[i].y) + 4);
		HPEN hpen4 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		SelectObject(hdc, hpen4);
		TextOut(hdc, xn(V[i].x) - 6, ym(V[i].y) - 6, TEXT("" + (i + 1)), 20);
	}




	SelectObject(hdc, hbrushOld);
	SelectObject(hdc, hpenOld);
	DeleteObject(hpen1);
	DeleteObject(hpen2);
	DeleteObject(hpen3);
	DeleteObject(hpen4);
	DeleteObject(hbrush);


	ReleaseDC(hwnd, hdc);

}

/*
1 - 4
2 - 7
3 - 10
4 - 13
5 - 16
NOrder
NPoints = (NOrder - 1)*3 + 4
*/
HPEN blue = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
HPEN green = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));


void SplineDC1(HWND hwnd) {
	HDC hdc;            //������������ ��������� ����������
	hdc = GetDC(hwnd);  //�������� �������� ���������� <hdc> ��� ���� <hwnd>


	
	RECT rc;
	GetClientRect(hwnd, &rc); //������� ������� ���������� ������� ���� <hwnd>

	//������� ���� � ������� ����������� � � ��������
	xLeft = -10; xRight = 10; yBottom = -6; yTop = 6;
	nLeft = rc.left; nRight = rc.right; mBottom = rc.bottom; mTop = rc.top;

	NPoints = (NOrder - 1) * 3 + 4;


	//������ ������� ����� � ������� ���������� ��������� �����
	double kr = 0.75;
	for (int i = 0; i < NPoints-1; i++)
	{
		Vt[i].x = kr*(xLeft + rand()*(xRight - xLeft) / RAND_MAX);
		Vt[i].y = kr*(yBottom + rand()*(yTop - yBottom) / RAND_MAX);
	}






	for (int j = 0; j < NPoints -1 ; j += 3) {

		for (int i = 0; i<Npoints; i++)
		{
			V[i].x = Vt[j + i].x;
			V[i].y = Vt[j + i].y;
		}

		HPEN hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		HPEN hpenOld = (HPEN)SelectObject(hdc, hpen1);

		int nb, ne, mb, me;

		//������ ��� OX
		nb = xn(xLeft); mb = ym(0);
		MoveToEx(hdc, nb, mb, NULL);
		ne = xn(xRight); me = ym(0);
		LineTo(hdc, ne, me);

		//������ ��� OY
		nb = xn(0); mb = ym(yBottom);
		MoveToEx(hdc, nb, mb, NULL);
		ne = xn(0); me = ym(yTop);
		LineTo(hdc, ne, me);



		HPEN hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		SelectObject(hdc, hpen2);

		//������ ������� ����� �� ������� ������
		MoveToEx(hdc, xn(V[0].x), ym(V[0].y), 0);
		for (int i = 1; i < Npoints; i++)
			LineTo(hdc, xn(V[i].x), ym(V[i].y));

		
		SelectObject(hdc, green);
		if (j % 2 == 0) {
			SelectObject(hdc, blue);
		}
			

		//������ ������ �����
		int Nt = 10 * Norder;
		double t, dt = 1.0 / (Nt - 1);
		MoveToEx(hdc, xn(splineBezie(Norder, 0).x), ym(splineBezie(Norder, 0).y), 0);
		for (int p = 1; p < Nt; p++)
		{
			t = dt*p;
			LineTo(hdc, xn(splineBezie(Norder, t).x), ym(splineBezie(Norder, t).y));
		}

		HPEN hpen4 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		SelectObject(hdc, hpen4);
		HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
		//������ ������ ����� � ���� �������
		for (int i = 0; i < Npoints; i++){
			Ellipse(hdc, xn(V[i].x) - 4, ym(V[i].y) - 4, xn(V[i].x) + 4, ym(V[i].y) + 4);
		}
		SelectObject(hdc, hbrushOld);
		SelectObject(hdc, hpenOld);
		DeleteObject(hpen1);
		DeleteObject(hpen2);
		DeleteObject(hpen4);
		DeleteObject(hbrush);
	}
	
	ReleaseDC(hwnd, hdc);
}

void LineDC(HWND hwnd)
{

	HDC hdc = GetDC(hwnd);  //�������� �������� ���������� <hdc> ��� ���� <hwnd>

	//������� �������� ������ hdcMem, ����������� � ���������� ������ hdc
	HDC hdcMem = CreateCompatibleDC(hdc);
	//������� (������) ������� ����� ���������� � ���������� ������
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, nRight, mBottom);
	//�������� ������� ����� hBmp � �������� ������ hdcMem
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hdcMem, hBmp);

	HPEN hpen0 = CreatePen(PS_SOLID, 2, RGB(0xC0, 0xC0, 0xC0));
	HPEN hpenOld = (HPEN)SelectObject(hdcMem, hpen0);
	HBRUSH hbrush0 = CreateSolidBrush(RGB(0xC0, 0xC0, 0xC0));
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdcMem, hbrush0);

	Rectangle(hdcMem, 0, 0, nRight, mBottom);


	HPEN hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	SelectObject(hdcMem, hpen1);

	int nb, mb, ne, me;
	//������ ��� OX
	nb = xn(xLeft); mb = ym(0);
	MoveToEx(hdcMem, nb, mb, NULL);
	ne = xn(xRight); me = ym(0);
	LineTo(hdcMem, ne, me);

	//������ ��� OY
	nb = xn(0); mb = ym(yBottom);
	MoveToEx(hdcMem, nb, mb, NULL);
	ne = xn(0); me = ym(yTop);
	LineTo(hdcMem, ne, me);

	HPEN hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdcMem, hpen2);
	//������ ������� ����� �� ������� ������
	MoveToEx(hdcMem, xn(V[0].x), ym(V[0].y), 0);
	for (int i = 1; i < Npoints; i++)
		LineTo(hdcMem, xn(V[i].x), ym(V[i].y));

	HPEN hpen3 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	SelectObject(hdcMem, hpen3);

	//������ ������ �����
	int Nt = 10 * Norder;
	double t, dt = 1.0 / (Nt - 1);
	MoveToEx(hdcMem, xn(splineBezie(Norder, 0).x), ym(splineBezie(Norder, 0).y), 0);
	for (int p = 1; p < Nt; p++)
	{
		t = dt*p;
		LineTo(hdcMem, xn(splineBezie(Norder, t).x), ym(splineBezie(Norder, t).y));
	}


	HPEN hpen4 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	SelectObject(hdcMem, hpen4);
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdcMem, hbrush);
	//������ ������ ����� � ���� �������
	for (int i = 0; i < Npoints; i++){
		Ellipse(hdcMem, xn(V[i].x) - 4, ym(V[i].y) - 4, xn(V[i].x) + 4, ym(V[i].y) + 4);
		//TextOut(hdc, xn(V[i].x) - 6, ym(V[i].y) - 6, TEXT("" + (i + 1)), 20);
	}

	SelectObject(hdcMem, hbrushOld);
	DeleteObject(hbrush0);
	DeleteObject(hbrush);

	SelectObject(hdcMem, hpenOld);
	DeleteObject(hpen0);
	DeleteObject(hpen1);
	DeleteObject(hpen2);
	DeleteObject(hpen3);
	DeleteObject(hpen4);
	//�������� �������� ������ � �������� ������
	BitBlt(hdc, 0, 0, nRight, mBottom, hdcMem, 0, 0, SRCCOPY);

	DeleteDC(hdcMem);

	ReleaseDC(hwnd, hdc);

}

void LineDC1(HWND hwnd)
{
	
	HDC hdc = GetDC(hwnd);  //�������� �������� ���������� <hdc> ��� ���� <hwnd>

	//������� �������� ������ hdcMem, ����������� � ���������� ������ hdc
	HDC hdcMem = CreateCompatibleDC(hdc);
	//������� (������) ������� ����� ���������� � ���������� ������
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, nRight, mBottom);
	//�������� ������� ����� hBmp � �������� ������ hdcMem
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hdcMem, hBmp);

	HPEN hpen0 = CreatePen(PS_SOLID, 2, RGB(0xC0, 0xC0, 0xC0));
	HPEN hpenOld = (HPEN)SelectObject(hdcMem, hpen0);
	HBRUSH hbrush0 = CreateSolidBrush(RGB(0xC0, 0xC0, 0xC0));
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdcMem, hbrush0);

	Rectangle(hdcMem, 0, 0, nRight, mBottom);


	HPEN hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	SelectObject(hdcMem, hpen1);

	int nb, mb, ne, me;
	//������ ��� OX
	nb = xn(xLeft); mb = ym(0);
	MoveToEx(hdcMem, nb, mb, NULL);
	ne = xn(xRight); me = ym(0);
	LineTo(hdcMem, ne, me);

	//������ ��� OY
	nb = xn(0); mb = ym(yBottom);
	MoveToEx(hdcMem, nb, mb, NULL);
	ne = xn(0); me = ym(yTop);
	LineTo(hdcMem, ne, me);

	for (int j = 0; j < NPoints-1; j += 3) {

		for (int i = 0; i < Npoints; i++)
		{
			V[i].x = Vt[j + i].x;
			V[i].y = Vt[j + i].y;
		}

		HPEN hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		SelectObject(hdcMem, hpen2);
		//������ ������� ����� �� ������� ������
		MoveToEx(hdcMem, xn(V[0].x), ym(V[0].y), 0);
		for (int i = 1; i < Npoints; i++)
			LineTo(hdcMem, xn(V[i].x), ym(V[i].y));

		HPEN hpen3 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		SelectObject(hdcMem, hpen3);
		if (j % 2 == 0) {
			hpen3 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
			SelectObject(hdc, hpen3);
		}
		//������ ������ �����
		int Nt = 10 * Norder;
		double t, dt = 1.0 / (Nt - 1);
		MoveToEx(hdcMem, xn(splineBezie(Norder, 0).x), ym(splineBezie(Norder, 0).y), 0);
		for (int p = 1; p < Nt; p++)
		{
			t = dt*p;
			LineTo(hdcMem, xn(splineBezie(Norder, t).x), ym(splineBezie(Norder, t).y));
		}


		HPEN hpen4 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		SelectObject(hdcMem, hpen4);
		HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hdcMem, hbrush);
		//������ ������ ����� � ���� �������
		for (int i = 0; i < Npoints; i++)
			Ellipse(hdcMem, xn(V[i].x) - 4, ym(V[i].y) - 4, xn(V[i].x) + 4, ym(V[i].y) + 4);
		SelectObject(hdcMem, hbrushOld);
		DeleteObject(hbrush0);
		DeleteObject(hbrush);

		SelectObject(hdcMem, hpenOld);
		DeleteObject(hpen0);
		DeleteObject(hpen1);
		DeleteObject(hpen2);
		DeleteObject(hpen3);
		DeleteObject(hpen4);
		
	}
	//�������� �������� ������ � �������� ������
	BitBlt(hdc, 0, 0, nRight, mBottom, hdcMem, 0, 0, SRCCOPY);

	DeleteDC(hdcMem);

	ReleaseDC(hwnd, hdc);

}

