#include "imageprocess.h"
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/core/core.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <random>

void rgb::operator+(const int &v)
{
    R += v;
    G += v;
    B += v;
}

void rgb::operator-(const int &v)
{
    R -= v;
    G -= v;
    B -= v;
}

void rgb::operator/(const int &v)
{
    R /= v;
    G /= v;
    B /= v;
}

void rgb::operator*(const int &v)
{
    R *= v;
    G *= v;
    B *= v;
}

rgb &rgb::operator+=(const rgb &v)
{
    this->R += v.R;
    this->G += v.G;
    this->B += v.B;
    return *this;
}

rgb &rgb::operator/=(const int &v)
{
    this->R /= v;
    this->G /= v;
    this->B /= v;
    return *this;
}

double randDouble(const double & min, const double & max) {
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<double> distribution(min,max);
    return distribution(generator);
}

double newcalculateAveragePi(int dividedBy)
{
    int l=0;
    for (size_t i = 0; i < 1000000000; i++)
    {
        l+=i;
    }
    
    return 1;
}

double calculateAveragePi(int dividedBy)
{
    long long totalPoints = 1000000000;
    long long withinCircle = 0;
    
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);


    for (long long i = 0; i < totalPoints; i++)
    {
        // double x = randDouble(0., 1.);
        // double y = randDouble(0., 1.);
        double x = dist(e2);
        double y = dist(e2);
        // double x=(double)rand()/(double)RAND_MAX;
        // double y=(double)rand()/(double)RAND_MAX;
        double distance2 = x*x+y*y;

        if (distance2 < 1)
            withinCircle++;
    }
    return 4. * static_cast<double>(withinCircle) / static_cast<double>(totalPoints * dividedBy);
}

/**
 * Give the average color (RGB) of an image
 *
 * @param file Path to the image
 * @returns RGB values
 */
rgb averageColorImg(std::string file)
{
    return rgb(100,50,0);
}

/**
 * Is image containing only grayscale colors
 *
 * @param file Path to the image
 * @returns if it is only gray
 */
bool isImgGray(std::string file)
{
    return true;
}

/**
 * Convert a RGB color into HSV values
 *
 * @param color RGB values
 * @returns HSV values
 */
hsv rgbToHsv(rgb const color)
{
    hsv val;
    float f6(6.);
    float R(color.R / 255.f), G(color.G / 255.f), B(color.B / 255.f);
    // std::cout << R << " " << G << " " << B << std::endl;
    float xmax = std::max(R, std::max(G, B)),
          xmin = std::min(R, std::min(G, B));
    float chroma = xmax - xmin;

    // hue
    if (chroma == 0)
        val.H = 0;
    else if (xmax == R)
        val.H = 60. * std::fmod((G - B) / chroma, 6);
    else if (xmax == G)
        val.H = 60. * ((B - R) / chroma + 2);
    else if (xmax == B)
        val.H = 60. * ((R - G) / chroma + 4);

    //  saturation
    if (xmax == 0)
        val.S = 0;
    else
        val.S = chroma / xmax;

    // value
    val.V = xmax;
    return val;
}

rgb hsvToRgb(hsv const color)
{
    int chroma = color.V * color.S * 255;
    float f8(8.), f2(2.);
    float hue = std::fmod(color.H / 60, 8);
    int interm = chroma * (1 - abs(std::fmod(hue, 2) - 1));

    rgb val(255, 255, 255);
    if (hue <= 1.0)
        val = {chroma, interm, 0};
    else if (hue <= 2.0)
        val = {interm, chroma, 0};
    else if (hue <= 3.0)
        val = {0, chroma, interm};
    else if (hue <= 4.0)
        val = {0, interm, chroma};
    else if (hue <= 5.0)
        val = {interm, 0, chroma};
    else if (hue <= 6.0)
        val = {chroma, 0, interm};
    else if (hue <= 7.0)
        val = {interm, interm, interm};
    val.operator+(255 * color.V);
    val.operator-(chroma);

    return val;
}

/**
 * Is color belonging to a grayscale
 *
 * @param color RGB values
 * @returns belonging to a grayscale
 */
bool isColorGray(rgb const color)
{
    if (color.R == color.G && color.G == color.B)
        return true;
    else
        return false;
}

/**
 * Is color belonging to a grayscale
 *
 * @param color HSV values
 * @returns belonging to a grayscale
 */
bool isColorGray(hsv const color)
{
    return color.S == 0;
}

rgb splitColors(long const &combinedColors)
{
    int red = (combinedColors >> 16) & 0xFF;
    int green = (combinedColors >> 8) & 0xFF;
    int blue = combinedColors & 0xFF;
    return rgb(red, green, blue);
}

long combineColors(rgb const &splitColors)
{
    long rgb = splitColors.R;
    rgb = (rgb << 8) + splitColors.G;
    rgb = (rgb << 8) + splitColors.B;
    return rgb;
}

long combineColors(int const &red, int const &green, int const &blue)
{
    long rgb = red;
    rgb = (rgb << 8) + green;
    rgb = (rgb << 8) + blue;
    return rgb;
}