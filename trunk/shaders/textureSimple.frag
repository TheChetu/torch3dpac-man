uniform sampler2D tex;

varying vec4 ambientGlobal;

void main()
{

	vec4 color = ambientGlobal;
	float att;
	
	color += texture2D(tex,gl_TexCoord[0].st);

	gl_FragColor = color;

}