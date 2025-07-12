#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

//Source: https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/
#pragma pack(push, 1) //Basically para sa pag prevent ng unwanted padding
struct BMPFileHeader{
    uint16_t file_type{0x4D42};                //File type always BM which is 0x4D42
    uint32_t file_size{0};                     //Size of the file (in bytes)
    uint16_t reserved1{0};                     //Reserved, always 0
    uint16_t reserved2{0};                     //Reserved, always 0
    uint32_t offset_data{0};                   //Start position of pixel data (bytes from the beginning of the file)
    
};

struct BMPInfoHeader {
     uint32_t size{ 0 };                      // Size of this header (in bytes)
     int32_t width{ 0 };                      // width of bitmap in pixels
     int32_t height{ 0 };                     // width of bitmap in pixels
                                              //       (if positive, bottom-up, with origin in lower left corner)
                                              //       (if negative, top-down, with origin in upper left corner)
     uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
     uint16_t bit_count{ 0 };                 // No. of bits per pixel
     uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
     uint32_t size_image{ 0 };                // 0 - for uncompressed images
     int32_t x_pixels_per_meter{ 0 };
     int32_t y_pixels_per_meter{ 0 };
     uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
     uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};
#pragma pack(pop)

//Source: https://www.youtube.com/watch?v=nPBfluUiZ1s
int main(){

    std::ofstream myWriteFile("Images/Grayscaled.bmp", std::ios::binary); //Create / Writes Files | Para sa pag gawa ng grayscale
    std::ifstream myReadFile("Images/PIA.bmp", std::ios::binary); //Read | para sa gagamitin na bmp 
    //pwede gawing m31.bmp para mas malaki file
    
    if(myReadFile.is_open() == false){
        std::cerr << "Cannot open file." << std::endl;
        return -1;
    }

    //Reading & Writing the Headers
    BMPFileHeader file_header;
    BMPInfoHeader info_header;

    myReadFile.read((char*)&file_header, sizeof(file_header));
    myReadFile.read((char*)&info_header, sizeof(info_header));
    myWriteFile.write((char*)&file_header, sizeof(file_header));
    myWriteFile.write((char*)&info_header, sizeof(info_header));
    
    //Reading the image data
    myReadFile.seekg(file_header.offset_data, std::ios::beg);
    
    const size_t row_stride = (info_header.width * info_header.bit_count + 7) / 8;
    const int bytes_per_pixel = info_header.bit_count / 8;

    std::vector<char> image_data(row_stride * info_header.height);

    myReadFile.read(image_data.data(), image_data.size());

    myReadFile.close();

    //Grayscale shit na

    /*
    Grayscale formula = 0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue
    
    Blue at offset +0
    Green at offset +1
    Red at offset +2

    */
   
    auto start = std::chrono::high_resolution_clock::now();
    #pragma acc kernels
    for(int y = 0; y < info_header.height; y++){                    //vertical | column
        for (int x = 0; x < info_header.width; x++){                //horizontal | row
            size_t index = y * row_stride + x * bytes_per_pixel;

            unsigned char Blue = image_data[index + 0];             
            unsigned char Green = image_data[index + 1];
            unsigned char Red = image_data[index + 2];

            unsigned char gray = static_cast<unsigned char>(0.299 * Red + 0.587 * Green + 0.114 * Blue);

            image_data[index + 0] = gray;
            image_data[index + 1] = gray;
            image_data[index + 2] = gray;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Grayscale conversion took " << elapsed.count() << " ms" << std::endl;

    myWriteFile.write(image_data.data(), image_data.size());
    myWriteFile.close();
    
    std::cout << "Finished" << std::endl;

    return 0;
}