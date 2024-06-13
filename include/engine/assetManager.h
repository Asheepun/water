//#ifndef ENGINE_ASSETMANAGER_H
//#define ENGINE_ASSETMANAGER_H

#ifdef __ANDROID__

#include <android/asset_manager.h>

typedef AAssetManager AssetManager;

#else

typedef void AssetManager;

#endif
//#endif
