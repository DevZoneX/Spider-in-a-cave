#ifndef ORGANIC_SPIDER_HPP
#define ORGANIC_SPIDER_HPP

#include "spider.hpp"
#include "../map/cave_mesh.hpp"


class organic_spider: public spider{
    protected:
        static bool textureInitialized;
        static opengl_texture_image_structure texture;
        static opengl_texture_image_structure normal_texture;
        static material_mesh_drawable_phong material;
        mesh getLegMesh(leg whichLeg, bone whichBone, float &scaling);
        void initializeLegHierarchy(leg whichLeg, vec3 bindPosition) override;
        std::string getTexturePath();
    public:
        organic_spider(){}
        ~organic_spider(){}


        void initialize() override;
        float getBoneLength(leg whichLeg,bone whichBone) override;
        
};




#endif // ORGANIC_SPIDER_HPP