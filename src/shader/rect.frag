#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main(){
    FragColor = texture(ourTexture, TexCoords);
   // FragColor=vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
