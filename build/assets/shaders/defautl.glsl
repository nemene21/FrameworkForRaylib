#version 330

in vec4 fragColor; // Color coming from the vertex shader
out vec4 finalColor;

void main() {
    finalColor = fragColor; // Output the color
}