#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <iron/linmath.h>
#include <iron/types.h>
#include <iron/log.h>
#include <iron/math.h>
#include <iron/hashtable.h>
#include <iron/utils.h>
#include <iron/time.h>
#include <iron/gl.h>
#include <iron/mem.h>
#include <iron/process.h>
#include "main.h"
#include <GL/gl.h>

#include "u32_u32_table.h"
#include "u32_u32_table.c"
void initialize_graphics();
bool platform_load_clipboard_image(gl_window * win, image * img);


struct _distfield_context {
  bool should_close;
  gl_window * win;
  touch_event touch_events[TOUCH_EVENT_MAX];

  touch_touch current_touches[10];
  bool active_touches[10];
  
  bool touch_dragging;
  int two_touch_grace;
  int req_width, req_height;

  bool touch_noticed;
  int touch_counter;
  bool no_events;
  bool touch_paint_enable;
  void (* on_save)(distfield_context * ctx);
  int pos_bits;
  blit3d_polygon * quad;
  dist3d_context * dist;

  
};

void distfield_load_data(distfield_context * distfield, distfield_save * data){
}


void distfield_save_data(distfield_context * distfield, distfield_save * save){
}


distfield_context * distfield_context_new(gl_window * win){
  distfield_context ctx = {0};
  ctx.win = win;
  ctx.dist = dist3d_context_new();
  return iron_clone(&ctx, sizeof(ctx));
}

void distfield_push_touch_event(distfield_context * distfield, touch_event * evt){
  for(int i = 0; i < TOUCH_EVENT_MAX; i++){
    if(distfield->touch_events[i].num_touches == 0){
      distfield->touch_events[i] = *evt;
      break;
    }
  }
}

touch_event distfield_pop_touch_event(distfield_context * distfield){
  for(int i = 0; i < TOUCH_EVENT_MAX; i++){
    var evt = distfield->touch_events[i];
    if(evt.num_touches != 0){
      distfield->touch_events[i] = (touch_event){0};
      return evt;
    }
  }
  return (touch_event){0};
}


void distfield_save_reload(distfield_context * ctx){
}


u64 last_render_time = 0;
void render_update(distfield_context * ctx){
  gl_window_make_current(ctx->win);
  blit_begin(BLIT_MODE_UNIT);
  blit_translate(-1,-1);
  blit_scale(2,2);
  blit_rectangle2(0,0,0,1);
  dist3d_context_load(ctx->dist);
  dist3d_polygon_blit(ctx->dist);
}


int linux_main(void (* on_load)(distfield_context * ctx), void (* on_save)(distfield_context * ctx)){
  iron_gl_debug = true;

  iron_gl_backend = IRON_GL_BACKEND_GLFW;
  gl_window * win = gl_window_open(512,512);
  gl_window_make_current(win);

  
  distfield_context * ctx = distfield_context_new(win);

  ctx->on_save = on_save;
  if(on_load) on_load(ctx);
  while(ctx->should_close == false){
    render_update(ctx);
    gl_window_swap(win);
  }
  
  return 0;
}

