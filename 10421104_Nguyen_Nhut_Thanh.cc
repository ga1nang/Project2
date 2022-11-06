#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include ".\headers\stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION  
#include ".\headers\stb_image_write.h"

unsigned char calculate_image(unsigned char *image, unsigned char *template, int start_i, int start_j, int heightt, int widtht, int channelt, unsigned char tmp[], unsigned char r[], unsigned char g[], unsigned char b[])
{
    for (int i=start_i; i < start_i + heightt; i++)
    {
        for (int j=start_j; j < start_j + widtht; j++)
        {
            tmp[(i - start_i)*widtht + (j-start_j)] = 0;
            for (int k = 0; k < channelt; k++)
            {
                tmp[(i - start_i)*widtht + (j-start_j)] += abs(image[i*widtht*channelt + j*channelt + k] - template[(i - start_i)*widtht*channelt + (j-start_j)*channelt + k]);
                //printf("%d ", abs(image[i*widtht*channelt + j*channelt + k] - template[(i - start_i)*widtht*channelt + (j-start_j)*channelt + k]));
            }
            //printf("\n");
            //printf("%d ", tmp[(i - start_i)*widtht + (j-start_j)]);
            r[(i - start_i)*widtht + (j-start_j)] = abs(image[i*widtht*channelt + j*channelt + 0] - template[(i - start_i)*widtht*channelt + (j-start_j)*channelt + 0]);
            g[(i - start_i)*widtht + (j-start_j)] = abs(image[i*widtht*channelt + j*channelt + 1] - template[(i - start_i)*widtht*channelt + (j-start_j)*channelt + 1]);
            b[(i - start_i)*widtht + (j-start_j)] = abs(image[i*widtht*channelt + j*channelt + 2] - template[(i - start_i)*widtht*channelt + (j-start_j)*channelt + 2]);
        }
    }  
}

int conditionn(unsigned char tmp[], unsigned char r[], unsigned char g[], unsigned char b[], int heightt, int widtht, unsigned int *sum_of_image, unsigned int *red, unsigned int *green, unsigned int *blue)
{
    for (int i = 0; i < heightt; i++)
    {
        for (int j = 0; j < widtht; j++)
        {
            *sum_of_image += tmp[i*widtht + j];
            *red += r[i*widtht + j];
            *green += g[i*widtht + j];
            *blue += b[i*widtht + j];
            //printf("%d\n", g[i*widtht + j]);

            if (r[i*widtht + j] > 210 && g[i*widtht + j] > 50 && b[i*widtht + j] > 219)
            {
                return 0;
            }    
        } 
    }

    return 1;
}

int main()
{
    //load template
    int widtht, heightt, channelt;
    char path_template[100] = "E:\\subject\\Programming1\\Project2\\template.jpg";

    unsigned char *template = stbi_load(path_template, &widtht, &heightt, &channelt, 0);
    if (template == NULL)
    {
        printf("\nError 1");
        exit(1);
    }
    printf("Template:\nWidth = %d\nHeight = %d\nChannel = %d\n\n", widtht, heightt, channelt);



    //load image
    int width, height, channel;
    char path_img[100] = "E:\\subject\\Programming1\\Project2\\images\\img62.jpg";
    char save_path[100] = "E:\\subject\\Programming1\\Project2\\res\\res62.jpg";
    unsigned char *image = stbi_load(path_img, &width, &height, &channel, 0);

    if (image == NULL)
    {
        printf("\nError 2");
        exit(1);
    }
    printf("Image:\nWidth = %d\nHeight = %d\nChannel = %d\n", width, height, channel);

    //process
    unsigned char tmp[34240];
    unsigned char r[34240];
    unsigned char g[34240];
    unsigned char b[34240];

    int draw_i = 0;
    int draw_j = 0;
    int minn = 1000000000;
    int minnr = 1000000000;
    int minng = 1000000000;
    int minnb = 1000000000;
    unsigned int sum_of_image = 0;
    unsigned int red = 0;
    unsigned int green = 0;
    unsigned int blue = 0;
    int check = 0;

    for (int i = 101; i < height -130; i++)
    {
        for (int j = 270; j < width - 180; j++)
        {
            sum_of_image = 0;
            red = 0;
            green = 0;
            blue = 0;
            calculate_image(image, template, i, j, heightt, widtht, channelt, tmp, r, g, b);
            //return 0;
            if (conditionn(tmp,r, g, b, heightt, widtht, &sum_of_image, &red, &green, &blue) && red < minnr && green < minng && blue < minnb && minn > sum_of_image)
            {
                draw_i = i;
                draw_j = j;
                minn = sum_of_image;
                minnr = red; minng = green; minnb = blue;
                check++;
            }
        }     
    }

    for (int i = draw_i; i < draw_i + heightt; i++)
    {
        for (int j = draw_j; j < draw_j + widtht; j++)
        {
            template[(i-draw_i)*widtht*channelt + (j-draw_j)*channelt + 0] = image[i*width*channel + j*channel];
            template[(i-draw_i)*widtht*channelt + (j-draw_j)*channelt + 1] = image[i*width*channel + j*channel + 1];
            template[(i-draw_i)*widtht*channelt + (j-draw_j)*channelt + 2] = image[i*width*channel + j*channel + 2];
        }
    }

    for (int i = draw_i; i < draw_i + 4; i++)
    {
        for (int j = draw_j; j < draw_j + widtht; j++)
        {
            image[i*width*channel + j*channel] = 255;
            image[i*width*channel + j*channel + 1] = 0;
            image[i*width*channel + j*channel + 2] = 0;
        }
    }

    for (int i = draw_i; i < draw_i + heightt; i++)
    {
        for (int j = draw_j; j < draw_j + 4; j++)
        {
            image[i*width*channel + j*channel] = 255;
            image[i*width*channel + j*channel + 1] = 0;
            image[i*width*channel + j*channel + 2] = 0;
        }
    }

    for (int i = draw_i + heightt - 4; i < draw_i + heightt; i++)
    {
        for (int j = draw_j; j < draw_j + widtht; j++)
        {
            image[i*width*channel + j*channel] = 255;
            image[i*width*channel + j*channel + 1] = 0;
            image[i*width*channel + j*channel + 2] = 0;
        }
    }

    for (int i = draw_i; i < draw_i + heightt; i++)
    {
        for (int j = draw_j + widtht - 4; j < draw_j + widtht; j++)
        {
            image[i*width*channel + j*channel] = 255;
            image[i*width*channel + j*channel + 1] = 0;
            image[i*width*channel + j*channel + 2] = 0;
        }
    }
    
    printf("Saving image...\n");

    //save iamge
    char path_template_1[100] = "E:\\subject\\Programming1\\Project2\\template2.jpg";
    stbi_write_png(path_template_1, widtht, heightt, channelt, template, widtht*channelt);

    //char save_path[100] = "E:\\subject\\Programming1\\Project2\\result\\res1.jpg";
    stbi_write_png(save_path, width, height, channel, image, width*channel);

    printf("Finish image\n");

    return 0;
} 
