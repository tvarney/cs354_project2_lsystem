
#include "ImageIO.hpp"
#include "Image.hpp"

#include <cassert>
#include <cctype>
#include <cstdio>
#include <exception>
#include <png.h>
#include <stdexcept>

using namespace cs354;

/* Support just lossless formats */
static const char _bmp_ext[] = ".bmp";
static const char _png_ext[] = ".png";
static const char _tga_ext[] = ".tga";

inline bool str_iequals(const char *s1, const char *s2) {
    while(*s1 != '\0' && *s2 != '\0') {
        if(std::toupper(*s1) != std::toupper(*s2)) {
            return false;
        }
        s1++;
        s2++;
    }
    return (*s1 == *s2);
}

Image * ImageIO::Load(const char *fname) {
    assert(fname != NULL);
    
    Image *img = NULL;
    FILE *fp = fopen(fname, "rb");
    if(!fp) {
        std::string msg = std::string("ImageIO:: Could not open image: ") +
            fname;
        throw std::runtime_error(msg);
    }
    
    const char *pos = fname, *lastdot = NULL;
    while(*pos != '\0') {
        if(*pos == '.') {
            lastdot = pos;
        }
        pos += 1;
    }
    if(lastdot == NULL) {
        /* No filename hint, just load it as an opaque file pointer */
        img = ImageIO::Load(fp);
    }else if(str_iequals(lastdot, _bmp_ext)) {
        img = ImageIO::LoadBMP(fp);
    }else if(str_iequals(lastdot, _png_ext)) {
        img = ImageIO::LoadPNG(fp);
    }else if(str_iequals(lastdot, _tga_ext)) {
        img = ImageIO::LoadTGA(fp);
    }else {
        img = ImageIO::Load(fp);
    }
    
    fclose(fp);
    if(!img) {
        std::string msg = std::string("ImageIO:: Could not load image: ") +
            fname;
        throw std::runtime_error(msg);
    }
    return img;
}

Image * ImageIO::Load(FILE *fp) {
    assert(fp != NULL);
    
    Image *img = NULL;
    try {
        img = ImageIO::LoadBMP(fp);
    }catch(...) {
        try {
            img = ImageIO::LoadPNG(fp);
        }catch(...) {
            try {
                img = ImageIO::LoadBMP(fp);
            }catch(...) {
                
            }
        }
    }
    
    if(!img) {
        std::string msg = std::string("ImageIO:: Could not load image");
        throw std::runtime_error(msg);
    }
    return img;
}

Image * ImageIO::LoadBMP(FILE *fp) {
    return NULL;
}
#define PNG_ERROR1(msg, fp, pos)   \
    fseek(fp, pos, SEEK_SET);      \
    throw std::runtime_error(std::string(msg))
#define PNG_ERROR2(msg, fp, pos, png_pp, info_pp, end_pp) \
    png_destroy_read_struct(png_pp, info_pp, end_pp);     \
    PNG_ERROR1(msg, fp, pos)

static const char _png_struct_alloc_err[] =
    "ImageIO:: Could not allocate png structures";
Image * ImageIO::LoadPNG(FILE *fp) {
    assert(fp != NULL);
    
    long int pos = ftell(fp);
    png_byte header[8];
    if(fread(header, 1, 8, fp) != 8 || png_sig_cmp(header, 0, 8)) {
        PNG_ERROR1("ImageIO:: Invalid PNG file", fp, pos);
    }
    
    png_structp png_ptr =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if(!png_ptr) {
        PNG_ERROR2(_png_struct_alloc_err, fp, pos, &png_ptr, NULL, NULL);
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        PNG_ERROR2(_png_struct_alloc_err, fp, pos, &png_ptr, NULL, NULL);
    }
    png_infop end_ptr = png_create_info_struct(png_ptr);
    if(!end_ptr) {
        PNG_ERROR2(_png_struct_alloc_err, fp, pos, &png_ptr, &info_ptr, NULL);
    }
    
    if(setjmp(png_jmpbuf(png_ptr))) {
        PNG_ERROR2("ImageIO:: Error reading PNG file", fp, pos, &png_ptr,
                   &info_ptr, &end_ptr);
    }
    
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    
    uint32_t width = png_get_image_width(png_ptr, info_ptr);
    uint32_t height = png_get_image_height(png_ptr, info_ptr);
    uint32_t bitdepth = png_get_bit_depth(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    switch(color_type) {
    case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(png_ptr);
        channels = 3;
        break;
    case PNG_COLOR_TYPE_GRAY:
        if(bitdepth < 8) {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
            bitdepth = 8;
        }
        break;
    }
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
        channels += 1;
    }
    if(bitdepth == 16) {
        png_set_strip_16(png_ptr);
        bitdepth = 8;
    }
    
    uint32_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if(rowbytes != width * bitdepth * channels / 8) {
        fputs("ImageIO: Stride length is wrong!\n", stderr);
    }
    rowbytes += 3 - ((rowbytes - 1) % 4);
    
    PixelFormat format = (channels == 4) ? PF_RGBA : PF_RGB;
    Image *img = new Image(width, height, rowbytes, format);
    uint8_t * data = img->getMutableData();
    png_bytep *row_pointers = new png_bytep[height];
    for(uint32_t i = 0; i < height; ++i) {
        row_pointers[height - 1 - i] = data + i * rowbytes;
    }
    png_set_interlace_handling(png_ptr);
    png_read_image(png_ptr, row_pointers);
    
    delete row_pointers;
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
    return img;
}
Image * ImageIO::LoadTGA(FILE *fp) {
    return NULL;
}

#define PPM_BUFFER_SIZE 4096
#define PPM_GETLINE(buffer, fp)                 \
    do {                                        \
        fgets(buffer, PPM_BUFFER_SIZE, fp);     \
    }while(strncmp(buffer, "#", 1) == 0)

Image * ImageIO::LoadPPM(FILE *fp) {
    char buffer[PPM_BUFFER_SIZE];
    int32_t w, h, max, i;
    //int j, k;
    //uint8_t rgb[3];
    
    fgets(buffer, PPM_BUFFER_SIZE, fp);
    if(strncmp(buffer, "P6", 2) != 0) {
        fputs("ImageIO:: Invalid PPM file\n", stderr);
        return NULL;
    }
    
    PPM_GETLINE(buffer, fp);
    if(sscanf(buffer, "%d %d", &w, &h) != 2) {
        fputs("ImageIO:: Could not read size of image\n", stderr);
        return NULL;
    }
    
    PPM_GETLINE(buffer, fp);
    if(sscanf(buffer, "%d", &max) != 1) {
        fputs("ImageIO:: No maximum size specified\n", stderr);
        return NULL;
    }
    
    Image *img = new Image(w, h, PF_RGB);
    uint8_t *pixels = img->getMutableData();
    size_t rowsize = sizeof(uint8_t) * w * 3;
    
    printf("ImageIO:: Reading %d columns and %d rows\n", w, h);
    for(i = h - 1; h >= 0; --i) {
        if(fread(pixels+i*rowsize, rowsize, 1, fp) != rowsize) {
            fputs("ImageIO:: EOF while reading data\n", stderr);
            delete img;
            return NULL;
        }
    }
    /* The for-loop structure used in the starter code. For reference.
    for(i = 0; i < h; ++i) {
        for(j = 0; j < w; ++j) {
            fread(rgb, sizeof(uint8_t), 3, input);
            for(k = 0; k < 3; ++k) {
                *(pixels+(h-1-i)*w*3+j*3+k) = rgb[k];
            }
        }
    }
    */
    return img;
}
