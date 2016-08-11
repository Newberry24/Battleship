//Enough mips for 16K X 16K which is the minimum required for
//OpenGL 4.x and Higher
#define MAX_TEXTURE_MIPS 14
//Each texture image data structure contains an array of
//MAX_TEXTURE_MIPS of these mipmap structures. The structure represents
//the mipmap data for all slices at that level.
struct vglImageMipData{
    GLsizei width;  //Width of this mipmap level
    GLsizei height; //Height of this mipmap level
    GLsizei depth;  //Depth of mipmap level
    GLsizeiptr mipStride;  //Distance between mip levels in memory.
    GLvoid* data;  //Pointer to data
};

// This is the main image data structure. It contains all
// the parameters needed to place texture data into a texture
// object using OpenGL.

struct vglImageData{
    GLenum target;
    //Texture target (2D, cube map, etc.
    GLenum internalFormat;  //Recommended internal format
    GLenum format;  //Format in memory.
    GLenum type;  //Type in memory (GL_RGB, etc.)
    GLenum swizzle[4];  //Swizzle for RGBA
    GLsizei mipLevels;  //Number of present mipmap levels
    GLsizei slices; //Number of slices (for arrays)
    GLsizeiptr sliceStride;  //Distance between slices of an array texture
    GLsizeiptr totalDataSize; //Total data allocated for texture
    vglImageMipData mip[MAX_TEXTURE_MIPS]; //Actual mipmap data
};
