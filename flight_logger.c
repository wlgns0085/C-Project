#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include <conio.h>
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code)&0x8001)?1:0)

double lat_value = 36.144;
double lon_value = 128.393;

HWND hwnd;
HDC hdc;

void GotoXY(int x, int y)
{
	COORD CursorPosition = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}
void CursorHide(int size, BOOL flag)
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = size;
	cursorInfo.bVisible = flag;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}
void DrawPoint(HDC hdc, int x, int y, int rad)
{
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd);
	Ellipse(hdc, x-rad, y-rad, x+rad, y+rad);
	return 0;
}

struct _point
{
	double latitude;
	double longitude;
}point;
void print_main();
void print_map();
void make_plan();
void log_save();
void log_check();
int main()
{

	while (1)
	{
		print_main();
		char key = getch();
		
		switch (key)
		{
		case '1':
			make_plan();
			break;
		case '2':
			log_save();
			break;
		case '3':
			log_check();
			break;
		case 'q':
		case 'Q':
			return 0;
			break;
		}
	}
	return;
}
void print_main()
{
	system("cls");
	printf("1. Make Flight Plan\n");
	printf("2. Log Save\n");
	printf("3. Log Check\n");
	printf("Q. Exit\n");
}
void print_map()
{
	HBRUSH Brush, oldBrush;
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd);

	/*
	Brush = CreateSolidBrush(RGB(255, 255, 255));
	oldBrush = (HBRUSH)SelectObject(hdc, Brush);
	Rectangle(hdc, 700 - 30, 100 - 30, 1200 + 30, 600 + 30);
	DeleteObject(Brush);
	DeleteObject(oldBrush);
	*/
	//GotoXY(55, 24);
	//GotoXY(55, 24);
	//printf("%lf\t\t\t\t%lf", lat_value, lat_value+0.001);


	Brush = CreateSolidBrush(RGB(0, 128, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, Brush);
	Rectangle(hdc, 700, 100, 1200, 600);
	DeleteObject(Brush);
	DeleteObject(oldBrush);

	for (int i = 700; i <= 1200; i += 50) DrawLine(hdc, i, 100, i, 600);
	for (int i = 100; i <= 600; i += 50) DrawLine(hdc, 700, i, 1200, i);
	
	
	//GotoXY(0, 6);
	getch();
}
void make_plan()
{
	FILE* fp;
	errno_t err;
	char log_file_name[] = "plan.txt";
	err = fopen_s(&fp, log_file_name, "wt");
	if (fp != NULL)
	{
		printf("Create success of [ %s ]\n", log_file_name);

		struct _point* waypoint;
		int count = 0;
		waypoint = (struct _point*)malloc(sizeof(struct _point));

		printf("* latitude  : %lf ~ %lf *\n", lat_value, lat_value + 0.001);
		printf("* longitude : %lf ~ %lf *\n", lon_value, lon_value + 0.001);
		while (1)
		{
			printf("point[ %d ] : ", count);
			scanf_s("%lf %lf", &point.latitude, &point.longitude);
			if (point.latitude < lat_value || point.latitude > lat_value + 0.001 || point.longitude < lon_value || point.longitude > lon_value + 0.001)
			{
				printf("logging exit\n");
				break;
			}
			fprintf(fp, "%lf %lf\n", point.latitude, point.longitude);
			(waypoint + count)->latitude = point.latitude;
			(waypoint + count)->longitude = point.longitude;
			count++;
			waypoint = (struct _point*)realloc(waypoint, sizeof(struct _point) * (count + 1));
		}
		fclose(fp);
		free(waypoint);
	}
	else printf("Create fail of [ %s ]\n", log_file_name);

	return;
}
/*
void log_save()
{
	FILE* fp;
	errno_t err;
	char log_file_name[] = "log.txt";
	err = fopen_s(&fp, log_file_name, "wt");
	if (fp != NULL)
	{
		printf("Create success of [ %s ]\n", log_file_name);

		struct _point* waypoint;
		int count = 0;
		waypoint = (struct _point*)malloc(sizeof(struct _point));
		
		printf("* latitude  : %lf ~ %lf *\n", lat_value, lat_value + 0.001);
		printf("* longitude : %lf ~ %lf *\n", lon_value, lon_value + 0.001);
		while (1)
		{
			printf("point[ %d ] : ", count);
			scanf_s("%lf %lf", &point.latitude, &point.longitude);
			if (point.latitude < lat_value || point.latitude > lat_value + 0.001 || point.longitude < lon_value || point.longitude > lon_value + 0.001)
			{
				printf("logging exit\n");
				break;
			}
			fprintf(fp, "%lf %lf\n", point.latitude, point.longitude);
			(waypoint + count)->latitude = point.latitude;
			(waypoint + count)->longitude = point.longitude;
			count++;
			waypoint = (struct _point*)realloc(waypoint, sizeof(struct _point) * (count + 1));
		}
		fclose(fp);
		free(waypoint);
	}
	else printf("Create fail of [ %s ]\n", log_file_name);

	return;
}
*/
void log_save()
{
	FILE* fp;
	errno_t err;
	char log_file_name[] = "log.txt";
	err = fopen_s(&fp, log_file_name, "wt");
	if (fp != NULL)
	{
		printf("Create success of [ %s ]\n", log_file_name);

		struct _point* waypoint;
		int count = 0;
		waypoint = (struct _point*)malloc(sizeof(struct _point));

		printf("\n");
		printf("    ก่    [q] to save and exit\n");
		printf("  ก็  กๆ  [spacebar] to stop\n");
		printf("    ก้  \n");
		int drone_x = 950;
		int drone_y = 350;
		int drone_x_speed = 0;
		int drone_y_speed = 0;
		int X_MIN = 700;
		int X_MAX = 1200;
		int Y_MIN = 100;
		int Y_MAX = 600;

		while (1)
		{
			if (KEY_DOWN(VK_LEFT)) drone_x_speed -= 1;
			else if (KEY_DOWN(VK_RIGHT)) drone_x_speed += 1;
			else if (KEY_DOWN(VK_UP)) drone_y_speed -= 1;
			else if (KEY_DOWN(VK_DOWN)) drone_y_speed += 1;
			else if (KEY_DOWN(VK_SPACE)) { drone_x_speed = 0; drone_y_speed = 0; }
			else if (KEY_DOWN(0x51)) break;

			if (drone_x_speed <= 0 && drone_x > X_MIN + drone_x_speed) drone_x += drone_x_speed;
			else if (drone_x_speed > 0 && drone_x < X_MAX + drone_x_speed) drone_x += drone_x_speed;
			if (drone_y_speed <= 0 && drone_y > Y_MIN + drone_y_speed) drone_y += drone_y_speed;
			else if (drone_y_speed > 0 && drone_y < Y_MAX + drone_y_speed) drone_y += drone_y_speed;

			point.latitude = (drone_x - 700.0) / 1000 / 500 + lat_value;
			point.longitude = (drone_y - 600.0) / 1000 / 500 + lon_value;

			GotoXY(0, 11);
			printf("  x speed: %3d\n  y speed: %3d\n", drone_x_speed, -1 * drone_y_speed);
			printf("  x: %lf\n  y: %lf\n", point.latitude, point.longitude);
			printf(" count: %d", ++count);
			print_map();
			DrawPoint(hdc, drone_x, drone_y, 5);
			fprintf(fp, "%lf %lf\n", point.latitude, point.longitude);

			Sleep(500);
		}
		fclose(fp);
		free(waypoint);
	}
	else printf("Create fail of [ %s ]\n", log_file_name);
	return 0;
}
void log_check()
{
	FILE* fp;
	errno_t err;
	char log_file_name[] = "log.txt";
	err = fopen_s(&fp, log_file_name, "rt");
	if (fp != NULL)
	{
		printf("load success of [ %s ]\n", log_file_name);

		struct _point* waypoint;
		int count = 0;
		waypoint = (struct _point*)malloc(sizeof(struct _point));
		
		while (EOF != fscanf_s(fp, "%lf %lf", &point.latitude, &point.longitude))
		{
			(waypoint + count)->latitude = point.latitude;
			(waypoint + count)->longitude = point.longitude;
			count++;
			waypoint = (struct _point*) realloc(waypoint, sizeof(struct _point) * (count+1));
		}
		fclose(fp);

		print_map();

		for (int i = 0; i < count; i++)
		{
			printf("point[ %d ]: [ %12.8lf ], [ %12.8lf ]\n", i, (waypoint+i)->latitude, (waypoint + i)->longitude);
			int lat_x = 700 + ((waypoint + i)->latitude - lat_value) * 1000 * 500;
			int	lon_y = 600 - ((waypoint + i)->longitude - lon_value) * 1000 * 500;
			DrawPoint(hdc, lat_x, lon_y, 5);
		}
		for (int i = 1; i < count; i++)
		{
			int lat_x1 = 700 + ((waypoint + i - 1)->latitude - lat_value) * 1000 * 500;
			int	lon_y1 = 600 - ((waypoint + i - 1)->longitude - lon_value) * 1000 * 500;
			int lat_x2 = 700 + ((waypoint + i)->latitude - lat_value) * 1000 * 500;
			int	lon_y2 = 600 - ((waypoint + i)->longitude - lon_value) * 1000 * 500;
			DrawLine(hdc, lat_x1, lon_y1, lat_x2, lon_y2);
		}

		getch();
		free(waypoint);
	}
	else printf("load fail of [ %s ]\n", log_file_name);

	return ;
}