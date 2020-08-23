#version 430
layout(local_size_x = 16, local_size_y = 16) in;
layout(binding = 0, rgba8) uniform image2D tex;
layout(binding = 1) buffer Dir{
  vec3 Directionsp[];
};
void main(){

  vec2 center = vec2(64, 64);
  float radius = 4;
  
  vec3 grp = gl_LocalInvocationID + gl_WorkGroupSize * gl_WorkGroupID;

  float d = 1 - (length(grp.xy - center) - radius);
  
  vec4 color = vec4(d,d,d,1);
  if(d < 0.0)
    color = vec4(0,0,0,1);

  ivec2 storePos = ivec2(grp.xy);
  imageStore(tex, storePos, color);
  Directionsp[0] = vec3(1,0,1.5);
}
