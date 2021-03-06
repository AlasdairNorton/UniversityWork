#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define X_LENGTH 78
#define Y_LENGTH 78
#define MAX_LAMPS 100
#define MAX_LOCATIONS 100
#define PI 3.14159
#define POLE_HEIGHT 8.25

typedef struct {
	double x, y;
} coordinates_t;

typedef struct {
	double output;
	coordinates_t pos;
} lamp_t;

double calc_intensity(lamp_t[], int, coordinates_t);
double intensity_at_point(double, double, lamp_t[], int);
void print_map_cell(double illuminance);
void read_input(lamp_t[], int*, coordinates_t[], int*);
void stage_1(lamp_t[], int);
void stage_2(lamp_t[], int, coordinates_t[], int);
void stage_3(lamp_t[], int);
void stage_4(lamp_t[], int);


int
main(int argc, char *argv[]) {
	lamp_t lamps[MAX_LAMPS];
	coordinates_t locations[MAX_LOCATIONS];
	int lamp_count=0, loc_count=0;
	read_input(lamps, &lamp_count, locations, &loc_count);
	stage_1(lamps, lamp_count);
	stage_2(lamps, lamp_count, locations, loc_count);
	stage_3(lamps, lamp_count);
	stage_4(lamps, lamp_count);
	return 0;
}

/* Takes an array of lamps and a location, calculates the combined illuminance
   at the point according to the formula defined below. */
double
calc_intensity (lamp_t lamps[], int n, coordinates_t point){
	/* Illuminance from a single lamp is given by l/(4PIr^2) where
	l is lamp.output
	r is the distance from the point in question to the light source
	given by sqrt((x-lamp.x)^2+(y-lamp.y)^2+height^2) */
	double intensity=0, distance;
	int i;
	for(i=0;i<n;i++){
		distance = sqrt((point.x-lamps[i].pos.x)*(point.x-lamps[i].pos.x)+
			(point.y-lamps[i].pos.y)*(point.y-lamps[i].pos.y)+POLE_HEIGHT*POLE_HEIGHT);
		intensity += lamps[i].output/(4*PI*distance*distance);
	}
	return intensity;
}

/* Simply creates a coordinates_t object from the provided doubles, and feeds 
   into calc_intensity. DO I NEED coordinates_t DATA TYPE (PROBABLY NOT) */
double
intensity_at_point(double x, double y, lamp_t lamps[], int lamp_count){
	coordinates_t coord;
	coord.x = x;
	coord.y = y;
	return calc_intensity(lamps, lamp_count, coord);
}

/* Takes illuminance at a position as an input, prints the appropriate character
   for the maps used in parts 4 and 5 */
void
print_map_cell(double illuminance){
	char cell_values[]={'-',' ','2',' ','4',' ','6',' ','8',' '}, cell;
	if(illuminance<10){
		cell = cell_values[(int)floor(illuminance)];
	}else{
		cell = '+';
	}
	printf("%c", cell);
}

/* Reads through the input. When a flag character is found, reads into the
   relevant array. Raises errors when too little data is provided (i.e. two or
   less fields for a P line, one or less fields for an L or B line). Will not
   raise errors if more data fields than necessary are provided, or if the
   arrays storing the read data overflow (by default, 100 lines in each catagory)
   */
void 
read_input(lamp_t lamps[], int *n, coordinates_t locations[], int *m){
	char c;
	while((c=getchar())!=EOF){
		if(c == 'P'){
			if(*n<MAX_LAMPS && scanf("%lf %lf %lf", 
				&lamps[*n].pos.x, &lamps[*n].pos.y,
				&lamps[*n].output)==3){
				*n+=1;
			}else if(*n<MAX_LAMPS){
			/* If the scanf fails (but the lamps array is not
			overflowing), print out the number of the offending 
			line of input and exits. Similar error handling for
			location and boundary inputs. */
				printf("Input error, lamppost entry %d", *n+1);
				exit(EXIT_FAILURE);				
			}
		}
		if(c == 'L'){
			if(*m<MAX_LOCATIONS && scanf("%lf %lf", &locations[*m].x, &locations[*m].y)==2){
				*m+=1;
			}else if(*m<MAX_LAMPS){
				printf("Input error, location entry %d", *m+1);
				exit(EXIT_FAILURE);
			}
		}
	}
}

/* Takes the read lamp array, and its size. Prints the output for stage 1:
   1. The number of lamps present.
   2. The illuminance provided at the origin. */
void
stage_1(lamp_t lamps[], int n){
	double intensity;
	coordinates_t origin={0,0};
	intensity=calc_intensity(lamps, n, origin);
	printf("Stage 1\n=======\n");
	printf("Number of light poles: %d\n", n);
	printf("Illuminance at (0.0, 0.0): %.2f lux\n\n", intensity);
}

/* Takes the lamp array and the list of locations specified. Prints the 
   illuminance at each of those locations */
void 
stage_2(lamp_t lamps[], int n, coordinates_t locations[], int m){
	int i;
	double intensity;
	printf("Stage 2\n=======\n");
	for(i=0;i<m;i++){
		intensity=calc_intensity(lamps, n, locations[i]);
		printf("Illuminance at (%.1f, %.1f): %.2f lux\n", locations[i].x,
			locations[i].y, intensity);
	}
	printf("\n");
}

/* Calculates the illuminance at every point on a (default) 78x78 grid. Prints
   the number of points samples, and the number and proportion found to have
   illuminance below 1 lux. */
void
stage_3(lamp_t lamps[], int lamp_count){
	int i, j, low_count=0, point_count=0;
	for(i=1; i<X_LENGTH; i++){
		for(j=1;j<Y_LENGTH;j++){
			if(intensity_at_point(i, j, lamps, lamp_count) < 1.0){
				low_count+=1;
			}
			point_count+=1;
		}
	}
	printf("Stage 3\n=======\n");
	printf("With %d poles in use and %d points sampled, %d points (%.1f%%)",
		lamp_count, point_count, low_count, (double)low_count/point_count*100);
	printf("\nhave illuminance below the 1.0 lux acceptable level.\n");
}
/* Calculates the illuminance at the centre of each cell in a 78x39 grid, where
   each cell is 1x2m. These values are stored in a 2x2 array, for further use. 
   Prints a map of every cell in the grid. */
void
stage_4(lamp_t lamps[], int lamp_count){
	int i, j;
	double map[Y_LENGTH/2][X_LENGTH];
	/* Since each cell is 2m long in the y direction, there are Y_LENGTH/2
	   cells required.
	   
	   Iterate over every cell, calculate the intensity and print the 
	   corresponding character */
	printf("Stage 4\n=======\n");
	for(i=Y_LENGTH/2-1;i>=0;i--){
	/* Since y=0 should be displayed as the bottom of the graph, not the top
	   the y values are iterated through backwards */
		for(j=0;j<X_LENGTH;j++){
			map[i][j] = intensity_at_point(j+0.5, 2*i+1, lamps, lamp_count);
			/* x and y values passed to intensity function are modified
			to be the centre of the cell they refer to */
			print_map_cell(map[i][j]);
		}
		printf("\n");
	}
}
