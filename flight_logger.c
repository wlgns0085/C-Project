#include "flight_logger.h"
// 디스플레이 다듬기


int main()
{
	system("Flight Logger");
	system("mode con cols=108 lines=35");
	CursorHide(1, FALSE);

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
			plan_check();
			break;
		case '3':
			log_save();
			break;
		case '4':
			log_check();
			break;
		case '0':
			return 0;
			break;
		}
	}
	return;
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

		int drone_x = (map_x1 + map_x2) / 2;
		int drone_y = (map_y1 + map_y2) / 2;
		int X_MIN = map_x1;
		int X_MAX = map_x2;
		int Y_MIN = map_y1;
		int Y_MAX = map_y2;
		int point_speed = 5;
		BOOL tmp_point = FALSE;

		while (1)
		{
			if (KEY_DOWN(VK_LEFT)) { if (drone_x > X_MIN + point_speed) drone_x -= point_speed; }
			else if (KEY_DOWN(VK_RIGHT)) { if (drone_x < X_MAX - point_speed) drone_x += point_speed; }
			else if (KEY_DOWN(VK_UP)) { if (drone_y > Y_MIN + point_speed) drone_y -= point_speed; }
			else if (KEY_DOWN(VK_DOWN)) { if (drone_y < Y_MAX - point_speed) drone_y += point_speed; }
			else if (KEY_DOWN(0x4D)) point_speed++;
			else if (KEY_DOWN(0x4E)) point_speed--;
			else if (KEY_DOWN(0x51)) break;
			else if (KEY_DOWN(VK_SPACE))
			{
				point.latitude = (drone_x - (double)map_x1) / 1000 / 500 + lat_value;
				point.longitude = (drone_y - (double)map_y2) / 1000 / 500 + lon_value;
				fprintf(fp, "%lf %lf\n", point.latitude, point.longitude);
				(waypoint + count)->latitude = point.latitude;
				(waypoint + count)->longitude = point.longitude;
				count++;
				waypoint = (struct _point*)realloc(waypoint, sizeof(struct _point) * (count + 1));
			}
			GotoXY(0, 11);
			printf("  x: %lf\n  y: %lf\n", (drone_x - (double)map_x1) / 1000 / 500 + lat_value, (drone_y - (double)map_y2) / 1000 / 500 + lon_value);
			printf("  point speed: %d\n", point_speed);
			printf("  count: %d", count);
			print_map();
			DrawPoint(hdc, drone_x, drone_y, 5);
			print_route(count, waypoint);

			Sleep(500);
		}
		fclose(fp);
		free(waypoint);
	}
	else printf("Create fail of [ %s ]\n", log_file_name);

	return;
}
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
		printf("    ↑    [q] to save and exit\n");
		printf("  ←  →  [spacebar] to stop\n");
		printf("    ↓  \n");
		int drone_x = (map_x1+ map_x2)/2;
		int drone_y = (map_y1+ map_y2)/2;
		int drone_x_speed = 0;
		int drone_y_speed = 0;
		int X_MIN = map_x1;
		int X_MAX = map_x2;
		int Y_MIN = map_y1;
		int Y_MAX = map_y2;


		while (1)
		{
			// 사용자의 키보드 입력 -> 드론 속도 제어
			if (KEY_DOWN(VK_LEFT)) drone_x_speed -= 1;
			else if (KEY_DOWN(VK_RIGHT)) drone_x_speed += 1;
			else if (KEY_DOWN(VK_UP)) drone_y_speed -= 1;
			else if (KEY_DOWN(VK_DOWN)) drone_y_speed += 1;
			else if (KEY_DOWN(VK_SPACE)) { drone_x_speed = 0; drone_y_speed = 0; }
			else if (KEY_DOWN(0x51)) break;

			// 속도 반영하여 드론 이동
			if (drone_x_speed <= 0 && drone_x > X_MIN + drone_x_speed) drone_x += drone_x_speed;
			else if (drone_x_speed > 0 && drone_x < X_MAX + drone_x_speed) drone_x += drone_x_speed;
			if (drone_y_speed <= 0 && drone_y > Y_MIN + drone_y_speed) drone_y += drone_y_speed;
			else if (drone_y_speed > 0 && drone_y < Y_MAX + drone_y_speed) drone_y += drone_y_speed;

			// GPS 좌표로 변환
			point.latitude = (drone_x - (double)map_x1) / 1000 / 500 + lat_value;
			point.longitude = (drone_y - (double)map_y2) / 1000 / 500 + lon_value;

			// 상태 표시
			GotoXY(0, 11);
			printf("  x speed: %3d\n  y speed: %3d\n", drone_x_speed, -1 * drone_y_speed);
			printf("  x: %lf\n  y: %lf\n", point.latitude, point.longitude);
			printf(" count: %d", ++count);
			print_map();
			DrawPoint(hdc, drone_x, drone_y, 5);
			fprintf(fp, "%lf %lf\n", point.latitude, point.longitude);

			Sleep(300);
		}
		fclose(fp);
		free(waypoint);
	}
	else printf("Create fail of [ %s ]\n", log_file_name);
	printf("Logging Exit\n");
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
			waypoint = (struct _point*)realloc(waypoint, sizeof(struct _point) * (count + 1));
		}
		fclose(fp);

		print_map();

		print_route(count, waypoint);

		getch();
		free(waypoint);
	}
	else printf("load fail of [ %s ]\n", log_file_name);

	return;
}
void plan_check()
{
	FILE* fp;
	errno_t err;
	char log_file_name[] = "plan.txt";
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
			waypoint = (struct _point*)realloc(waypoint, sizeof(struct _point) * (count + 1));
		}
		fclose(fp);

		print_map();

		print_route(count, waypoint);

		getch();
		free(waypoint);
	}
	else printf("load fail of [ %s ]\n", log_file_name);

	return;
}

