/* COMP20005 - Assignment 1
   Alasdair Norton - 587162
  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GRAPH_SCALE 	10
#define GRAPH_MINIMUM 	10
#define ENERGY_BASE 	10
#define ENERGY_MULT 	1.5
#define ENERGY_ADD  	7.2
#define DATE_TIME_ARRAY 6
#define POS_MAG_ARRAY 	3
#define PREFIXES_ARRAY 	3
#define HEADER_LINES 	2
#define MONTHS_IN_YEAR	12                                                   

void remove_headers(int);
void print_stage_1(int, int, int, int, int, int, double, double, double);
void print_stage_2(int[], double[], int, double);
void print_stage_3_4(int, int, int, double, double);
void print_energy(double);
void update_maximums(int[], double[], int, int, int, int, int, int, double, 
	double, double);
void check_skipped_months(int, int, int, int);
double calculate_energy(double);


int
main(int argc, char *argv[]) {
	/* Input data variables */
	int yyyy, mm, dd, hour, min, sec, junki;
	double latit, longit, magnitude, depth;
	char junkc;
	/* Part 2 record keeping variables */
	int total_earthquakes=0, max_date_time[DATE_TIME_ARRAY];
	double max_pos_magnitude[POS_MAG_ARRAY], total_energy=0;
	/* Part 3/4 record keeping variables */
	int current_month, current_year, monthly_total=0, last_month, last_year;
	double month_magnitude, month_energy;

	remove_headers(HEADER_LINES);
	/* Read first line, initialise running variables for stages 2 and 3,
	   print stage 1 */
	scanf("%4d-%2d-%2d%c%2d:%2d:%2d+%2d:%2d,%lf,%lf,%lf,%lf", &yyyy, &mm,
	&dd, &junkc, &hour, &min, &sec, &junki, &junki, &latit, &longit, &depth,
	&magnitude);
	print_stage_1(yyyy, mm, dd, hour, min, sec, latit, longit, magnitude);
	/* Stage 2 Priming */
	total_earthquakes++;
	total_energy+=calculate_energy(magnitude);
	update_maximums(max_date_time, max_pos_magnitude, yyyy, mm, dd, hour,
		min, sec, latit, longit, magnitude);
	/* Stage 3/4 priming*/
	current_month=mm;
	current_year =yyyy;
	monthly_total++;
	month_energy=calculate_energy(magnitude);
	month_magnitude=magnitude;
	printf("Stage 3/4\n=========\n");
	
	while(scanf("%4d-%2d-%2d%c%2d:%2d:%2d+%2d:%2d,%lf,%lf,%lf,%lf",
	&yyyy, &mm, &dd, &junkc, &hour, &min, &sec, &junki,
	&junki, &latit, &longit, &depth, &magnitude)==13){
		/* Reads each line of the file in turn 
		   Stage 2 iteration */
		total_earthquakes++;
		total_energy+=calculate_energy(magnitude);
		if(magnitude>max_pos_magnitude[2]){
			update_maximums(max_date_time, max_pos_magnitude, yyyy, 
			mm, dd, hour, min, sec, latit, longit, magnitude);
		}
		/* Stage 3/4 iteration */
		if(current_month==mm){
			monthly_total++;
			month_energy+=calculate_energy(magnitude);
			if(magnitude>month_magnitude){
				month_magnitude=magnitude;
			}
		}else{
			/* Print data for the previous month, update/reset
			   running variables and check that no months have been
			   skipped */
			print_stage_3_4(current_month, current_year, 
				monthly_total, month_magnitude, month_energy);
			last_month=current_month;
			last_year=current_year;
			current_year=yyyy;
			current_month=mm;
			check_skipped_months(current_month, current_year,
				last_month, last_year);
			monthly_total=1;
			month_energy=calculate_energy(magnitude);
			month_magnitude=magnitude;
		}	
	}
	print_stage_3_4(current_month, current_year, monthly_total, 
		month_magnitude, month_energy);
	printf("\n");
	print_stage_2(max_date_time, max_pos_magnitude, total_earthquakes, 
		total_energy);
	return 0;
}

void 
remove_headers(int num_lines){
	/* Reads and discards the the specified number of lines from the top of 
	   the input file*/
	int c, lines_read=0;
	while(lines_read<num_lines && (c=getchar())!=EOF ){
		/* Iterates through characters in the input. If the character is
		   '\n', increases the count of lines read. Once the specified 
		   number of lines are read, exits the loop*/
		if(c=='\n'){
			lines_read++;
		}
	}
}

void 
print_stage_1(int yyyy, int mm, int dd, int hour, int min, int sec,
	double latit, double longit, double magnitude){
	double energy;
	printf("Stage 1\n=======\n");
	printf("Records commence: %d-%02d-%02d, %02d:%02d:%02d\n", 
		yyyy, mm, dd, hour, min, sec);
	printf("First earthquake: magnitude %.1f at (%.1f,%.1f)\n", magnitude,
		latit, longit);
	printf("Energy released : ");
	energy=calculate_energy(magnitude);
	print_energy(energy);
	printf("\n\n");
}

void
print_stage_2(int date_time[], double pos_magnitude[], int total_earthquakes, 
	double total_energy){
	/* In date_time array, 0 - yyyy, 1- mm, 2- dd, 3- hour, 4- min, 5-sec
	   in pos_magnitude array, 0- latitude, 1- longitude, 3- magnitude */
	printf("Stage 2\n=======\n");
	printf("Total earthquakes    : %d\n", total_earthquakes);
	printf("Strongest earthquake : magnitude %.1f at (%.1f,%.1f)\n",
		pos_magnitude[2], pos_magnitude[0], pos_magnitude[1]);
	printf("Took place on        : %04d-%02d-%02d, %02d:%02d:%02d\n",
		date_time[0], date_time[1], date_time[2], date_time[3], 
		date_time[4], date_time[5]);
	printf("Total energy released: ");
	print_energy(total_energy);
	printf("\n");
}

void
print_stage_3_4(int month, int year, int total, double max_magnitude, 
	double energy){
	int num_symbols, i;
	double scaled_energy;
	printf("%4d-%02d: %3d, max %.1f ", year, month, total, max_magnitude);
	/* Take the monthly energy, given in TJ, and work out how many symbols
	   are required. Using a log scale, with minimum value n TJ and scaled 
	   for m points per order of magnitude, the formula used is 
	   m*log_10(energy/n)*/
	scaled_energy=GRAPH_SCALE*log10(energy/GRAPH_MINIMUM);
	/* Round the value off to the nearest integer */
	if(scaled_energy-floor(scaled_energy)<0.5){
		num_symbols=floor(scaled_energy);
	}else{
		num_symbols=ceil(scaled_energy);
	}
	printf("|");
	for(i=1;i<=num_symbols;i++){
		if(i==num_symbols){
			printf("|");
		}else if(i%10==0){
			printf("+");
		}else{
			printf("-");
		}
	}
	printf("\n");
}
double
calculate_energy(double magnitude){
	double energy;
	energy = pow(ENERGY_BASE, ENERGY_MULT*magnitude-ENERGY_ADD);
	return energy;
}
	
void
print_energy(double energy){
	int order=0;
	char prefixes[PREFIXES_ARRAY]={'T','P','E'};
	while(energy>=1000){
	/* Determines how many orders of magnitude above TJ the energy is.
	This then determines the index from the prefixes array to be printed. */
		order++;
		energy=energy/1000;
	}
	if(energy<10){
		printf("%.1f %cJ", energy, prefixes[order]);
	}
	if(energy>=10){
		printf("%.0f %cJ", energy, prefixes[order]);
	}
}

void
update_maximums(int time_date[], double pos_magnitude[], int yyyy, int mm, 
	int dd, int hour, int min, int sec, double latit, 
	double longit, double magnit){
	time_date[0] = yyyy;
	time_date[1] = mm;
	time_date[2] = dd;
	time_date[3] = hour;
	time_date[4] = min;
	time_date[5] = sec;
	pos_magnitude[0] = latit;
	pos_magnitude[1] = longit;
	pos_magnitude[2] = magnit;
}

void
check_skipped_months(int current_month, int current_year, int last_month, 
	int last_year){
	/* Checks that there have been no months for which no data was provided.
	   For each skipped month, prints a generic line, formatted as
	   yyyy-mm:   0, max --- |
	   to represent the output of stage 3 and 4 if given no data.
	
	Case where some months in the same year have been skipped */
	if(current_year==last_year&&current_month!=last_month+1){
		last_month+=1;
		while(last_month<current_month){
			printf("%d-%02d:   0, max --- |\n", 
				last_year, last_month);
			last_month++;
		}
	}
	/* Case where the year changes */
	if(current_year!=last_year&&(current_month!=1
		||last_month!=MONTHS_IN_YEAR)){
		while(last_year<current_year){
			while(last_month<MONTHS_IN_YEAR){
				last_month++;
				printf("%d-%02d:   0, max --- |\n", 
					last_year, last_month);
			}
			last_year++;
			last_month=1;
		}
		while(last_month<current_month){
			printf("%d-%02d:   0, max --- |\n", 
				last_year, last_month);
			last_month++;
		}
	}
}
	
