#include <lcom/lcf.h>
#include "graphics.h"
#include "math.h"
#include "stdio.h"


static uint8_t* video_mem;
static vbe_mode_info_t modeInfo;
static u32_t vram_base;
static u32_t vram_size;
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static uint8_t* test;

int changeMode(uint16_t mode){
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.bx = mode | BIT(14);
  r86.al = 0x02;
  r86.ah = 0x4f;

  if (sys_int86(&r86) != OK) return 1;

  if (r86.al != 0x4f) {
    printf("Function is not supported\n");
    return 1;
  }
  if (r86.ah != 0x00) {
    printf("Function was not sucessfull. Check AH value to know what is wrong\n");
    return 1;
  }
  return 0;
}

int mapMemory(uint16_t mode){

  if (vbe_get_mode_info(mode, &modeInfo)) return 1; // Get Mode info

  struct minix_mem_range mr;
  int r;

  vram_base = modeInfo.PhysBasePtr;
  bits_per_pixel = modeInfo.BitsPerPixel;
  h_res = modeInfo.XResolution;
  v_res = modeInfo.YResolution;
  vram_size = h_res * v_res * ((bits_per_pixel+7) / 8);

  test = malloc(vram_size);

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size; 

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED){
    panic("couldn't map video memory");
    return 1;
  }
  return 0;
}

void* getVideoMem(){
  return video_mem;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color){

  if (x >= h_res || y >=v_res) {
    return 0;
  }


  unsigned bytesPerPixel = (bits_per_pixel + 7) / 8;
  unsigned index = (x + y * h_res) * bytesPerPixel;
  if (memcpy(video_mem + index, &color, bytesPerPixel) == NULL) return 1;
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for (size_t i = 0; i < len; i++){
    if (draw_pixel(x + i, y, color)) return 1;
  }
  return 0;
}


int vg_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  
  for (size_t i = 0; i < len; i++){
    if (draw_pixel(x, y + i, color)) return 1;
  }
  return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

  for (size_t row = 0; row < height; row++){
    if (vg_draw_hline(x, y+row, width, color)) return 1;
  }
  return 0;
}

uint8_t R(uint32_t color){
  return (color >> (modeInfo.RedFieldPosition)) & ~(0xffffff << (modeInfo.RedMaskSize));
}

uint8_t G(uint32_t color){
  return (color >> (modeInfo.GreenFieldPosition)) & ~(0xffffff << (modeInfo.GreenMaskSize));
}

uint8_t B(uint32_t color){
  return (color >> (modeInfo.BlueFieldPosition)) & ~(0xffffff << (modeInfo.BlueMaskSize));
}

int vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step) {

  unsigned Xrect_res = h_res / no_rectangles;
  unsigned Yrect_res = v_res / no_rectangles;

  for (size_t col = 0; col < no_rectangles; col++){

    for (size_t line=0; line<no_rectangles; line++){

      int32_t color = (first + (line * no_rectangles + col) * step) % (1 << bits_per_pixel) ;

      uint8_t red = (R(first) + col * step) % (1 << modeInfo.RedMaskSize);
	    uint8_t green = (G(first) + line * step) % (1 << modeInfo.GreenMaskSize);
	    uint8_t blue = (B(first) + (col + line) * step) % (1 << modeInfo.BlueMaskSize);
      if (modeInfo.RedMaskSize){
        color = (red << modeInfo.RedFieldPosition) | (green << modeInfo.GreenFieldPosition) | (blue << modeInfo.BlueFieldPosition);
      }

      if (vg_draw_rectangle(col * Xrect_res, line * Yrect_res, Xrect_res, Yrect_res, color)) return 1;
    }
  }
  return 0;
}

int (cleanCanvas)(){
  memset(video_mem, 0, vram_size);
  return 0;
}


int drawXPMImage(xpm_image_t img, double x, double y, double angle){
  uint32_t transparentColor = xpm_transparency_color(img.type);

    double cosseno = cos(angle);
    double seno = sin(angle);


  for (size_t col = 0 ; col < img.width; col++){
    for (size_t row = 0 ; row < img.height; row ++){


      unsigned index = (col + row * img.width) * ((bits_per_pixel + 7) / 8);
      uint32_t color = 0;
      memcpy(&color, img.bytes +index, ((bits_per_pixel + 7) / 8));

      if (color != transparentColor){

      double imageX = col - img.width/2.0;
      double imageY = row - img.height/2.0;


      double changedX = cosseno*imageX - seno*imageY;
      double changedY = seno*imageX + cosseno*imageY;


      uint16_t posX = x + changedX;
      uint16_t posY = y + changedY * -1;
      if (draw_pixel(posX , posY, color)) return 1;
      }
    }
  }
  return 0;
}
