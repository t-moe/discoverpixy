#include "pixy_helper.h"
#include "pixy.h"
#include "tft.h"
#include <stdlib.h>



static int renderBA81(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);
static int saveBA81(FILE_HANDLE* handle, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);


int pixy_render_full_frame(uint16_t x, uint16_t y) {
	return pixy_render_cropped_frame(x,y,0,0,320,200);
}


int pixy_render_cropped_frame(uint16_t x, uint16_t y, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height) {
	uint8_t* videodata;
	int32_t response;
	int32_t fourccc;
	int8_t renderflags;
	uint16_t xwidth;
	uint16_t ywidth;
	uint32_t size;


	int return_value = pixy_command("cam_getFrame",  // String id for remote procedure
								  INT8(0x21),     // mode
								  INT16(xoffset),        // xoffset
								  INT16(yoffset),         // yoffset
								  INT16(width),       // width
								  INT16(height),       // height
								  END_OUT_ARGS,              // separator
								  &response,      // pointer to mem address for return value
								  &fourccc,
								  &renderflags,
								  &xwidth,
								  &ywidth,
								  &size,
								  &videodata,        // pointer to mem address for returned frame
								  END_IN_ARGS);

	if(return_value==0) {
		return_value = renderBA81(x,y,xwidth,ywidth,size,videodata);
	}

	return return_value;
}

int pixy_save_full_frame(FILE_HANDLE* handle) {
	return pixy_save_cropped_frame(handle,0,0,320,200);
}

int pixy_save_cropped_frame(FILE_HANDLE* handle, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height) {
	uint8_t* videodata;
	int32_t response;
	int32_t fourccc;
	int8_t renderflags;
	uint16_t xwidth;
	uint16_t ywidth;
	uint32_t size;


	int return_value = pixy_command("cam_getFrame",  // String id for remote procedure
								  INT8(0x21),     // mode
								  INT16(xoffset),        // xoffset
								  INT16(yoffset),         // yoffset
								  INT16(width),       // width
								  INT16(height),       // height
								  END_OUT_ARGS,              // separator
								  &response,      // pointer to mem address for return value
								  &fourccc,
								  &renderflags,
								  &xwidth,
								  &ywidth,
								  &size,
								  &videodata,        // pointer to mem address for returned frame
								  END_IN_ARGS);

	if(return_value==0) {
		return_value = saveBA81(handle,xwidth,ywidth,size,videodata);
	}

	return return_value;
}





static void interpolateBayer(uint16_t width, uint16_t x, uint16_t y, uint8_t *pixel, uint8_t* r, uint8_t* g, uint8_t* b)
{
    if (y&1)
    {
        if (x&1)
        {
            *r = *pixel;
            *g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
            *b = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
        }
        else
        {
            *r = (*(pixel-1)+*(pixel+1))>>1;
            *g = *pixel;
            *b = (*(pixel-width)+*(pixel+width))>>1;
        }
    }
    else
    {
        if (x&1)
        {
            *r = (*(pixel-width)+*(pixel+width))>>1;
            *g = *pixel;
            *b = (*(pixel-1)+*(pixel+1))>>1;
        }
        else
        {
            *r = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
            *g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
            *b = *pixel;
        }
    }

}






static int renderBA81(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame)
{
    uint16_t x, y;
    uint8_t r, g, b;


    // skip first line
    frame += width;

    // don't render top and bottom rows, and left and rightmost columns because of color
    // interpolation
    //uint32_t decodedimage[(width-2)*(height-2)];
    uint16_t* decodedimage = malloc(sizeof(uint16_t)*(width-2)*(height-2));

    if(decodedimage==NULL) { //not enough free space to decode image in memory
        //decode & render image pixel by pixel
        for (y=1; y<height-1; y++)
        {
                frame++;
                for (x=1; x<width-1; x++, frame++)
                {
                    interpolateBayer(width, x, y, frame, &r, &g, &b);
                    tft_draw_pixel(xpos+x-1,ypos+y-1,RGB(r,g,b));
                }
                frame++;
         }
    } else { //enough space
            uint16_t* line = decodedimage;
            for (y=1; y<height-1; y++)
            {
                //line = (unsigned int *)img.scanLine(y-1);
                frame++;
                for (x=1; x<width-1; x++, frame++)
                {
                    interpolateBayer(width, x, y, frame, &r, &g, &b);
                    //*line++ = (0xff<<24) | (r<<16) | (g<<8) | (b<<0);
                    *line++ = RGB(r,g,b);
                }
                frame++;
            }

            tft_draw_bitmap_unscaled(xpos,ypos,width-2,height-2,decodedimage);

            free(decodedimage);
    }

    return 0;
}

static int saveBA81(FILE_HANDLE* handle, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame)
{
    uint16_t x, y;
    uint8_t r, g, b;

    uint32_t fpos = handle->fpos;
	uint32_t row_size_padded = ((width-2)*3 + 3) & (~3); //row size aligned to 4 bytes
	uint32_t fpos_end = fpos + row_size_padded* (height-2);


    // skip first line
    frame += width;

    // don't render top and bottom rows, and left and rightmost columns because of color
	// interpolation

	for (y=1; y<height-1; y++)
	{
			frame++;
			uint8_t rowbuf[row_size_padded];

			//Bitmaps are saved "bottom-up". Seek to the right row.
			if(filesystem_file_seek(handle,fpos_end-row_size_padded*y)!=F_OK) {
				return -1;
			}

			for (x=1; x<width-1; x++, frame++)
			{
				interpolateBayer(width, x, y, frame, &r, &g, &b);
				//bitmaps are saved in 24bit b,g,r format
				rowbuf[(x-1)*3] = b;
				rowbuf[(x-1)*3+1] = g;
				rowbuf[(x-1)*3+2] = r;
			}

			if(filesystem_file_write(handle,rowbuf,row_size_padded)!=F_OK) {
				return -1;
			}

			frame++;
	 }
	return 0;

}
