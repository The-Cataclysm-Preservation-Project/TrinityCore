
#include "NewArchaeology.h"
#include "ArchaeologyPackets.h"
#include "NewArchaeologyMgr.h"
#include "Containers.h"
#include "Guild.h"
#include "Log.h"
#include "SpellDefines.h"
#include "SpellPackets.h"

NewArchaeology::NewArchaeology(Player* owner) : _owner(owner)
{
}

void NewArchaeology::LoadResearchHistory(PreparedQueryResult result)
{
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 researchProjectRecId = fields[0].GetUInt32();

        if (ResearchProjectEntry const* projectEntry = sResearchProjectStore.LookupEntry(researchProjectRecId))
            _researchHistory.History[projectEntry->ResearchBranchID].emplace_back(projectEntry, fields[1].GetUInt32(), fields[2].GetUInt32());

    } while (result->NextRow());
}

void NewArchaeology::LoadResearchSites(PreparedQueryResult result)
{
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 researchSiteRectId = fields[0].GetUInt32();
        if (ResearchSiteEntry const* siteEntry = sResearchSiteStore.LookupEntry(researchSiteRectId))
        {
            uint8 researchSiteIndex = fields[1].GetUInt8();

            ResearchSite& site = _researchSites[siteEntry->MapID].GetResearchSite(researchSiteIndex);
            site.RecID = researchSiteRectId;
            site.RemainingFinds = fields[2].GetUInt32();
        }

    } while (result->NextRow());
}

void NewArchaeology::LoadResearchProjects(PreparedQueryResult result)
{
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 researchProjectRecId = fields[0].GetUInt32();
        if (ResearchProjectEntry const* projectEntry = sResearchProjectStore.LookupEntry(researchProjectRecId))
            _researchProjects[projectEntry->ResearchBranchID] = projectEntry;

    } while (result->NextRow());
}

void NewArchaeology::BuildResearchHistoryPacket(WorldPackets::Archaeology::SetupResearchHistory& packet)
{
    for (auto& itr : _researchHistory.History)
    {
        for (ResearchHistoryInfo const& info : itr.second)
        {
            WorldPackets::Archaeology::ResearchHistory& history = packet.History.emplace_back();
            history.ProjectID = info.ProjectEntry->ID;
            history.CompletionCount = info.CompletionCount;
            history.FirstCompleted = info.FirstCompletionTime;
        }
    }
}

void NewArchaeology::GenerateResearchSites()
{
    for (uint32 continentId : sNewArchaeologyMgr->GetResearchSiteContinentIds())
        GenerateResearchSitesForContinent(continentId);
}

inline uint32 GetRequiredSkillValueForResearchBranch(ResearchBranches researchBranchRecId)
{
    switch (researchBranchRecId)
    {
        case ResearchBranches::Draenei:
        case ResearchBranches::Orc:
            return 300u;
        case ResearchBranches::Nerubian:
        case ResearchBranches::Vrykul:
            return 375u;
        case ResearchBranches::Tolvir:
            return 450u;
        default:
            return 0u;
    }

    return 0u;
}

void NewArchaeology::GenerateResearchSitesForContinent(uint32 continentId)
{
    ResearchSites& researchSites = _researchSites[continentId];

    // Player has all sites occupied.
    if (!researchSites.HasUnusedResearchSite())
        return;

    // Find branches that we may research
    std::unordered_set<uint32> const* researchBranches = sNewArchaeologyMgr->GetResearchBranchesForContinent(continentId);
    if (!researchBranches || researchBranches->empty())
        return;

    std::unordered_set<uint32> elegibleSites;
    for (uint32 researchBranch : *researchBranches)
    {
        // Some research branches have a skill requirement as they are endgame content for their respective expansion
        if (_owner->GetSkillValue(SKILL_ARCHAEOLOGY) < GetRequiredSkillValueForResearchBranch(static_cast<ResearchBranches>(researchBranch)))
            continue;

        std::vector<ResearchSiteEntry const*>* researchSiteEntries = sNewArchaeologyMgr->GetResearchSitesForBranch(researchBranch);
        if (!researchSiteEntries)
            continue;

        // Patch 4.1.0 (2011-04-26): Players now have a much smaller chance of getting a dig site for a race for which they have completed all rare finds.
        // Dev Note: Blizzard has never revealed any chance values for branches or projects so we go with a 50% chance for the time being.
        if (_researchHistory.HasCompletedAllRareProjectsOfBranch(researchBranch))
            if (roll_chance_i(50))
                continue;

        for (ResearchSiteEntry const* siteEntry : *researchSiteEntries)
        {
            if (siteEntry->MapID != continentId)
                continue;

            // do not allow to get the same research site twice
            if (researchSites.HasResearchSiteActive(siteEntry->ID))
                continue;

            elegibleSites.insert(siteEntry->ID);
        }
    }

    if (elegibleSites.empty())
        return;

    uint8 siteIndex = 0u;
    while (researchSites.GetNextUnusedResearchSiteIndex(siteIndex) && !elegibleSites.empty())
    {
        uint32 researchSite = Trinity::Containers::SelectRandomContainerElement(elegibleSites);
        researchSites.SetActiveResearchSite(siteIndex, researchSite);
        _owner->SetResearchSiteId(sNewArchaeologyMgr->GetResearchSiteFieldIndexForContinent(continentId, siteIndex), researchSite);
        elegibleSites.erase(researchSite);
    }
}

void NewArchaeology::StartResearchProjectForBranch(ResearchBranches researchBranch)
{
    auto itr = _researchProjects.find(AsUnderlyingType(researchBranch));
    if (itr != _researchProjects.end() && itr->second != nullptr)
        return;

    if (_owner->GetSkillValue(SKILL_ARCHAEOLOGY) < GetRequiredSkillValueForResearchBranch(researchBranch))
    {
        TC_LOG_ERROR("player", "Player (%s) tried to start an archaeology project for research branch (RecID: %u) that his current skill level doesn't allow yet. Possible dirty database data or gamemaster command abuse.", _owner->GetName().c_str(), AsUnderlyingType(researchBranch));
        return;
    }

    std::vector<ResearchProjectEntry const*>* researchProjects = sNewArchaeologyMgr->GetResearchProjectsForBranch(AsUnderlyingType(researchBranch));
    if (!researchProjects || researchProjects->empty())
        return;

    std::vector<ResearchProjectEntry const*> eligibleProjects;
    for (ResearchProjectEntry const* researchProject : *researchProjects)
    {
        // Rare research projects are unique and can only be completed once per character
        if (static_cast<ResearchProjectRarity>(researchProject->Rarity) == ResearchProjectRarity::Rare)
            if (_researchHistory.HasCompletedProject(researchProject))
                continue;

        // Skill requirement
        if (_owner->GetSkillValue(SKILL_ARCHAEOLOGY) < sNewArchaeologyMgr->GetRequiredSkillValueForResearchProject(researchProject->ID))
            continue;

        eligibleProjects.push_back(researchProject);
    }

    uint8 fieldIndex = sNewArchaeologyMgr->GetResearchBranchFieldIndex(researchBranch);
    if (eligibleProjects.empty())
    {
        TC_LOG_ERROR("player", "Player (%s) tried to start an archaeology project for research branch (RecID: %u) but no project is available.", _owner->GetName().c_str(), AsUnderlyingType(researchBranch));
        _owner->SetResearchProjectId(fieldIndex, 0);
        return;
    }

    ResearchProjectEntry const* researchProject = Trinity::Containers::SelectRandomContainerElement(eligibleProjects);
    _researchProjects[AsUnderlyingType(researchBranch)] = researchProject;
    _owner->SetResearchProjectId(fieldIndex, researchProject->ID);
}

bool NewArchaeology::HasResearchProjectActive(ResearchProjectEntry const* projectEntry) const
{
    auto itr = _researchProjects.find(projectEntry->ResearchBranchID);
    if (itr == _researchProjects.end())
        return false;

    return itr->second->ID == projectEntry->ID;
}

void NewArchaeology::CompleteResearchProject(uint32 researchProjectRecId, std::vector<WorldPackets::Spells::SpellWeight> const& weights)
{
    ResearchProjectEntry const* projectEntry = sResearchProjectStore.LookupEntry(researchProjectRecId);
    if (!projectEntry)
    {
        TC_LOG_ERROR("player", "Player (%s) tried to complete an archaeology project (RecID: %u) but there is no such project.", _owner->GetName().c_str(), researchProjectRecId);
        return;
    }

    if (!HasResearchProjectActive(projectEntry))
        return;

    // Store the completed project and we may continue
    if (ResearchHistoryInfo const* historyInfo = _researchHistory.StoreProjectInHistory(projectEntry))
    {
        // New unique history entry. Update achievement criterias
        if (historyInfo->CompletionCount == 1)
            _owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS, projectEntry->ID, 1);

        WorldPackets::Archaeology::ResearchComplete packet;
        packet.Research.ProjectID = projectEntry->ID;
        packet.Research.CompletionCount = historyInfo->CompletionCount;
        packet.Research.FirstCompleted = historyInfo->FirstCompletionTime;
        _owner->SendDirectMessage(packet.Write());
    }

    // Guilds count all research projects
    if (Guild* guild = _owner->GetGuild())
        guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS, projectEntry->ID, 1, 0, nullptr, nullptr);

    // Project has been completed, item has been added via Spell::DoCreateItem, time to destroy the items and currencies
    for (WorldPackets::Spells::SpellWeight const& weight : weights)
    {
        switch (static_cast<SpellWeightType>(weight.Type))
        {
            case SpellWeightType::Currency:
                _owner->ModifyCurrency(weight.ID, weight.Quantity * -1);
                break;
            case SpellWeightType::Item:
                _owner->DestroyItemCount(weight.ID, weight.Quantity, true);
                break;
            default:
                break;
        }
    }

    _researchProjects[projectEntry->ResearchBranchID] = nullptr;
    _owner->SetResearchProjectId(sNewArchaeologyMgr->GetResearchBranchFieldIndex(static_cast<ResearchBranches>(projectEntry->ResearchBranchID)), 0);
    StartResearchProjectForBranch(static_cast<ResearchBranches>(projectEntry->ResearchBranchID));
}
