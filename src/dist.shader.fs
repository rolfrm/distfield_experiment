#version 100
precision mediump float;
varying vec3 dir;

void main() 
{

    vec3 center = vec3(0,0,5);
    float r = 1.9;

    vec3 d = normalize(dir);
    vec3 pos = dir;

    for(int i = 0; i < 5; i++){
       float d = length(pos - center) - r;
       if(d <= 0.01) break;
       pos = pos + d * dir;
    }
    float d2 = length(pos - dir) * 0.1;
    //    if(d2 < 0.5) discard;

    //    if(d.y > 0.0) discard;
    //if(d.x > 0.0) discard;
    //if(d.y < 0.5) discard;
    //if(d.x < 0.5) discard;
    gl_FragColor = vec4(d2,d2,d2,1);

} 
