#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Prototype for Reflect swap function
void swap(RGBTRIPLE *a, RGBTRIPLE *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // To calculate sum of avarage we have to "+" each of element and then "/" number how many elements do we have
            // In this case for accurancy we need to round the number
            float greysum = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            image[i][j].rgbtBlue = greysum;
            image[i][j].rgbtRed = greysum;
            image[i][j].rgbtGreen = greysum;
        }
    }
    return;
}

// actual swap function for reflect image
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    *a = *b;
    *b = temp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // we need (width / 2) because we want loop to stop counting at the right moment, to avoid repetedly count
            if (j < width / 2)
            {
                swap(&image[i][j], &image[i][width - (j + 1)]);
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE sum[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // declaration of variables
            float red = 0.0;
            float green = 0.0;
            float blue = 0.0;
            int counter = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int p = -1; p < 2; p++)
                {
                    // Detection of edges and some other cases
                    if (i + k >= 0 && i + k < width && j + p >= 0 && j + p < height)
                    {
                        red += image[i + k][j + p].rgbtRed;
                        green += image[i + k][j + p].rgbtGreen;
                        blue += image[i + k][j + p].rgbtBlue;
                        counter++;
                    }
                }
            }
            sum[i][j].rgbtRed = round(red / counter);
            sum[i][j].rgbtGreen = round(green / counter);
            sum[i][j].rgbtBlue = round(blue / counter);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = sum[i][j].rgbtRed;
            image[i][j].rgbtGreen = sum[i][j].rgbtGreen;
            image[i][j].rgbtBlue = sum[i][j].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE sum[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum[i][j] = image[i][j];
        }
    }

    //  3×3 kernels or matrix One for horizontal and another for vertical changes
    int Gx[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};

    int Gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Declaration of variables
            float Gx_red = 0.0;
            float Gx_green = 0.0;
            float Gx_blue = 0.0;
            float Gy_red = 0.0;
            float Gy_green = 0.0;
            float Gy_blue = 0.0;

            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    // Similar at blur function
                    if (i + x >= 0 && i + x < width && j + y >= 0 && j + y < height)
                    {
                        // Since the Sobel kernels can be decomposed as the products of an averaging and a differentiation kernel,
                        // they compute the gradient with smoothing
                        Gx_red += sum[i + x][j + y].rgbtRed * Gx[x + 1][y + 1];
                        Gx_green += sum[i + x][j + y].rgbtGreen * Gx[x + 1][y + 1];
                        Gx_blue += sum[i + x][j + y].rgbtBlue * Gx[x + 1][y + 1];

                        Gy_red += sum[i + x][j + y].rgbtRed * Gy[x + 1][y + 1];
                        Gy_green += sum[i + x][j + y].rgbtGreen * Gy[x + 1][y + 1];
                        Gy_blue += sum[i + x][j + y].rgbtBlue * Gy[x + 1][y + 1];
                    }
                }
            }

            // At each point in the image, the resulting gradient approximations can be combined to give the gradient magnitude
            int Red = round(sqrt((Gx_red * Gx_red) + (Gy_red * Gy_red)));
            int Green = round(sqrt((Gx_green * Gx_green) + (Gy_green * Gy_green)));
            int Blue = round(sqrt((Gx_blue * Gx_blue) + (Gy_blue * Gy_blue)));

            if (Red > 255)
            {
                Red = 255;
            }
            if (Green > 255)
            {
                Green = 255;
            }
            if (Blue > 255)
            {
                Blue = 255;
            }

            image[i][j].rgbtRed = Red;
            image[i][j].rgbtGreen = Green;
            image[i][j].rgbtBlue = Blue;
        }
    }
    return;
}
