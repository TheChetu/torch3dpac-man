uniform sampler2D tex;
vec4 color;

void main()
{
	gl_FragColor = color;
	vec4 color = texture2D(tex,gl_TexCoord[0].st);
	gl_FragColor = color;
	vec4 color1 = texture2D(tex,gl_TexCoord[1].st);
	gl_FragColor = color;
	vec4 color2 = texture2D(tex,gl_TexCoord[2].st);
	gl_FragColor = color;
}
