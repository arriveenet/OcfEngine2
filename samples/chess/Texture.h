#pragma once
#include <ocf/renderer/Texture.h>
#include <stdint.h>

constexpr int TXSX = 256;
constexpr int TXSY = 256;

void genarateTexture();

ocf::Texture* getWhiteSquareTexture();

ocf::Texture* getBlackSquareTexture();

ocf::Texture* getWoodTexture();
