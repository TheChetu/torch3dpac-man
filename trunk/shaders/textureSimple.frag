uniform sampler2D tex;

void main()
{
	vec4 color = texture2D(tex,gl_TexCoord[0].st);
	vec4 color = texture2D(tex,gl_TexCoord[1].st);
	vec4 color = texture2D(tex,gl_TexCoord[2].st);
	gl_FragColor = color;
}
