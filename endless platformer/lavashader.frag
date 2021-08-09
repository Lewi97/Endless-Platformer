#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

void main() {
    vec2 p = gl_FragCoord.xy / u_resolution.xy;

    vec3 color = vec3(1.,.3,.0);

    color.g += (sin(p.y-u_time)/2.);

    gl_FragColor = vec4(color, 1.);
}
