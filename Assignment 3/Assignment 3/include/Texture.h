#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glew.h>

class Texture {
public:
    unsigned int id;
    std::string type;
    std::string path;

    Texture(const char* path, const std::string& directory, const std::string& type);

    void bind(unsigned int unit) const;

private:
    void loadTexture(const char* path, const std::string& directory);
};

#endif // TEXTURE_H
