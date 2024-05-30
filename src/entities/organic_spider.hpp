#ifndef ORGANIC_SPIDER_HPP
#define ORGANIC_SPIDER_HPP

#include "spider.hpp"


class organic_spider: public spider{
    protected:
        bool imageInitialized = false;
        image_structure im;
        mesh getLegMesh(leg whichLeg, bone whichBone, float &scaling);
        void initializeLegHierarchy(leg whichLeg, vec3 bindPosition) override;
        std::string getTexturePath();
    public:
        organic_spider(){}
        ~organic_spider(){}

        float getBoneLength(leg whichLeg,bone whichBone) override;
};




#endif // ORGANIC_SPIDER_HPP