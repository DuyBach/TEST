#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "bitmap.h"
#include "color.h"
#include "raytrace.h"
#include "util.h"

int raytracer_simple(const char* filename){

	printf("%s      :  ", filename);
	unsigned long start, end;

	//init time measurement
	start = current_time_millis();

	//init raytracer
	vec_t bounds[4];
	scene_t *scene = create_scene();
	calculate_casting_bounds(scene->cam, bounds);

	//Allocate buffer for picture data
	pix_t *img = (pix_t*) calloc(HEIGHT * WIDTH, sizeof(pix_t));
	if (img){

		//calculate the data for the image (do the actual raytrace)
		raytrace(img, bounds, scene, 0, 0, WIDTH, HEIGHT);

		delete_scene(scene);

		//open file
		FILE *file = fopen(filename, "wb");
		if (file) {
			//write the header
			write_bitmap_header(file, WIDTH, HEIGHT);

			//write image to file on disk
			fwrite(img, 3, WIDTH * HEIGHT, file);

			//free buffer
			free(img);
			
			//close file
			fclose(file);
			
			//print the measured time
			end = current_time_millis();
			printf("Render time: %.3fs\n", (double) (end - start) / 1000);
			
			return EXIT_SUCCESS;
		}
	}
	return EXIT_FAILURE;
}

int raytracer_loop(const char* filename, int processcount){

	printf("%s (%i)    :  ", filename, processcount);
	unsigned long start, end;

	//init time measurement
	start = current_time_millis();

	//init raytracer
	vec_t bounds[4];
	scene_t *scene = create_scene();
	calculate_casting_bounds(scene->cam, bounds);

	//TODO: split image calculation into segments
	//create array with y-offset for each segment
	int space = HEIGHT / processcount;
	int splitArray[processcount][2];

	splitArray[0][0] = 0;
	splitArray[0][1] = space;

	int i;
	for(i = 1; i < processcount; i++) {
		splitArray[i][0] = splitArray[i-1][0] + space;
		splitArray[i][1] = splitArray[i-1][1] + space;
	}

	//TODO: write segments to file
	FILE *file =  fopen(filename, "wb");
	//write header
	write_bitmap_header(file, WIDTH, HEIGHT);
	fclose(file);

	pix_t *img;

	for(i = 0; i < processcount; i++) {
		file =  fopen(filename, "ab");

		img = (pix_t*) calloc(HEIGHT * WIDTH, sizeof(pix_t));
		if (img){
			//calculate the data for the image (do the actual raytrace)
			raytrace(img, bounds, scene, 0, splitArray[i][0], WIDTH, splitArray[i][1]);
			//write into file
			fwrite(img, 3, WIDTH * space, file);
			free(img);
			fclose(file);
			
		}
	}
	
	delete_scene(scene);

	//print the measured time
	end = current_time_millis();
	printf("Render time: %.3fs\n", (double) (end - start) / 1000);
	
	return EXIT_SUCCESS;
	//return EXIT_FAILURE;
}

int raytracer_parallel(const char* filename, int processcount){	
	//moved printf to the end of the function
	//printf("%s (%i):  ", filename, processcount);
	unsigned long start, end;
	
	//init time measurement
	start = current_time_millis();

	//init raytracer
	vec_t bounds[4];
	scene_t *scene = create_scene();
	calculate_casting_bounds(scene->cam, bounds);


	//TODO: spawn processes for each frame
	//create array with y-offset for each segment
	int space = HEIGHT / processcount;
	int splitArray[processcount][2];

	splitArray[0][0] = 0;
	splitArray[0][1] = space;

	int i;
	for(i = 1; i < processcount; i++) {
		splitArray[i][0] = splitArray[i-1][0] + space;
		splitArray[i][1] = splitArray[i-1][1] + space;
	}

	FILE *file = fopen(filename, "wb");
	//write header
	write_bitmap_header(file, WIDTH, HEIGHT);
	fclose(file);

	pid_t pid;
	int counter = 0;

	//spawn child-process
	for(i = 0; i < processcount-1; i++) {
		pid = fork();
		
		if(pid == 0) {
			//if child
			//break out of for-loop
			break;
		} else if ( pid > 0) {
			//if parent
			//counter++ and keep fork() up;
			counter++;
		} else {
			//fork failed
			printf("fork() failed!\n");
			return EXIT_FAILURE;
		}
	}

	//TODO: seek to the right position in the bitmap file and write frame
	//Allocate buffer for picture data
	pix_t *img = (pix_t*) calloc(HEIGHT * WIDTH, sizeof(pix_t));
	if (img){

		//calculate the data for the image (do the actual raytrace)
		raytrace(img, bounds, scene, 0, splitArray[counter][0], WIDTH, splitArray[counter][1]);

		delete_scene(scene);

		//open file
		FILE *file = fopen(filename, "ab");
		if (file) {
			//wait till the child before writes into the file so for example: child2 writes before child1 and the picture is in wrong order.
			int size = 26;
			if (processcount > 1) {
				while((counter*(1440000/(processcount)))+26 != size) {
					fseek(file, 0, SEEK_END); //seek to end of file
					size = ftell(file); //get current file pointer
					fseek(file, 0, SEEK_SET); //seek back to beginning of file
				}
			}

			//write image to file on disk
			fwrite(img, 3, WIDTH * space, file);

			//free buffer
			free(img);
		
			//close file
			fclose(file);
		}
	}

	//if child exit
	if (counter != processcount-1) {
		exit(0);
	} else {
		printf("%s (%i):  ", "image-parallel.bmp", processcount);
	}

	//TODO: wait for all child processes to finish before stopping time
	int status = 0;
	pid_t wpid;
	
	while ((wpid = wait(&status)) > 0);

	//print the measured time
	end = current_time_millis();
	printf("Render time: %.3fs\n", (double) (end - start) / 1000);
	

	return EXIT_SUCCESS;
	//return EXIT_FAILURE;
}

int main(int argc, char** argv) {

	if (argc != 2){
		printf("Usage: raytracer PROCESSCOUNT\n");
		return EXIT_FAILURE;
	}
	
	unsigned int processcount = strtol(argv[1], NULL, 10);

	if (raytracer_simple("image-simple.bmp") != EXIT_SUCCESS){
		printf("Error or not implemented.\n\n");
	}
	
	if (raytracer_loop("image-loop.bmp", processcount) != EXIT_SUCCESS){
		printf("Error or not implemented.\n\n");
	}

	if (raytracer_parallel("image-parallel.bmp", processcount) != EXIT_SUCCESS){
		printf("Error or not implemented.\n\n");
	}


	return 0;
}
