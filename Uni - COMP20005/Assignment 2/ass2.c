/* Alasdair Norton -587162
 * COMP20005 Engineering Computation - Assignment 2
 * May 2013
 * Programming is Fun
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Dimensions of carpark */
#define X_LENGTH 78
#define Y_LENGTH 78
#define CELL_WIDTH 1
#define CELL_HEIGHT 2
#define X_CELLS X_LENGTH/CELL_WIDTH
#define Y_CELLS Y_LENGTH/CELL_HEIGHT

/* Array restrictions */
#define MAX_POLES 100
#define MAX_LOCATIONS 100
#define MAX_BOUNDS 100

/* Constants for calculations */
#define PI 3.14159
#define POLE_HEIGHT 8.25
#define MIN_LUX 1.0
#define MAX_LUX 10.0


typedef struct {
	double x, y;
} coordinates_t;

typedef struct {
	double output;
	coordinates_t pos;
} pole_t;

typedef struct {
	coordinates_t p1, p2;
} line_t;

double intensity_at_point(double, double, pole_t[], int);
double calc_intensity(pole_t[], int, coordinates_t);
coordinates_t polygon_centroid(coordinates_t[], int);
void make_boundary_lines(coordinates_t[], int, line_t[], int*);
void print_map_cell(double);
void read_input(pole_t[], int*, coordinates_t[], int*, coordinates_t[], int*);
void stage_1(pole_t[], int);
void stage_2(pole_t[], int, coordinates_t[], int);
void stage_3(pole_t[], int);
void stage_4(pole_t[], int, double[][X_LENGTH]);
void stage_5(double[][X_LENGTH], coordinates_t[], int);
/* LineIntersect function as provided on LMS */
int LineIntersect(line_t, line_t);

int
main(int argc, char *argv[]) {
	pole_t poles[MAX_POLES];
	coordinates_t locations[MAX_LOCATIONS], boundary[MAX_BOUNDS];
	int n_poles=0, n_locations=0, n_bounds=0;
	double map[Y_LENGTH/CELL_HEIGHT][X_LENGTH];
	
	read_input(poles, &n_poles, locations, &n_locations, boundary,
		&n_bounds);
	stage_1(poles, n_poles);
	if (n_locations > 0) {
		/* Only display stage 2 if any locations are specified */
		stage_2(poles, n_poles, locations, n_locations);
	}
	stage_3(poles, n_poles);
	stage_4(poles, n_poles, map);
	if (n_bounds > 2) { 
		/* need at least 3 boundary points to define a polygon */
		stage_5(map, boundary, n_bounds);
	}
	
	return 0;                                                       
}


/* Converts from distinct x and y values to a coordinates_t object for 
   use in calc_intensity */
double
intensity_at_point(double x, double y, pole_t poles[], int n_poles) {
	coordinates_t coord;
	coord.x = x;
	coord.y = y;
	return calc_intensity(poles, n_poles, coord);
}

/* Takes an array of lamps and a location, calculates the combined illuminance
   at the point. */
double
calc_intensity(pole_t poles[], int n_poles, coordinates_t point) {
	double intensity=0, distance;
	int i;
	for (i=0; i<n_poles; i++){
		/* Calculates distance using pythagorean theorem;
		   r = sqrt(dx^2 * dy^2 * dz^2)
		   where dz is the pole's height */
		distance = sqrt(pow((point.x - poles[i].pos.x), 2) +
			pow((point.y-poles[i].pos.y),2) + pow(POLE_HEIGHT,2));
		/* Illuminance is given by L/(4PIr^2) */
		intensity += poles[i].output/(4*PI*pow(distance,2));
	}
	return intensity;
}

/* Calculates the centroid of the polygon defined by the points provided.
   Assumes that the boundary points are given in order and that the shape
   described is not self-intersecting.
*/
coordinates_t
polygon_centroid(coordinates_t vertices[], int n_vertices) {
	coordinates_t centroid={0,0};
	double area=0;
	int i;
	/* Using the formulae for centroid and area given at
	 * http://en.wikipedia.org/wiki/Centroid#Centroid_of_polygon
	 * First find the polygon's area */
	for (i=0; i<n_vertices-1; i++) {
		area+=(vertices[i].x * vertices[i+1].y -
			vertices[i+1].x * vertices[i].y)/2;
	}
	area+=(vertices[i].x * vertices[0].y -
			vertices[0].x * vertices[i].y)/2;
	/* Next, find the coordinates of the centroid */
	for (i=0; i<n_vertices-1; i++) {
		centroid.x+=(vertices[i].x + vertices[i+1].x) *
				(vertices[i].x * vertices[i+1].y -
				vertices[i+1].x * vertices[i].y)/(6*area);
				
		centroid.y+=(vertices[i].y + vertices[i+1].y) *
				(vertices[i].x * vertices[i+1].y -
				vertices[i+1].x * vertices[i].y)/(6*area);
	}
	centroid.x+=(vertices[i].x + vertices[0].x) *
			(vertices[i].x * vertices[0].y -
			vertices[0].x * vertices[i].y)/(6*area);
			
	centroid.y+=(vertices[i].y + vertices[0].y) *
			(vertices[i].x * vertices[0].y -
			vertices[0].x * vertices[i].y)/(6*area);
	return centroid;
}

/* Converts the list of boundary points read into a list of lines */
void
make_boundary_lines(coordinates_t boundary_points[], int n_bounds,
		    line_t boundary_lines[], int *n_lines) {
	int i;
	for (i=1; i<n_bounds; i++) {
		boundary_lines[*n_lines].p1 = boundary_points[i-1];
		boundary_lines[*n_lines].p2 = boundary_points[i];
		*n_lines+=1;
	}
	/* Add the last line, from the final point listed to the first */
	boundary_lines[*n_lines].p1 = boundary_points[i-1];
	boundary_lines[*n_lines].p2 = boundary_points[0];
	*n_lines+=1;
}


/* Takes illuminance at a position as an input, prints the appropriate
   character for the maps used in parts 4 and 5 */
void
print_map_cell(double illuminance) {
	char cell_values[]={'-',' ','2',' ','4',' ','6',' ','8',' '}, cell;
	if (illuminance < MAX_LUX) {
		cell = cell_values[(int)floor(illuminance)];
	} else {
		cell = '+';
	}
	printf("%c", cell);
}

/* Reads through the input. When a flag character is found, reads the line
   into the relevant array. 
 * Provides error messages and quits for incorrectly formatted data
 * Guards against overflow in the three input arrays, but will not provide an
   error message if there is overflow */
void 
read_input(pole_t poles[], int *n_poles, coordinates_t locations[], 
	int *n_locations, coordinates_t boundary[], int *n_bounds) {
	pole_t temp_pole;
	coordinates_t temp_coord;
	char c, lastchar='\n';
	while ((c=getchar()) != EOF) {
		if (lastchar=='\n' && c == 'P') {
			if (*n_poles < MAX_POLES && scanf("%lf %lf %lf", 
				&temp_pole.pos.x, &temp_pole.pos.y, 
				&temp_pole.output) == 3) { 
				poles[*n_poles] = temp_pole;
				*n_poles+=1;
			}else if (*n_poles < MAX_POLES) {
			/* Guards agains format errors in input, shows line
			   number of incorrect line.*/
				printf("Input error: 'P' line %d", *n_poles+1);
				exit(EXIT_FAILURE);				
			}
		}
		if (lastchar=='\n' && c == 'L') {
			if (*n_locations < MAX_LOCATIONS && scanf("%lf %lf", 
				&temp_coord.x, &temp_coord.y) == 2) {
			        locations[*n_locations] = temp_coord;
				*n_locations+=1;
			}else if (*n_locations < MAX_LOCATIONS) {
				printf("Input error, 'L' line %d", 
					*n_locations+1);
				exit(EXIT_FAILURE);
			}
		}
		if (lastchar=='\n' && c == 'B') {
			if (*n_bounds < MAX_BOUNDS && scanf("%lf %lf", 
				&temp_coord.x, &temp_coord.y) == 2){
				boundary[*n_bounds] = temp_coord;
				*n_bounds+=1;
			}else if (*n_bounds < MAX_BOUNDS) {
				printf("Input error, 'B' line %d",*n_bounds+1);
				exit(EXIT_FAILURE);
			}
		}
		lastchar=c;
	}
}

/* Takes the read poles array, and its size. Prints the output for stage 1:
   1. The number of poles present.
   2. The illuminance provided at the origin. */
void
stage_1(pole_t poles[], int n_poles) {
	double intensity;
	coordinates_t origin = {0,0};
	intensity=calc_intensity(poles, n_poles, origin);
	printf("Stage 1\n=========\n");
	printf("Number of light poles: %d\n", n_poles);
	printf("Illuminance at ( 0.0, 0.0):  %.2f lux\n\n", intensity);
}

/* Takes the poles array and the list of locations specified. Prints the 
   illuminance at each of those locations */
void 
stage_2(pole_t poles[], int n_poles, coordinates_t locations[], 
	int n_locations) {
	int i;
	double intensity;
	printf("Stage 2\n=========\n");
	for (i=0; i<n_locations; i++) {
		intensity = calc_intensity(poles, n_poles, locations[i]);
		printf("Illuminance at (%3.1f,%3.1f):  %.2f lux\n", 
			locations[i].x, locations[i].y, intensity);
	}
	printf("\n");
}

/* Calculates the illuminance at every point on a (default) 78x78 grid. Prints
   the number of points samples, and the number and proportion found to have
   illuminance below 1 lux. */
void
stage_3(pole_t poles[], int n_poles) {
	int i, j, low_count=0, point_count=0;
	for (i=1; i<X_LENGTH; i++) {
		for (j=1; j<Y_LENGTH; j++) {
			if (intensity_at_point(i, j, poles, n_poles)< MIN_LUX){
				low_count+=1;
			}
			point_count+=1;
		}
	}
	printf("Stage 3\n=========\n");
	printf("With %d poles in use and %d points sampled, ",
		n_poles, point_count);
	printf("%d points (%.1f%%)", low_count, 
		(double)low_count/point_count*100);
	printf("\nhave illuminance below the 1.0 lux acceptable level\n\n");
}

/* Calculates the illuminance at the centre of each cell in a 78x39 grid, where
   each cell is 1x2m. These values are then stored in a two dimensional array.
   Prints a map of each cell in the grid. */
void
stage_4(pole_t poles[], int n_poles, double map[][X_CELLS]) {
	int i, j;
	double x_pos, y_pos;
	printf("Stage 4\n=========\n");
	for (i=Y_CELLS-1; i>=0; i--) {
	/* Since y=0 should be displayed as the bottom of the graph, not the
	   top the y values are iterated through backwards */
		for (j=0; j<X_CELLS; j++) {
			/* x and y values passed to intensity function are 
			modified to be the centre of the cell they refer to */
			x_pos = CELL_WIDTH*(j+0.5);
			y_pos = CELL_HEIGHT*(i+0.5);
			map[i][j]= intensity_at_point(x_pos, 
				y_pos, poles, n_poles);
			print_map_cell(map[i][j]);
		}
		printf("\n");
	}
}

void
stage_5(double map[][X_LENGTH], coordinates_t boundary[], int n_bounds) {
	line_t boundary_lines[MAX_BOUNDS], current_line;
	int n_lines=0, i, j, k, outside;
	/* Create the set of lines defining the polygon's outline. */
	make_boundary_lines(boundary, n_bounds, boundary_lines, &n_lines);
	/* Set the first coordinate of the line to be compared to the centroid
	   of the polygon */
	current_line.p1 = polygon_centroid(boundary, n_bounds);
	printf("\nStage 5\n=========\n");
	for (i=Y_CELLS-1; i>=0; i--) {
		for( j=0; j<X_CELLS; j++) {
			current_line.p2.x=CELL_WIDTH*(j+0.5);
			current_line.p2.y=CELL_HEIGHT*(i+0.5);
			/* Constructs the line between the current point and
			   the polygon's centroid */
			k=0;
			outside=0;
			/* Check whether this line intersects with any of the
			   boundary lines */
			while (k<n_lines && outside==0) {
				outside = LineIntersect(current_line,
					boundary_lines[k]);
				k++;
			}
			/* Print the appropriate symbol */
			if (outside == 1) {
				printf("#");
			}else{
				print_map_cell(map[i][j]);
			}
		}
		printf("\n");
	}
}

/* The following code is presented exactly as copied from the LMS (14/5/13) */
/* =============================================================== */

/* This function was adapted in 2012 from 
 * http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/pdb.c
 * (no longer available at that URL in 2013)
 * and was written in the first instance by Paul Bourke
 * Modified for use here by Alistair Moffat by:
 *   . changing the argument type to two structs type line_t
 *   . making sure result is TRUE if an endpoint is on the other line
 */
#define TRUE 1
#define FALSE 0
#define EPS (1e-06)
#define ABS(x) (fabs(x))

int LineIntersect(line_t l1, line_t l2) {
   double x1=l1.p1.x, y1=l1.p1.y,
   	  x2=l1.p2.x, y2=l1.p2.y,
   	  x3=l2.p1.x, y3=l2.p1.y,
   	  x4=l2.p2.x, y4=l2.p2.y;
   double mua,mub;
   double denom,numera,numerb;

   denom  = (y4-y3) * (x2-x1) - (x4-x3) * (y2-y1);
   numera = (x4-x3) * (y1-y3) - (y4-y3) * (x1-x3);
   numerb = (x2-x1) * (y1-y3) - (y2-y1) * (x1-x3);

   /* Are the line coincident? */
   if (ABS(numera) < EPS && ABS(numerb) < EPS && ABS(denom) < EPS) {
      return(TRUE);
   }

   /* Are the line parallel */
   if (ABS(denom) < EPS) {
      return(FALSE);
   }

   /* Is the intersection along the the segments */
   mua = numera / denom;
   mub = numerb / denom;
   /* AM - use equality here so that "on the end" is not an
    * intersection; use strict inequality if "touching at end" is an
    * intersection */
   if (mua < 0 || mua > 1 || mub < 0 || mub > 1) {
      return(FALSE);
   }
   return(TRUE);
}
