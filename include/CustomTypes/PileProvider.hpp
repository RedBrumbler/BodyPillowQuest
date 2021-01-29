#pragma once
#include "bs-utils/shared/AssetBundle.hpp"
#include "CustomTypes/Pile.hpp"
#include "UnityEngine/GameObject.hpp"

#include "custom-types/shared/register.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(MenuPillow, PileProvider, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(static void, LoadBundle, bool alsoLoadAssets = false);
    DECLARE_METHOD(static void, LoadAssets);
    DECLARE_METHOD(static Pile*, GetPile, int index = -1);
    DECLARE_METHOD(void, OnAssetLoadComplete, UnityEngine::GameObject* pillowContainer);
    DECLARE_METHOD(static void, ManualUpdate);
    private:
        static inline UnityEngine::GameObject* container = nullptr;

        static inline bs_utils::AssetBundle* bundle = nullptr;
        static inline std::vector<Pile*> piles = {};

        static inline bool bundleLoading = false;
        static inline bool loadingAssets = false;
        static inline bool doLoadAssets = false;

        static inline int childIndex = 0;
        static inline int childCount = 0;

    REGISTER_FUNCTION(PileProvider,
        REGISTER_METHOD(LoadBundle);
        REGISTER_METHOD(LoadAssets);
        REGISTER_METHOD(GetPile);
        REGISTER_METHOD(OnAssetLoadComplete);
    )
)

/*
namespace MenuPillow
{
    class PillowProvider
    {
        public:
            void LoadBundle(bool alsoLoadAssets = false);
            void LoadAssets();
            Pillow* GetPillow(int index = -1);

        private:
            void OnAssetLoadComplete(UnityEngine::GameObject* pillowContainer);
            
            UnityEngine::GameObject* container;

            bs_utils::AssetBundle* bundle;
            std::vector<Pillow*> pillows;

            bool bundleLoading = false;
            bool loadingAssets = false;

            int childIndex = 0;
            int childCount = 0;
    };
}
*/