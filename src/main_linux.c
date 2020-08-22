#include<iron/full.h>
#include <iron/gl.h>
#include <ctype.h>
#include "main.h"
#include <stdio.h>
#include <unistd.h>
int linux_main(void (* on_load)(distfield_context *ctx), void (* on_save)(distfield_context *ctx));

void * load_data = NULL; 
size_t load_data_size;
distfield_context * current_context;

void do_load(distfield_context * ctx){
  current_context = ctx;
  if(load_data != NULL){
    distfield_save save = {0};
    //decode_from_data(&save, load_data, load_data_size);
    distfield_load_data(ctx, &save);
    dealloc(load_data);
    load_data = NULL;
  }
}

char * out_file = NULL;

void do_save(distfield_context * ctx){
  if(out_file == NULL) return;
  distfield_save save;
  distfield_save_data(ctx, &save);
  size_t len;
  //var stringv = encode_as_data(&save, &len);
  //write_buffer_to_file(stringv, len, out_file);
}

int main(int argc, char ** argv){
  
  int r =  linux_main(do_load, do_save);

  distfield_save save;
  distfield_save_data(current_context, &save);
  size_t len;
  //var stringv = encode_as_data(&save, &len);
  //write_buffer_to_file(stringv, len, out_file);
  return r;
}

static bool startswith(const char *pre, const char *str)
{
  size_t lenpre = strlen(pre),
    lenstr = strlen(str);
  return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

static char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

static char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

static char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}
