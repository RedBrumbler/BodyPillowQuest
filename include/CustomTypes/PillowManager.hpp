#pragma once
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "constellation.hpp"
#include <vector>
#include <string>

DECLARE_CLASS_CODEGEN(MenuPillow, PillowManager, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(static void, OnMenuSceneActivate);
    DECLARE_METHOD(static void, OnMenuSceneDeActivate);
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, RemoveLastPillow);


    public:
        std::vector<MenuPillow::Constellation> constellations = {};
        static std::vector<MenuPillow::Constellation>* GetConstellations();
        UnityEngine::Transform* SpawnPile(pillowparams param);
        bool useSaved = false;
        bool shouldRandomizeOnReEnable = false;
        Constellation savedConstellation;
        
        void SetActiveConstellation(Constellation constellation);
        void SetActiveConstellation(std::string name);
        void LoadConstellations();
        static void RandomizeTextures();
        
        static void OnModEnable();
        static void OnModDisable();
        
    REGISTER_FUNCTION(PillowManager,
        REGISTER_METHOD(OnMenuSceneActivate);
        REGISTER_METHOD(OnMenuSceneDeActivate);
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(RemoveLastPillow);
    )
)