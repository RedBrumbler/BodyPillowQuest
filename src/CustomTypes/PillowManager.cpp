#include "CustomTypes/PillowManager.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "CustomTypes/PileProvider.hpp"
#include "config.hpp"
#include "TexturePool.hpp"
#include "static-defines.hpp"

DEFINE_CLASS(MenuPillow::PillowManager);

using namespace UnityEngine;

extern config_t config;
extern Logger& getLogger();

namespace MenuPillow
{
    void PillowManager::OnMenuSceneActivate()
    {
        PillowManager* manager = Object::FindObjectOfType<PillowManager*>();
        if (!manager)
        {
            GameObject* newObj = GameObject::New_ctor();
            Object::DontDestroyOnLoad(newObj);
            manager = newObj->AddComponent<PillowManager*>();
        }

        manager->get_gameObject()->SetActive(true);
    }

    void PillowManager::OnMenuSceneDeActivate()
    {
        PillowManager* manager = Object::FindObjectOfType<PillowManager*>();
        if (!manager) return;
        manager->get_gameObject()->SetActive(true);
    }

    void PillowManager::Awake()
    {
        TexturePool::LoadAllTextures();
        LoadConstellations();
        SetActiveConstellation(config.lastActiveConstellation);
    }

    void PillowManager::LoadConstellations()
    {
        this->constellations = Constellation::ConstellationsFromFolderPath(CONSTELLATIONPATH);
    }

    Transform* PillowManager::SpawnPile(pillowparams param)
    {
        Pile* pile = PileProvider::GetPile(param.type);
        if (!pile) return nullptr;
        Transform* pileTransform = pile->get_transform();
        pileTransform->set_localPosition(param.position);
        pileTransform->set_localEulerAngles(param.rotation);
        return pileTransform;
    }

    void PillowManager::RemoveLastPillow()
    {
        int childCount = get_transform()->get_childCount();

        Object::Destroy(get_transform()->GetChild(childCount - 1));
    }

    void PillowManager::SetActiveConstellation(std::string name)
    {
        // find the constellation with this name, and then make that the active one
        Constellation* constellation = nullptr;
        if (name == "") name = "default"; 

        for (auto& it : constellations)
        {
            constellation = &it; 
            if (it.get_name().find(name) != std::string::npos) break; 
        }
        if (!constellation) return;
        SetActiveConstellation(*constellation);
    }

    void PillowManager::SetActiveConstellation(Constellation constellation)
    {
        if (!config.enabled)
        {
            savedConstellation = constellation;
            useSaved = true;
            return;
        }

        // remove all old piles
        Array<Pile*>* piles = GetComponentsInChildren<Pile*>();
        if (piles)
        {
            for (int i = 0; i < piles->Length(); i++)
            {
                Pile* pile = piles->values[i];
                if (!pile) continue;
                Object::Destroy(pile->get_gameObject());
            }
        }

        // spawn all new piles
        const std::vector<pillowparams>& params = constellation.get_params();

        for (auto param : params)
        {
            Transform* pile = SpawnPile(param);
            if (pile) pile->SetParent(get_transform());
        }
    }

    std::vector<MenuPillow::Constellation>* PillowManager::GetConstellations()
    {
        PillowManager* manager = Object::FindObjectOfType<PillowManager*>();
        if (manager) return &manager->constellations;
        return nullptr;
    }

    void PillowManager::RandomizeTextures()
    {
        PillowManager* manager = Object::FindObjectOfType<PillowManager*>();
        if (!manager) return;
        Array<Pile*>* piles = manager->GetComponentsInChildren<Pile*>();

        if (!piles) return;
        for (int i = 0; i < piles->Length(); i++)
        {
            Pile* pile = piles->values[i];
            if (!pile) continue;
            pile->RandomizeTextures();
        }
    }

    void PillowManager::OnModEnable()
    {
        PillowManager* manager = Object::FindObjectOfType<PillowManager*>();
        if (!manager) return;
        if (manager->useSaved)
        {
            manager->useSaved = false;
            manager->SetActiveConstellation(manager->savedConstellation);
        }
        else
        {
            int childCount = manager->get_transform()->get_childCount();
            for (int i = 0; i < childCount; i++)
            {
                Transform* child = manager->get_transform()->GetChild(i);
                child->get_gameObject()->SetActive(true);
            }
        }
        
    }

    void PillowManager::OnModDisable()
    {
        PillowManager* manager = Object::FindObjectOfType<PillowManager*>();
        if (!manager) return;
        int childCount = manager->get_transform()->get_childCount();
        for (int i = 0; i < childCount; i++)
        {
            Transform* child = manager->get_transform()->GetChild(i);
            child->get_gameObject()->SetActive(false);
        }
    }
}