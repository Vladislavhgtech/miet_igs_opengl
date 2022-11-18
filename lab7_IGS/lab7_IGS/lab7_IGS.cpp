

#include "stdafx.h"
#include <windows.h>     
#include "Resource.h"     

#include <math.h>
#include <stdio.h>
#include <tchar.h>


//идентификатор приложения
HINSTANCE hInstApp;


//порядок кривой Безье
int    Norder;
//число опорных точек
int    Npoints;
bool cube = false;

int NOrder; // колличество линий безье
int NPoints;

BOOL CALLBACK DialProc(HWND, UINT, WPARAM, LPARAM);

//прототип функции окна
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



int WINAPI WinMain(HINSTANCE hInstance,       //идентификатор приложения
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdParam,
	int       nCmdShow)
{

	WCHAR cname[] = L"Spline";     // Имя класса окна 
	WCHAR title[] = L"  Interactive Control   to    Bezie Spline Curves";  // Заголовок окна

	MSG msg;    // структура для работы с сообщениями

	//регистрация класса окна приложения
	//------------------------------------------------------------------------
	WNDCLASS wc;  // структура для регистрации класса окна приложения

	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;         //адрес функции окна
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;       //идентификатор приложения
	wc.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszMenuName = (LPCTSTR)MENU1;
	wc.lpszClassName = cname;

	if (!RegisterClass(&wc))  //регистрация класса окна приложения
		return 0;

	hInstApp = hInstance;

	//создание окна приложения
	//------------------------------------------------------------------------
	HWND hWnd;       // дескриптор окна приложения

	hWnd = CreateWindow(cname,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,         //идентификатор окна-родителя     
		NULL,          //идентификатор меню дочернего окна
		hInstance,     //идентификатор приложения
		NULL);

	if (!hWnd)
		return 0;



	// Рисуем окно. Для этого после функции ShowWindow,
	// рисующей  окно, вызываем функцию UpdateWindows,
	// посылающую сообщение WM_PAINT в функцию окна
	//--------------------------------------------------------------------------
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// Запускаем цикл обработки сообщений
	//-------------------------------------------------------------------------

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;


}
//функция диалогового окна
BOOL CALLBACK DialProc(HWND hwndDlg, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	char S[10];
	//TCHAR S[10];
	switch (uMsg)
	{
		//сообщение при открытии диалогового окна
	case WM_INITDIALOG:
		SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), (LPCWSTR)"1");
		sprintf_s(S, "%d", 1);
		//swprintf_s(S, 20, _T("%d"), 1);
		SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), (LPCWSTR)S);
		break;

		//сообщение при закрытии диалогового окна
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		break;

		//сообщение, приходящее от элементов диалогового окна
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)  //нажата кнопка
		{
			EndDialog(hwndDlg, 0);
		}
		if (LOWORD(wParam) == IDOK)  //нажата кнопка
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

//функция окна, обрабатывающая сообщения
LRESULT CALLBACK  WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	//координаты мышки
	int x, y;
	//значение возврщаемое после закрыимя диалогвого окна
	//	int EndDial;
	switch (message)
	{
		//сообщение при открытии окна
	case WM_CREATE:
		SplineCreate(hWnd);  //выход из цикла сообщений
		break;

		//сообщение при нажатии левой клавиши мышки
	case WM_LBUTTONDOWN:
		SplineLButtonDown();
		break;

		//сообщение при отпускании левой клавиши мышки
	case WM_LBUTTONUP:
		SplineLButtonUp();
		break;

		//сообщение при движении мышки
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

		//сообщение при закрытии окна
	case WM_DESTROY:
		PostQuitMessage(0);  //выход из цикла сообщений
		break;

		//сообщения от кнопок меню
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EXIT:
			PostQuitMessage(0);  //выход из цикла сообщений
			break;

		case ID_CLEAN:
			InvalidateRect(hWnd, NULL, TRUE);  //посылает сообщение WM_PAINT
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

//массив опорных точек
POINT2 V[4];
POINT2 Vt[20];

//размеры окна вывода в мировой системе координат
double xLeft, xRight, yBottom, yTop;
//размеры окна вывода в пикселях на экране монитора
int    nLeft, nRight, mBottom, mTop;

//флаги
int flagDown, flagUp, flagLock;
//номер зацепленной опорной точки
int Nlock;


//переход от x к пикселю n
inline int xn(double x)
{
	return (int)((x - xLeft) / (xRight - xLeft)*(nRight - nLeft)) + nLeft;
}

//переход от y к пикселю m
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


//координаты точки сплайновой кривой
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

//обрабатывает сообщение WM_LBUTTONDOWN
void SplineLButtonDown()
{
	flagUp = 0;
	flagDown = 1;
}

//обрабатывает сообщение WM_LBUTTONUP
void SplineLButtonUp()
{
	flagUp = 1;
	flagDown = 0;
	flagLock = 0;
}



//обрабатывает сообщение WM_CREATE
void SplineCreate(HWND hWnd)
{


	flagDown = 0; flagUp = 0; flagLock = 0;
}


//переход от пикселя n к x 
inline double nx(int n)
{
	return (n - nLeft)*(xRight - xLeft) / (nRight - nLeft) + xLeft;
}

//переход от пикселя m к y 
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





//функция обрабатывающая сообщение WM_COMMON
void SplineDC(HWND hwnd)
{


	RECT rc;
	GetClientRect(hwnd, &rc); //находит размеры клиентской области окна <hwnd>

	//размеры окна в мировых координатах и в пикселях
	xLeft = -10; xRight = 10; yBottom = -6; yTop = 6;
	nLeft = rc.left; nRight = rc.right; mBottom = rc.bottom; mTop = rc.top;

	Npoints = Norder + 1;
	//задаем опорные точки с помощью генератора случайных чисел
	double kr = 0.75;
	for (int i = 0; i<Npoints; i++)
	{
		V[i].x = kr*(xLeft + rand()*(xRight - xLeft) / RAND_MAX);
		V[i].y = kr*(yBottom + rand()*(yTop - yBottom) / RAND_MAX);
	}




	HDC hdc;            //идентификтор контекста устройства
	hdc = GetDC(hwnd);  //получаем контекст устройства <hdc> для окна <hwnd>


	HPEN hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	HPEN hpenOld = (HPEN)SelectObject(hdc, hpen1);

	int nb, ne, mb, me;

	//рисуем ось OX
	nb = xn(xLeft); mb = ym(0);
	MoveToEx(hdc, nb, mb, NULL);
	ne = xn(xRight); me = ym(0);
	LineTo(hdc, ne, me);

	//рисуем ось OY
	nb = xn(0); mb = ym(yBottom);
	MoveToEx(hdc, nb, mb, NULL);
	ne = xn(0); me = ym(yTop);
	LineTo(hdc, ne, me);



	HPEN hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, hpen2);
	//рисуем ломаную линию по опорным точкам

	MoveToEx(hdc, xn(V[0].x), ym(V[0].y), 0);
	for (int i = 1; i < Npoints; i++)
		LineTo(hdc, xn(V[i].x), ym(V[i].y));


	HPEN hpen3 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	SelectObject(hdc, hpen3);
	//рисуем кривую Безье
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
	//рисуем опрные точки в виде кружков
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
	HDC hdc;            //идентификтор контекста устройства
	hdc = GetDC(hwnd);  //получаем контекст устройства <hdc> для окна <hwnd>


	
	RECT rc;
	GetClientRect(hwnd, &rc); //находит размеры клиентской области окна <hwnd>

	//размеры окна в мировых координатах и в пикселях
	xLeft = -10; xRight = 10; yBottom = -6; yTop = 6;
	nLeft = rc.left; nRight = rc.right; mBottom = rc.bottom; mTop = rc.top;

	NPoints = (NOrder - 1) * 3 + 4;


	//задаем опорные точки с помощью генератора случайных чисел
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

		//рисуем ось OX
		nb = xn(xLeft); mb = ym(0);
		MoveToEx(hdc, nb, mb, NULL);
		ne = xn(xRight); me = ym(0);
		LineTo(hdc, ne, me);

		//рисуем ось OY
		nb = xn(0); mb = ym(yBottom);
		MoveToEx(hdc, nb, mb, NULL);
		ne = xn(0); me = ym(yTop);
		LineTo(hdc, ne, me);



		HPEN hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		SelectObject(hdc, hpen2);

		//рисуем ломаную линию по опорным точкам
		MoveToEx(hdc, xn(V[0].x), ym(V[0].y), 0);
		for (int i = 1; i < Npoints; i++)
			LineTo(hdc, xn(V[i].x), ym(V[i].y));

		
		SelectObject(hdc, green);
		if (j % 2 == 0) {
			SelectObject(hdc, blue);
		}
			

		//рисуем кривую Безье
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
		//рисуем опрные точки в виде кружков
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

	HDC hdc = GetDC(hwnd);  //получаем контекст устройства <hdc> для окна <hwnd>

	//создаем контекст памяти hdcMem, совместимый с контекстом экрана hdc
	HDC hdcMem = CreateCompatibleDC(hdc);
	//создаем (пустую) битовую карту совместмую с контекстом экрана
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, nRight, mBottom);
	//выбираем битовую карту hBmp в контекст памяти hdcMem
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hdcMem, hBmp);

	HPEN hpen0 = CreatePen(PS_SOLID, 2, RGB(0xC0, 0xC0, 0xC0));
	HPEN hpenOld = (HPEN)SelectObject(hdcMem, hpen0);
	HBRUSH hbrush0 = CreateSolidBrush(RGB(0xC0, 0xC0, 0xC0));
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdcMem, hbrush0);

	Rectangle(hdcMem, 0, 0, nRight, mBottom);


	HPEN hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	SelectObject(hdcMem, hpen1);

	int nb, mb, ne, me;
	//рисуем ось OX
	nb = xn(xLeft); mb = ym(0);
	MoveToEx(hdcMem, nb, mb, NULL);
	ne = xn(xRight); me = ym(0);
	LineTo(hdcMem, ne, me);

	//рисуем ось OY
	nb = xn(0); mb = ym(yBottom);
	MoveToEx(hdcMem, nb, mb, NULL);
	ne = xn(0); me = ym(yTop);
	LineTo(hdcMem, ne, me);

	HPEN hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdcMem, hpen2);
	//рисуем ломаную линию по опорным точкам
	MoveToEx(hdcMem, xn(V[0].x), ym(V[0].y), 0);
	for (int i = 1; i < Npoints; i++)
		LineTo(hdcMem, xn(V[i].x), ym(V[i].y));

	HPEN hpen3 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	SelectObject(hdcMem, hpen3);

	//рисуем кривую Безье
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
	//рисуем опрные точки в виде кружков
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
	//копируем контекст памяти в контекст экрана
	BitBlt(hdc, 0, 0, nRight, mBottom, hdcMem, 0, 0, SRCCOPY);

	DeleteDC(hdcMem);

	ReleaseDC(hwnd, hdc);

}

void LineDC1(HWND hwnd)
{
	
	HDC hdc = GetDC(hwnd);  //получаем контекст устройства <hdc> для окна <hwnd>

	//создаем контекст памяти hdcMem, совместимый с контекстом экрана hdc
	HDC hdcMem = CreateCompatibleDC(hdc);
	//создаем (пустую) битовую карту совместмую с контекстом экрана
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, nRight, mBottom);
	//выбираем битовую карту hBmp в контекст памяти hdcMem
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hdcMem, hBmp);

	HPEN hpen0 = CreatePen(PS_SOLID, 2, RGB(0xC0, 0xC0, 0xC0));
	HPEN hpenOld = (HPEN)SelectObject(hdcMem, hpen0);
	HBRUSH hbrush0 = CreateSolidBrush(RGB(0xC0, 0xC0, 0xC0));
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdcMem, hbrush0);

	Rectangle(hdcMem, 0, 0, nRight, mBottom);


	HPEN hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	SelectObject(hdcMem, hpen1);

	int nb, mb, ne, me;
	//рисуем ось OX
	nb = xn(xLeft); mb = ym(0);
	MoveToEx(hdcMem, nb, mb, NULL);
	ne = xn(xRight); me = ym(0);
	LineTo(hdcMem, ne, me);

	//рисуем ось OY
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
		//рисуем ломаную линию по опорным точкам
		MoveToEx(hdcMem, xn(V[0].x), ym(V[0].y), 0);
		for (int i = 1; i < Npoints; i++)
			LineTo(hdcMem, xn(V[i].x), ym(V[i].y));

		HPEN hpen3 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		SelectObject(hdcMem, hpen3);
		if (j % 2 == 0) {
			hpen3 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
			SelectObject(hdc, hpen3);
		}
		//рисуем кривую Безье
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
		//рисуем опрные точки в виде кружков
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
	//копируем контекст памяти в контекст экрана
	BitBlt(hdc, 0, 0, nRight, mBottom, hdcMem, 0, 0, SRCCOPY);

	DeleteDC(hdcMem);

	ReleaseDC(hwnd, hdc);

}

