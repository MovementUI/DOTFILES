/* made by profil 2011-12-29.
**
** Compile with:
** gcc -Wall -pedantic -std=c99 -lX11 status.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>

static Display *dpy;

void setstatus(char *str) {
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

float getfreq(char *file) {
	FILE *fd;
	char *freq;
	float ret;

	freq = malloc(10);
	fd = fopen(file, "r");
	if(fd == NULL) {
		fprintf(stderr, "Cannot open '%s' for reading.\n", file);
		exit(1);
	}

	fgets(freq, 10, fd);
	fclose(fd);

	ret = atof(freq)/1000000;
	free(freq);
	return ret;
}

char *getdatetime() {
	char *buf;
	time_t result;
	struct tm *resulttm;

	if((buf = malloc(sizeof(char)*65)) == NULL) {
		fprintf(stderr, "Cannot allocate memory for buf.\n");
		exit(1);
	}
	result = time(NULL);
	resulttm = localtime(&result);
	if(resulttm == NULL) {
		fprintf(stderr, "Error getting localtime.\n");
		exit(1);
	}
	if(!strftime(buf, sizeof(char)*65-1, "%a %b %d %H:%M", resulttm)) {
		fprintf(stderr, "strftime is 0.\n");
		exit(1);
	}

	return buf;
}
float gettemperature()
{
	char fo[10];
	FILE *ftpr;
	//"/sys/class/hwmon/hwmon0/device", "temp1_input"  
	ftpr = fopen("/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp3_input", "r");
	fgets(fo, 5, ftpr); 
	float co = atof(fo)/100;
	fclose(ftpr);
	return co;
}


int main(void) {
	char *status;
	float cpu0;
	char *datetime;
	float coretemp;

	if (!(dpy = XOpenDisplay(NULL)))
	{
		fprintf(stderr, "Cannot open display.\n");
		return 1;
	}

	if((status = malloc(200)) == NULL)
		exit(1);


	for (;;sleep(5))
	{
		cpu0 = getfreq("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
		datetime = getdatetime();
		coretemp= gettemperature();
		
		snprintf(status, 200, "%0.0F°C - %0.2FMHz |  %s", coretemp, cpu0, datetime);
		

		free(datetime);
		setstatus(status);

	}

	free(status);
	XCloseDisplay(dpy);

	return 0;
}


