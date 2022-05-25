#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

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
	
}
void print_main()
{
	printf("1. Make Flight Plan\n");
	printf("2. Log Save\n");
	printf("3. Log Check\n");
}
void print_map()
{

}
void make_plan()
{

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
		int count = 0;
		printf("* put 500 to exit *\n");
		while (1)
		{
			printf("point[ %d ] : ", count);
			scanf_s("%lf %lf", &point.latitude, &point.longitude);
			if (point.latitude == 500 || point.longitude == 500)
			{
				printf("logging exit\n");
				break;
			}
			fprintf(fp, "%lf %lf\n", point.latitude, point.longitude);
			
			count++;
		}
		fclose(fp);
	}
	else printf("Create fail of [ %s ]\n", log_file_name);

	return;
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
		int count=0;
		while (EOF != fscanf_s(fp, "%lf %lf", &point.latitude, &point.longitude))
		{
			printf("point[ %d ]: [ %12.8lf ], [ %12.8lf ]\n",count, point.latitude, point.longitude);
			count++;
		}
		fclose(fp);
	}
	else printf("load fail of [ %s ]\n", log_file_name);
	
	return ;
}