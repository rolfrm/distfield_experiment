#version 430
layout(local_size_x = 16, local_size_y = 16) in;
layout(binding = 0, rgba32f) uniform image2D tex;
layout(binding = 1) buffer Dir{
  vec3 Directionsp[];
};
void main(){
  ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
  imageStore(tex, storePos, vec4(1.0,0.0,1.0,1.0));
  Directionsp[0] = vec3(1,0,1.5);
}
