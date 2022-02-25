#include "seamcarving.h"
#include <stdio.h>
#include <math.h>


void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    
    create_img(grad, im->height, im->width);

    int height = im->height;
    int width = im->width;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int rx;
            int ry;
            int gx;
            int gy;
            int bx;
            int by;
            

            // for x values
            if(x+1 >= width){
                rx = get_pixel(im, y, 0, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, 0, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, 0, 2) - get_pixel(im, y, x-1, 2);
            }

            else if (x-1 < 0){
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, width-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, width-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, width-1, 2);
            }
            else{
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, x-1, 2);
            }

            // for y values
            if (y+1 >= height){
                ry = get_pixel(im, 0, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, 0, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, 0, x, 2) - get_pixel(im, y-1, x, 2);
            }
            else if (y-1 < 0){
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, height-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, height-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, height-1, x, 2);
            }
            else{
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, y-1, x, 2);
            }

            int change_x = rx*rx + gx*gx + bx*bx;
            int change_y = ry*ry + gy*gy + by*by;

            uint8_t energy = (uint8_t)((sqrt(change_x + change_y))/10);
            
            set_pixel(*grad, y, x, energy, energy, energy);
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    double check_min;
    double check_eng;
    int position = -1;
    int height = grad->height;
    int width = grad->width;

    int row;
    int col;

    *best_arr = (double *)malloc(sizeof(double)*(height*width));

    // best_arr is a really long array
    // width is the length (*best_arr)[i*width+j];

    
    for(int i = 0; i < height*width; i++){
        row = i / width;
        col = i - row*width;
        (*best_arr)[i]= get_pixel(grad, row, col, 0);
    }

    for(int x = 1; x < height; x++){
        for(int y = x*width;  y < (x+1)*width; y++){
            for(int z = -1; z <= 1; z++){
                
                if( ((y-width+z) >= x*width) || ((y-width+z) < (x-1)*width)){
                    position = 0;
                    continue;
                }
                if (z == position){
                    check_min = (*best_arr)[y-width+z] + (*best_arr)[y];
                }
 
                check_eng = (*best_arr)[y-width+z] + (*best_arr)[y];

                if(check_eng < check_min){
                    check_min = check_eng;
                }
            }
            (*best_arr)[y] = check_min;
            position = -1;
            
        }
    }
}

void recover_path(double *best, int height, int width, int **path){
    // allocate the space for path -- same as the height of *best
    int end;
    int position;
    int min_val;
    int length = height*width;
    int min1;

    *path = (int *)malloc(sizeof(int)*(height));

    // set the min val at the last row 
    end = (length / width) - 1;
    min1 = best[length-1];
    position = length-1;
    
    for (int j = end*width; j < length; j++){
        if (best[j] < min1){
            min1 = best[j];
            position = j - end*width;
        }
    }

    (*path)[height-1] = position;


    for (int x = height-1; x > 0; x--){
        end = ((length / width) - (height - x + 1)) * width;

        if ((position-1) <= 0) {
            min_val = best[end + position];
            position = position;

            if (best[end + position + 1] < min_val){
                min_val = best[end + position + 1];
                position = position + 1;
            }
        }

        if ((position + 1) >= width){
            min_val = best[end + position - 1];
            position = position - 1;
            

            if (best[end + position] < min_val){
                min_val = best[end + position];
                position = position;;
            }
        }

        else{
            min_val = best[end + position];
            position = position;

            if (best[end + position + 1] < min_val){
                min_val = best[end + position + 1];
                position = position + 1;
            }
            if (best[end + position-1] < min_val){
                min_val = best[end + position - 1];
                position = position - 1;
            }
        }
        (*path)[x-1] = position;
    }
}


void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    create_img(dest, (src->height), (src->width)-1); // -1 because the width is decreased by 1

    int height = src->height;
    int width = src->width;

    int r;
    int g;
    int b;
    int in = 0;

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width-1; x++){
            
            if (x == path[y]){
                in = 1;
                continue;
            }
            r = get_pixel(src, y, x, 0);
            g = get_pixel(src, y, x, 1);
            b = get_pixel(src, y, x, 2);


            if (in == 1){
                set_pixel(*dest, y, x-1, r, g, b);
            }
            
            else{
                set_pixel(*dest, y, x, r, g, b);
            }       
        }
        in = 0;
    }
}