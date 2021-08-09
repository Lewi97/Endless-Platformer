#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

float random (vec2 st) {
    return fract(sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123);
}

float circleShape(vec2 pos, float radius){
    return step(radius, length(pos - .5));
}

void main() {
    vec2 p = gl_FragCoord.xy / u_resolution.xy;
    
    vec3 color = vec3(.0);

    color = vec3(1.,.3,.0);

    color.g += (sin(p.y-u_time)/2.);

    gl_FragColor = vec4(color, 1.);
}