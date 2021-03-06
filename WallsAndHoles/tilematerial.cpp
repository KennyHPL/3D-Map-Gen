#include "tilematerial.h"

TileMaterial::TileMaterial(QObject *parent)
    : TileMaterial("new Tile Material", getDefaultTexture(), 1, 1, 1, 1, parent)
{
}


TileMaterial::TileMaterial(QString name,
                           SharedImageAndSource texture,
                           float ambient,
                           float diffuse,
                           float specular,
                           float shininess,
                           QObject *parent)
    : QObject(parent)
    , mName(name)
    , mTexture(texture)
    , mAmbient(ambient)
    , mDiffuse(diffuse)
    , mSpecular(specular)
    , mShininess(shininess) {}


QString TileMaterial::name() const { return mName; }
SharedImageAndSource TileMaterial::texture() const { return mTexture; }
float TileMaterial::ambient() const { return mAmbient; }
float TileMaterial::diffuse() const { return mDiffuse; }
float TileMaterial::specular() const { return mSpecular; }
float TileMaterial::shininess() const { return mShininess; }


void TileMaterial::setName(QString name) { mName = name; emit nameChanged(mName); }
void TileMaterial::setTexture(SharedImageAndSource texture) { mTexture = texture; emit textureChanged(); }
void TileMaterial::setAmbient(float ambient) { mAmbient = ambient; emit phongParamsChanged(); }
void TileMaterial::setDiffuse(float diffuse) { mDiffuse = diffuse; emit phongParamsChanged(); }
void TileMaterial::setSpecular(float specular) { mSpecular = specular; emit phongParamsChanged(); }
void TileMaterial::setShininess(float shininess) { mShininess = shininess; emit phongParamsChanged(); }


TileMaterial *TileMaterial::defaultMaterial = nullptr;
TileMaterial *TileMaterial::getDefaultMaterial()
{
    if (defaultMaterial == nullptr)
        defaultMaterial = new TileMaterial("Default", getDefaultTexture(), 1, 1, 1, 1, nullptr);

    return defaultMaterial;
}

TileMaterial *TileMaterial::defaultGroundMaterial = nullptr;
TileMaterial *TileMaterial::getDefaultGroundMaterial()
{
    if (defaultGroundMaterial == nullptr)
        defaultGroundMaterial = new TileMaterial("Default Ground", getDefaultGroundTexture(), 1, 1, 1, 1, nullptr);

    return defaultGroundMaterial;
}


SharedImageAndSource TileMaterial::DefaultTexture = nullptr;
SharedImageAndSource TileMaterial::getDefaultTexture()
{
    if (DefaultTexture.isNull())
        DefaultTexture = ImageAndSource::getSharedImageAndSource("://images/textures/exampleTexture2.png");

    return DefaultTexture;
}

SharedImageAndSource TileMaterial::DefaultGroundTexture = nullptr;
SharedImageAndSource TileMaterial::getDefaultGroundTexture()
{
    if (DefaultGroundTexture.isNull())
        DefaultGroundTexture = ImageAndSource::getSharedImageAndSource("://images/textures/grassTexture.jpg");

    return DefaultGroundTexture;
}
