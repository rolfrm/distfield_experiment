#version 100
precision highp float;

uniform mat4 camera_tform;
attribute vec3 pos;
attribute vec3 dv;
varying vec3 dir;

void main() 
{
    vec4 camera_pos = camera_tform * vec4(0,0,0,1);
    vec4 light_pos = camera_tform * vec4(dv, 1);
    dir = light_pos.xyz / light_pos.w - camera_pos.xyz - camera_pos.w;
    
    gl_Position = vec4(pos, 1);
}