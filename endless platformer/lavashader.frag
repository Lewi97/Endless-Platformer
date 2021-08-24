#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

void main(){
    vec2 p = gl_FragCoord.xy / u_resolution;
    vec3 color = vec3(0.0);

    for (float i = 0.; i < 8.;i++)
        color.r += step(p.y,(sin((p.x - (i / 2.) +u_time/3.)*5.)*.5)+.5);

    color.r += step(p.y, .8);
    color.g += (sin(p.y)/2.);

    if (color.r < .1 && color.g > .1)
        color.rg = vec2(0.);

    gl_FragColor = vec4(color, 1.0);
}
