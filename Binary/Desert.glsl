#if defined(VS_FULL_TRIANGLE)

out gl_PerVertex
{
    vec4 gl_Position;
};

void main(void)
{
    vec2 verts[] = { vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0) };
    gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);
}

#elif defined(FS_DESERT)

layout(location = 0) out vec4 s_OutColor;

void main(void)
{
    s_OutColor = vec4(0.25, 0.0, 0.5, 1.0);
}

#endif
