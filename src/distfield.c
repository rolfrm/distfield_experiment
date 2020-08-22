#include <iron/full.h>
#define GL_GLEXT_PROTOTYPES
#include <iron/gl.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "main.h"
#include "dist.shader.c"


typedef struct _shader_3d{
  int camera_tform_loc;
  int pos_attr, dv_attr;
  int blit_shader;
  
}shader_3d;

struct _dist3d_context{
  shader_3d shader;
  bool initialized;
  mat4 camera_matrix;
  mat4 perspective_matrix;
  vec4 color;
  texture * current_texture;
  u32 quad, dir_buf;
  
};

dist3d_context * dist3d_context_new(){
  dist3d_context * ctx = alloc0(sizeof(ctx[0]));

  ctx->camera_matrix = mat4_identity();
  ctx->perspective_matrix = mat4_perspective(1.0, 1.0, 0.01, 100.0);
  return ctx;
}

void dist3d_context_initialize(dist3d_context * ctx){
  ctx->initialized = true;
  shader_3d shader;
  shader.blit_shader = gl_shader_compile2((char *)src_dist_shader_vs,src_dist_shader_vs_len, (char *)src_dist_shader_fs,src_dist_shader_fs_len);
  shader.pos_attr = 0;
  shader.dv_attr = 1;
  shader.camera_tform_loc = glGetUniformLocation(shader.blit_shader, "camera_tform");
  glUseProgram(shader.blit_shader);
  glEnableVertexAttribArray(shader.pos_attr);
  glEnableVertexAttribArray(shader.dv_attr);
  ctx->shader = shader;

  float pts[] = {-1, -1, 0,
		 1, -1, 0,
		 -1, 1, 0,
		 1, 1, 0};
  glGenBuffers(1, &ctx->quad);
  glBindBuffer(GL_ARRAY_BUFFER, ctx->quad);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

  float dv[] = {-1, -1, -1,
		1, -1, -1,
		-1, 1, -1,
		1, 1, -1};
  glGenBuffers(1, &ctx->dir_buf);
  glBindBuffer(GL_ARRAY_BUFFER, ctx->dir_buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(dv), dv, GL_STATIC_DRAW);
  

  
}

void dist3d_context_load(dist3d_context * ctx)
{
  if(false == ctx->initialized)
    dist3d_context_initialize(ctx);

  glUseProgram(ctx->shader.blit_shader);
  glEnableVertexAttribArray(0);
}

struct _dist3d_polygon{
  void * data;
  size_t length;
  u32 dimensions;
  u32 buffer;
  bool changed;
  vertex_buffer_type type;
};

dist3d_polygon * dist3d_polygon_new(){
  dist3d_polygon * pol = alloc0(sizeof(pol[0]));
  pol->type = VERTEX_BUFFER_ARRAY;
  return pol;
}

void dist3d_polygon_load_data(dist3d_polygon * polygon, void * data, size_t size){
  polygon->data = iron_clone(data, size);
  polygon->length = size;
  polygon->changed = true;
}

void dist3d_polygon_configure(dist3d_polygon * polygon, int dimensions){
  polygon->dimensions = dimensions;
}

void dist3d_polygon_update(dist3d_polygon * polygon){
  if(polygon->changed){
    polygon->changed = false;
    if(polygon->buffer == 0){
      glGenBuffers(1, &polygon->buffer);
     }
    if(polygon->type == VERTEX_BUFFER_ARRAY){
      glBindBuffer(GL_ARRAY_BUFFER, polygon->buffer);
      glBufferData(GL_ARRAY_BUFFER, polygon->length, polygon->data, GL_STATIC_DRAW);
    }
    else if(polygon->type == VERTEX_BUFFER_ELEMENTS){
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygon->buffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, polygon->length, polygon->data, GL_STATIC_DRAW);
    }
    free(polygon->data);
    polygon->data = NULL;
  }
}
struct _texture_handle {
  GLuint tex;
};
texture * get_default_tex();

void dist3d_polygon_blit(dist3d_context * ctx){
  var shader = ctx->shader;
  
  glBindBuffer(GL_ARRAY_BUFFER, ctx->quad);
  glVertexAttribPointer(shader.pos_attr, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, ctx->dir_buf);
  glVertexAttribPointer(shader.dv_attr, 3, GL_FLOAT, GL_FALSE, 0, 0);
  mat4 tform = mat4_mul(ctx->perspective_matrix, ctx->camera_matrix);
  
  glUniformMatrix4fv(shader.camera_tform_loc, 1, false, &tform.data[0][0]);
  glDrawArrays(GL_TRIANGLE_STRIP,0, 4);
}

/*void dist3d_polygon_blit2(dist3d_context * ctx, vertex_buffer ** polygons, u32 count){
  if(count == 0)
    return;

  var tex = ctx->current_texture != NULL ? ctx->current_texture : get_default_tex();
  glUniform1i(ctx->shader.textured_loc, 1);
  glBindTexture(GL_TEXTURE_2D, tex->handle->tex);
  int elements_index = -1;
  for(u32 i = 0; i < count; i++){
    dist3d_polygon_update(polygons[i]);
    if(polygons[i]->type == VERTEX_BUFFER_ELEMENTS){
      elements_index = (int)i; 
    }
  }

  if(elements_index != -1)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygons[elements_index]->buffer);
  
  u32 j = 0;
  for(u32 i = 0; i < count; i++){
    if((int)i == elements_index)
      continue;

    glBindBuffer(GL_ARRAY_BUFFER, polygons[j]->buffer);
    glVertexAttribPointer(j, polygons[j]->dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    j += 1;    
  }

  var shader = ctx->shader;
  var c = ctx->color;
  glUniform4f(shader.color_loc, c.x,c.y,c.z,c.w);
  glUniformMatrix4fv(shader.camera_tform_loc, 1, false, &ctx->matrix.m00);
  if(elements_index != -1){
    glDrawElements(GL_TRIANGLE_STRIP, polygons[elements_index]->length / (polygons[elements_index]->dimensions * 4), GL_UNSIGNED_BYTE, 0);

    // draw elements here
  }else{
    glDrawArrays(GL_TRIANGLE_STRIP,0, polygons[0]->length / (polygons[0]->dimensions * 4));
  }
  int err =  glGetError();
  if(err != 0)
    printf("eRR2: %i\n", err);
    }*/

