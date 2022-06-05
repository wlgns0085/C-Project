#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code)&0x8001)?1:0)

double lat_value = 36.144;
double lon_value = 128.393;

int map_x1 = 700;
int map_x2 = 1200;
int map_y1 = 280;
int map_y2 = 780;

HWND hwnd;
HDC hdc;

struct _point
{
	double latitude;
	double longitude;
}point;
void GotoXY(int x, int y)
{
	COORD CursorPosition = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}
void CursorHide(int size, BOOL flag)
{
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = size; // 커서 크기
	CurInfo.bVisible = flag; // FALSE: 안보이게, TRUE: 보이게
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}
void DrawColorLine(HDC hdc, int x1, int y1, int x2, int y2, int solid, int thick, int r, int g, int b)
{
	HPEN Pen, oldPen;
	Pen = CreatePen(solid, thick, RGB(r, g, b));
	oldPen = (HPEN)SelectObject(hdc, Pen);
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	DeleteObject(Pen);
	DeleteObject(oldPen);
}
void FilledRect(HDC hdc, int x1, int y1, int x2, int y2, int r, int g, int b) // 사각형 칠
{
	HBRUSH Brush, oldBrush;

	Brush = CreateSolidBrush(RGB(r, g, b));
	oldBrush = (HBRUSH)SelectObject(hdc, Brush);

	Rectangle(hdc, x1, y1, x2, y2);

	DeleteObject(Brush);
	DeleteObject(oldBrush);
}
void DrawPoint(HDC hdc, int x, int y, int rad)
{
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd);
	Ellipse(hdc, x - rad, y - rad, x + rad, y + rad);
	return 0;
}

void print_map()
{
	// 그래프 수치 표시
	GotoXY(55, 7);
	printf("%.3lf\t\t\t\t     %.3lf", lon_value, lon_value+0.001);
	GotoXY(48, 9);
	printf("%.3lf", lat_value+0.001);
	GotoXY(48, 30);
	printf("%.3lf", lat_value + 0.001);
	Sleep(100);

	HBRUSH Brush, oldBrush;
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd);

	// 테두리 생성
	Brush = CreateSolidBrush(RGB(255, 255, 255));
	oldBrush = (HBRUSH)SelectObject(hdc, Brush);
	Rectangle(hdc, map_x1 - 30, map_y1 - 30, map_x2 + 30, map_y2 + 30);
	DeleteObject(Brush);
	DeleteObject(oldBrush);

	

	// 바탕 생성
	Brush = CreateSolidBrush(RGB(0, 128, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, Brush);
	Rectangle(hdc, map_x1, map_y1, map_x2, map_y2);
	DeleteObject(Brush);
	DeleteObject(oldBrush);

	// 그리드 생성
	for (int i = map_x1; i <= map_x2; i += 50) DrawLine(hdc, i, map_y1, i, map_y2);
	for (int i = map_y1; i <= map_y2; i += 50) DrawLine(hdc, map_x1, i, map_x2, i);

	Sleep(100);
}
void print_route(int count, struct _point* waypoint)
{
	for (int i = 0; i < count; i++) // 드론 위치 표시
	{
		//printf("point[ %d ]: [ %12.8lf ], [ %12.8lf ]\n", i, (waypoint+i)->latitude, (waypoint + i)->longitude);
		int lat_x = map_x1 + ((waypoint + i)->latitude - lat_value) * 1000 * 500;
		int	lon_y = map_y2 + ((waypoint + i)->longitude - lon_value) * 1000 * 500;
		//printf("%d %d\n", lat_x, lon_y);

		FilledRect(hdc, lat_x - 4, lon_y - 4, lat_x + 4, lon_y + 4, 255, 0, 0);
	}
	for (int i = 1; i < count; i++) // 경로 표시
	{
		int lat_x1 = map_x1 + ((waypoint + i - 1)->latitude - lat_value) * 1000 * 500;
		int	lon_y1 = map_y2 + ((waypoint + i - 1)->longitude - lon_value) * 1000 * 500;
		int lat_x2 = map_x1 + ((waypoint + i)->latitude - lat_value) * 1000 * 500;
		int	lon_y2 = map_y2 + ((waypoint + i)->longitude - lon_value) * 1000 * 500;
		DrawColorLine(hdc, lat_x1, lon_y1, lat_x2, lon_y2, 0, 2, 255, 0, 0);
	}
}
void print_main()
{
	system("cls");
	printf("1. Make Flight Plan\n");
	printf("2. Flight Plan Check\n");
	printf("3. Log Save\n");
	printf("4. Log Check\n");
	printf("0. Exit\n");
}
void make_plan();
void plan_check();
void log_save();
void log_check();
