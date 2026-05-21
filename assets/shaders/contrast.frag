#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float contrast;

out vec4 finalColor;

void main() {
    vec4 col = texture(texture0, fragTexCoord) * colDiffuse * fragColor;
    col.rgb = clamp((col.rgb - 0.5) * contrast + 0.5, 0.0, 1.0);
    finalColor = col;
}
