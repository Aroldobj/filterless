#include "helpers.h"
#include <math.h>
#include <cs50.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            int average = round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;
        }
    }
    return;
}

void swap(RGBTRIPLE *pixel1, RGBTRIPLE *pixel2)
{
    RGBTRIPLE temp = *pixel1;
    *pixel1 = *pixel2;
    *pixel2 = temp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swap(&image[i][j], &image[i][width - 1 - j]);
        }
    }
}

bool is_valid_pixel(int i, int j, int height, int widht)
{
    return i >= 0 && i < height && j >= 0 && j < widht;
}

RGBTRIPLE get_blurred_pixel(int i, int j, int height, int widht, RGBTRIPLE image[height][widht])
{
     int redValue, blueValue, greenValue = redValue = blueValue = greenValue = 0;
     int num0Validpixels = 0;
     for (int di = -1; di <= 1; di++)
     {
        for (int dj = -1; dj <= 1; dj++)
        {
            int new_i = i + di;
            int new_j = j + dj;
            if (is_valid_pixel(new_i, new_j, height, widht))
            {
                num0Validpixels++;
                redValue += image[new_i][new_j].rgbtRed;
                blueValue += image[new_i][new_j].rgbtBlue;
                greenValue += image[new_i][new_j].rgbtGreen;
            }
        }
    }
    RGBTRIPLE blurred_pixel;
    blurred_pixel.rgbtRed = round((float)redValue / num0Validpixels);
    blurred_pixel.rgbtBlue = round((float)blueValue / num0Validpixels);
    blurred_pixel.rgbtGreen = round((float)greenValue / num0Validpixels);
    return blurred_pixel;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new_image[i][j] = get_blurred_pixel(i, j, height, width, image);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
    return;
}

int cap(int value)
{
    return value > 255 ? 255 : value;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    int Gx[3][3] = {{-1, 0, 1},{-2, 0, 2},{-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1},{0, 0, 0},{1, 2, 1}};

    // loop trought each row and column
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redX = 0;
            int blueX = 0;
            int greenX = 0;
            int redY = 0;
            int blueY = 0;
            int greenY = 0;

            // loop thought each pixel for neighbouring pixel
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                     // chech valid pixel
                     if (i - 1 + x < 0 || i - 1 + x > height - 1 || j - 1 + y < 0 || j -1 + y > width - 1)
                     {
                         continue;
                     }

                     // Calculate Gx for each color
                     redX = redX + (image[i - 1 + x][j - 1 + y].rgbtRed * Gx[x][y]);
                     blueX = blueX + (image[i - 1 + x][j - 1 + y].rgbtBlue * Gx[x][y]);
                     greenX = greenX + (image[i - 1 + x][j - 1 + y].rgbtGreen * Gx[x][y]);

                     // Calculate Gy for each color
                     redY = redY + (image[i - 1 + x][j - 1 + y].rgbtRed * Gx[x][y]);
                     blueY = blueY + (image[i - 1 + x][j - 1 + y].rgbtBlue * Gx[x][y]);
                     greenY = greenY + (image[i - 1 + x][j - 1 + y].rgbtGreen * Gx[x][y]);
                }
            }

            // Calculate square root Gx2 and Gy2
            int red = round(sqrt((redX * redX) + (redY * redY)));
            int green = round(sqrt((greenX * greenX) + (greenY * greenY)));
            int blue = round(sqrt((blueX * blueX) + (blueY * blueY)));

            // cap value at 255 if exceeds
            if ( red > 255)
            {
                red = 255;
            }
            if ( green > 255)
            {
                green = 255;
            }
            if ( blue > 255)
            {
                blue = 255;
            }

            // Copy values into temp image
            temp[i][j].rgbtRed = red;
            temp[i][j].rgbtGreen = green;
            temp[i][j].rgbtBlue = blue;

        }
    }
    // Copy new pixels into original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
