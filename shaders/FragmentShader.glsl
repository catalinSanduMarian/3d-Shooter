#version 330

// Input
// TODO(student): Get values from vertex shader
	in vec3 frag_color;
// Output
layout(location = 0) out vec4 out_color;

uniform float time;

void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(1);
    //nu am lasat x = frag_color deoarece cubul din stanga ar fi ramas negru 
    vec3 x = frag_color + vec3(cos(time), sin(time), -sin(time));
    x.g = x.g * 10 * cos(time);
  

    out_color = vec4(x, 1);
}
