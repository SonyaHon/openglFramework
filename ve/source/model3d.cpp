#include "../headers/model3d.h"
namespace ve {
bool Model3D::loadAssImp(
    const char *              path,
    std::vector<unsigned int> & indices,
    std::vector<glm::vec3>    & vertices,
    std::vector<glm::vec2>    & uvs,
    std::vector<glm::vec3>    & normals
) {
    Assimp::Importer importer;

    const aiScene * scene = importer.ReadFile(path, 0 /*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
    if (!scene) {
        fprintf(stderr, importer.GetErrorString());
        getchar();
        return false;
    }
    const aiMesh * mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

    // Fill vertices positions
    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D pos = mesh->mVertices[i];
        vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
    }

    // Fill vertices texture coordinates
    uvs.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
        uvs.push_back(glm::vec2(UVW.x, UVW.y));
    }

    // Fill vertices normals
    normals.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D n = mesh->mNormals[i];
        normals.push_back(glm::vec3(n.x, n.y, n.z));
    }


    // Fill face indices
    indices.reserve(3 * mesh->mNumFaces);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        // Assume the model has only triangles.
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
    }

    // The "scene" pointer will be deleted automatically by "importer"
} // loadAssImp

GLuint Model3D::png_texture_load(const char * file_name, int * width, int * height) {
    png_byte header[8];

    FILE * fp = fopen(file_name, "rb");

    if (fp == 0) {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
      NULL, NULL, NULL);

    if (width) *width = temp_width; if (height) *height = temp_height;
    // printf("%s: %lux%lu %d\n", file_name, temp_width, temp_height, color_type);

    if (bit_depth != 8) {
        fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name, bit_depth);
        return 0;
    }

    GLint format;
    switch (color_type) {
        case PNG_COLOR_TYPE_RGB:
            format = GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            format = GL_RGBA;
            break;
        default:
            fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
            return 0;
    }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes - 1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data = (png_byte *) malloc(rowbytes * temp_height * sizeof(png_byte) + 15);
    if (image_data == NULL) {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_byte ** row_pointers = (png_byte **) malloc(temp_height * sizeof(png_byte *));
    if (row_pointers == NULL) {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < temp_height; i++) {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);


    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
} // png_texture_load

void Model3D::loadTexture(std::string filename, int width, int height) {
    texture = png_texture_load(filename.c_str(), &width, &height);
}

void Model3D::draw(glm::mat4 ViewMatrix, glm::mat4 PROJECTION_MATRIX) {
    glm::mat4 MVP = PROJECTION_MATRIX * ViewMatrix * ModelMatrix;

    GLuint MatrixID  = glGetUniformLocation(shProg.getProg(), "MVP");
    GLuint TextureID = glGetUniformLocation(shProg.getProg(), "textureSampler");
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,         // Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
        3,         // Размер
        GL_FLOAT,  // Тип
        GL_FALSE,  // Указывает, что значения не нормализованы
        0,         // Шаг
        (void *) 0 // Смещение массива в буфере
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        1,         // атрибут. Нет особых причин писать 1, главное, чтобы совпадало со значением в шейдере.
        2,         // размер
        GL_FLOAT,  // тип
        GL_FALSE,  // нормализировано ли?
        0,         // шаг
        (void *) 0 // смещение в буфере
    );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glUseProgram(shProg.getProg());
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1i(TextureID, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glDrawElements(
        GL_TRIANGLES,
        indices.size(),
        GL_UNSIGNED_INT,
        (void *) 0
    );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
} // Model3D::draw

void Model3D::translate(float xt, float yt, float zt) {
    ModelMatrix *= glm::translate(glm::mat4(), glm::vec3(xt, yt, zt));
    posX        += xt;
    posZ        += zt;
    posY        += yt;
}

void Model3D::scale(float xs, float ys, float zs) {
    ModelMatrix *= glm::scale(glm::mat4(), glm::vec3(xs, ys, zs));
    scaX        += xs;
    scaY        += ys;
    scaZ        += zs;
}

void Model3D::scale(float alls) {
    ModelMatrix *= glm::scale(glm::mat4(), glm::vec3(alls, alls, alls));
    scaX        += alls;
    scaY        += alls;
    scaZ        += alls;
}

void Model3D::rotate(float angle, float xr, float yr, float zr) {
    ModelMatrix *= glm::rotate(glm::mat4(), angle, glm::vec3(xr, yr, zr));
    rotX        += xr;
    rotY        += yr;
    rotZ        += zr;
}
}
