#include "maps/GameMap.h"

GameMap::GameMap(const std::string &tilesetFilename, const std::vector<std::vector<int>> &mapData)
    : tilesetFilename(tilesetFilename)
    , mapData(mapData)
{
    //
}

GameMap *GameMap::create(const std::string &tilesetFilename, const std::vector<std::vector<int>> &mapData)
{
    GameMap *pRet = new (std::nothrow) GameMap(tilesetFilename, mapData);

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GameMap::init()
{
    if (!Node::init())
    {
        return false;
    }

    spriteBatchNode = SpriteBatchNode::create(tilesetFilename);
    addChild(spriteBatchNode);
    buildMap();

    return true;
}

void GameMap::buildMap()
{
    const int tileSize = 32;
    const Size screenSize = Director::getInstance()->getVisibleSize();

    for (int row = 0; row < mapData.size(); ++row)
    {
        for (int col = 0; col < mapData[row].size(); ++col)
        {
            int tileIndex = mapData[row][col];

            float x = col * tileSize + tileSize / 2;
            float y = screenSize.height - (row * tileSize + tileSize / 2);

            auto tile = Sprite::createWithTexture(spriteBatchNode->getTexture(), Rect(tileIndex * tileSize, 0, tileSize, tileSize));
            tile->setPosition(x, y);

            spriteBatchNode->addChild(tile);
        }
    }
}
