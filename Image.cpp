#include "Image.h"

Image3::Image3()
{
    glGenTextures(4, &texId);
    for (int i = 0; i < 4; i++)
    {
        glBindTexture(GL_TEXTURE_2D, texId + i);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

}

void Image3::refresh()
{
    if (mat.rows != 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, (int)mat.step / (int)mat.elemSize());
        glBindTexture(GL_TEXTURE_2D, texId);
        if (mat.channels() == 1)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.ptr());
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, mat.ptr());

            glBindTexture(GL_TEXTURE_2D, texId + 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr());

            glBindTexture(GL_TEXTURE_2D, texId + 2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr() + 1);

            glBindTexture(GL_TEXTURE_2D, texId + 3);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr() + 2);
        }

    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            glBindTexture(GL_TEXTURE_2D, texId + i);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        }
    }
}

Image1::Image1()
{
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Image1::refresh()
{
    glBindTexture(GL_TEXTURE_2D, texId);
    if (mat.rows != 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, (int)mat.step / (int)mat.elemSize());

        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.ptr());
    }
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}
