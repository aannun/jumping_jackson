#include "engine2D.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include "stb_image.h"

static GLuint compile_shader(GLenum shader_type, const char *filename)
{
    SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
    if (!rw)
    {
        SDL_Log("unable to open file");
        exit(1);
    }

    size_t file_len = SDL_RWsize(rw);
    char *source = SDL_malloc(file_len + 1);
    if (!source)
    {
        SDL_Log("unable to allocate memory");
        exit(1);
    }

    if (SDL_RWread(rw, source, 1, file_len) != file_len)
    {
        SDL_Log("unable to read file");
        exit(1);
    }

    source[file_len] = 0;

    SDL_RWclose(rw);

    GLuint shader = glCreateShader(shader_type);
    // assign the shader code
    glShaderSource(shader, 1, (const GLchar **)&source, (GLint *)&file_len);
    // compile the shader
    glCompileShader(shader);

    SDL_free(source);

    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE)
    {
        GLint log_size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
        char *error_log = SDL_malloc(log_size + 1);
        if (!error_log)
        {
            SDL_Log("unable to allocate memory for log");
            exit(1);
        }
        glGetShaderInfoLog(shader, log_size, &log_size, error_log);
        error_log[log_size] = 0;
        SDL_Log("shader compile error: %s", error_log);
        SDL_free(error_log);
        exit(1);
    }

    return shader;
}

static GLuint tex_loaded(context_2D_t *engine, unsigned int tex_id)
{
    int i = 0;
    for(i = 0; i < engine->loaded_textures_length; i++)
    {
        if(engine->sprites[i]->texture == tex_id)
            return engine->sprites[i]->texture;
    }
    return -1;
} 

static unsigned int get_hash(const char *data, size_t len)
{
    unsigned int hash = 5381;
    unsigned int i = 0;
    char* ptr = data;

    for(i = 0; i < len; i++)
    {   
        hash += hash << 5;
        hash += *ptr;
        ptr++;
    }   

    return hash;
}

static GLuint load_texture(context_2D_t *engine, sprite_t *sprite, const char *filename, size_t file_len)
{
    unsigned int hash = get_hash(filename, file_len);
    GLuint loaded = tex_loaded(engine, hash);
    
    if(loaded != -1)
        return loaded;

    int w, h, comp;
    unsigned char *pixels = stbi_load(filename, &w, &h, &comp, 4);
    if(!pixels)
        return -1;

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    engine->loaded_textures_length++;
    return tex;
}

GLuint load_mesh()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    int vertices_len = 18;
    int uvs_len = 12;

    float vertices[] = {1,0,0, 0,0,0, 0,1,0, 0,1,0, 1,1,0, 1,0,0};
    float uvs[] = {1,0, 0,0, 0,1, 0,1, 1,1, 1,0};

    GLuint vbo[2];
    glGenBuffers(2, &vbo[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices_len * sizeof(float), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, uvs_len * sizeof(float), uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    return vao;
}


int init_engine2D(context_2D_t *engine_instance, int sprites_max_len)
{   
    memset(engine_instance, 0, sizeof(context_2D_t));
    engine_instance->sprites_max_len = sprites_max_len;
   
    GLuint program = glCreateProgram();

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, "vertex.glsl");
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    // attach the sahders to the program
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    // linke the program (now is ready)
    glLinkProgram(program);

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    engine_instance->program = program;

    size_t size = sizeof(sprite_t)*sprites_max_len;
    engine_instance->sprites = malloc(size);
    if(!engine_instance->sprites)
        return 0;
    memset(engine_instance->sprites, 0, size);

    engine_instance->mesh_len = 18;
    engine_instance->mesh = load_mesh();

    engine_instance->engine_2D_pos_uniform = glGetUniformLocation(program, "position");
    engine_instance->engine_2D_rot_uniform = glGetUniformLocation(program, "rotation");
    engine_instance->engine_2D_scale_uniform = glGetUniformLocation(program, "scale");

    return 1;
}

int init_sprite(context_2D_t *engine_instance, sprite_t *sprite, const char *filename, size_t file_len, float posX, float posY, float posZ)
{
    if(engine_instance->sprites_len>=engine_instance->sprites_max_len)
        return 0;

    memset(sprite, 0, sizeof(sprite_t));
    sprite->posX = posX;
    sprite->posY = posY;
    sprite->posZ = posZ;
    sprite->scale = 1;
    
    sprite->texture = load_texture(engine_instance, sprite, filename, file_len);
    engine_instance->sprites[engine_instance->sprites_len] = sprite;
    engine_instance->sprites_len++;

    return 1;
}

void engine_2D_draw_sprites(context_2D_t *engine_instance)
{
    int i = 0;
    int len = engine_instance->sprites_len;
    int mesh_len = engine_instance->mesh_len / 3;

    glUseProgram(engine_instance->program);
    glBindVertexArray(engine_instance->mesh);

    for(i = 0; i < len; i++)
    {
        glUniform1f(engine_instance->engine_2D_scale_uniform, engine_instance->sprites[i]->scale);
        glUniform1f(engine_instance->engine_2D_rot_uniform, engine_instance->sprites[i]->rotation);
        glUniform3f(engine_instance->engine_2D_pos_uniform, engine_instance->sprites[i]->posX, engine_instance->sprites[i]->posY, engine_instance->sprites[i]->posZ);

        glBindTexture(GL_TEXTURE_2D, engine_instance->sprites[i]->texture);
        glDrawArrays(GL_TRIANGLES, 0, mesh_len);
    }
}