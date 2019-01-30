#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 uv;

uniform vec3 position;
uniform float rotation;
uniform float scale;
uniform vec2 pivot;

out vec2 uvs;

vec3 rotate_z(vec3 v, float rot)
{
    vec3 vo;
    vo.x = cos(rot) * v.x - sin(rot) * v.y;
    vo.y = sin(rot) * v.x + cos(rot) * v.y;
    vo.z = v.z;
    return vo;
}

float linear_convert(float value, float old_min, float old_max, float new_min, float new_max)
{
    float gradient = (value - old_min) / (old_max - old_min);
    return new_min + gradient * (new_max - new_min);
}

vec3 scale_vec(vec3 vec, float scale)
{   
    vec3 v;
    v.x = linear_convert(vec.x, -scale, scale, -1, 1);
    v.y = linear_convert(vec.y, -scale, scale, -1, 1);
    v.z = linear_convert(vec.z, -scale, scale, -1, 1);
    return v;
}

void main()
{
    vec3 world_position = rotate_z(vec3(vertex.x, 1-vertex.y, vertex.z) - vec3(pivot.xy,0), radians(rotation));
    float scale_ = 1/scale;
    world_position.x = linear_convert(world_position.x, -scale_, scale_, -1, 1) + position.x;
    world_position.y = linear_convert(world_position.y, -scale_, scale_, -1, 1) + position.y;
    world_position.z = linear_convert(world_position.z + position.z, 0, 100, -1, 1);

    gl_Position = vec4(world_position, 1);
    
    uvs = uv;
}

/*void main()
{
    uvs = uv;

    vec2 window_size = vec2(1024, 1024);
    vec2 pixel_unit = (1 / window_size) * 2;

    vec2 quad_pos = vec2(vertex.x, vertex.y);
    vec2 scaled_pos = (quad_pos / window_size) * 2;
    vec2 final_pos = scaled_pos + (pixel_unit * vec2(position.x, position.y));

    //final_pos -= 1;

    final_pos.y *= -1;

    gl_Position = vec4(final_pos, vertex.z, 1);
}*/