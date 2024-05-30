#ifndef ORGANIC_SPIDER_HPP
#define ORGANIC_SPIDER_HPP

#include "spider.hpp"


class organic_spider: public spider{
    protected:
        static bool textureInitialized;
        static opengl_texture_image_structure texture;
        mesh getLegMesh(leg whichLeg, bone whichBone, float &scaling);
        void initializeLegHierarchy(leg whichLeg, vec3 bindPosition) override;
        std::string getTexturePath();
    public:
        organic_spider(){}
        ~organic_spider(){}

        float getBoneLength(leg whichLeg,bone whichBone) override;
};




#endif // ORGANIC_SPIDER_HPP