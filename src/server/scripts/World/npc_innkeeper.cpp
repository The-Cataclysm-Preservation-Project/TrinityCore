/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameEventMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "WorldSession.h"

enum InnkeeperSpells
{
    SPELL_TRICK_OR_TREATED                          = 24755,
    SPELL_TREAT                                     = 24715,

    SPELL_TRICK                                     = 24753,
    SPELL_LEPPER_GNOME_COSTUME                      = 24713,
    SPELL_MALE_GHOST_COSTUME                        = 24735,
    SPELL_FEMALE_GHOST_COSTUME                      = 24736,
    SPELL_MALE_NINJA_COSTUME                        = 24710,
    SPELL_FEMALE_NINJA_COSTUME                      = 24711,
    SPELL_MALE_PIRATE_COSTUME                       = 24708,
    SPELL_FEMALE_PIRATE_COSTUME                     = 24709,
    SPELL_SKELETON_COSTUME                          = 24723,
    SPELL_HALLOW_END_CANDY_1                        = 24924,
    SPELL_HALLOW_END_CANDY_2                        = 24925,
    SPELL_HALLOW_END_CANDY_3                        = 24926,
    SPELL_HALLOW_END_CANDY_4                        = 24927
};

#define LOCALE_TRICK_OR_TREAT_0 "Trick or Treat!"
#define LOCALE_TRICK_OR_TREAT_1 "사탕 하나 주면 안 잡아먹지!"
#define LOCALE_TRICK_OR_TREAT_2 "Des bonbons ou des blagues!"
#define LOCALE_TRICK_OR_TREAT_3 "Süßes oder Saures!"
#define LOCALE_TRICK_OR_TREAT_4 "没有糖果就捣乱！"
#define LOCALE_TRICK_OR_TREAT_5 "不給糖就搗蛋!"
#define LOCALE_TRICK_OR_TREAT_6 "¡Truco o trato!"
#define LOCALE_TRICK_OR_TREAT_7 "¡Truco o trato!"
#define LOCALE_TRICK_OR_TREAT_8 "Конфета или жизнь!"
#define LOCALE_TRICK_OR_TREAT_10 "Doçura ou travessura!"
#define LOCALE_TRICK_OR_TREAT_11 "Dolcetto o scherzetto!"

#define LOCALE_INNKEEPER_0 "Make this inn my home."
#define LOCALE_INNKEEPER_1 "이 여관을 귀환 장소로 사용합니다."
#define LOCALE_INNKEEPER_2 "Faites de cette auberge votre foyer."
#define LOCALE_INNKEEPER_3 "Ich möchte dieses Gasthaus zu meinem Heimatort machen."
#define LOCALE_INNKEEPER_4 "将这座旅店设为你的家。"
#define LOCALE_INNKEEPER_5 "將這座旅店設為你的家。"
#define LOCALE_INNKEEPER_6 "Fija tu hogar en esta taberna."
#define LOCALE_INNKEEPER_7 "Fija tu hogar en esta taberna."
#define LOCALE_INNKEEPER_8 "Я хочу остановиться здесь."
#define LOCALE_INNKEEPER_10 "Faça desta estalagem seu lar."
#define LOCALE_INNKEEPER_11 "Rendi questo luogo la mia dimora."

#define LOCALE_VENDOR_0 "I want to browse your goods."
#define LOCALE_VENDOR_1 "물건을 보고 싶습니다."
#define LOCALE_VENDOR_2 "Je voudrais regarder vos articles."
#define LOCALE_VENDOR_3 "Ich sehe mich nur mal um."
#define LOCALE_VENDOR_4 "我想要看看你卖的货物。"
#define LOCALE_VENDOR_5 "我想要看看你賣的貨物。"
#define LOCALE_VENDOR_6 "Quiero ver tus mercancías."
#define LOCALE_VENDOR_7 "Quiero ver tus mercancías."
#define LOCALE_VENDOR_8 "Я хочу посмотреть на ваши товары."
#define LOCALE_VENDOR_10 "Deixe-me dar uma olhada nas suas mercadorias."
#define LOCALE_VENDOR_11 "Fammi vedere la tua merce."

struct npc_innkeeper : public ScriptedAI
{
    npc_innkeeper(Creature* creature) : ScriptedAI(creature) { }

    bool GossipHello(Player* player) override
    {
        if (IsHolidayActive(HOLIDAY_HALLOWS_END) && !player->HasAura(SPELL_TRICK_OR_TREATED))
        {
            char const* localizedEntry;

            switch (player->GetSession()->GetSessionDbcLocale())
            {
                case LOCALE_koKR:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_1;
                    break;
                case LOCALE_frFR:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_2;
                    break;
                case LOCALE_deDE:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_3;
                    break;
                case LOCALE_zhCN:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_4;
                    break;
                case LOCALE_zhTW:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_5;
                    break;
                case LOCALE_esES:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_6;
                    break;
                case LOCALE_esMX:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_7;
                    break;
                case LOCALE_ruRU:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_8;
                    break;
                case LOCALE_ptBR:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_10;
                    break;
                case LOCALE_itIT:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_11;
                    break;
                case LOCALE_enUS:
                default:
                    localizedEntry = LOCALE_TRICK_OR_TREAT_0;
                    break;
            }

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, localizedEntry, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        }

        if (me->IsQuestGiver())
            player->PrepareQuestMenu(me->GetGUID());

        if (me->IsVendor())
        {
            char const* localizedEntry;

            switch (player->GetSession()->GetSessionDbcLocale())
            {
                case LOCALE_koKR:
                    localizedEntry = LOCALE_VENDOR_1;
                    break;
                case LOCALE_frFR:
                    localizedEntry = LOCALE_VENDOR_2;
                    break;
                case LOCALE_deDE:
                    localizedEntry = LOCALE_VENDOR_3;
                    break;
                case LOCALE_zhCN:
                    localizedEntry = LOCALE_VENDOR_4;
                    break;
                case LOCALE_zhTW:
                    localizedEntry = LOCALE_VENDOR_5;
                    break;
                case LOCALE_esES:
                    localizedEntry = LOCALE_VENDOR_6;
                    break;
                case LOCALE_esMX:
                    localizedEntry = LOCALE_VENDOR_7;
                    break;
                case LOCALE_ruRU:
                    localizedEntry = LOCALE_VENDOR_8;
                    break;
                case LOCALE_ptBR:
                    localizedEntry = LOCALE_VENDOR_10;
                    break;
                case LOCALE_itIT:
                    localizedEntry = LOCALE_VENDOR_11;
                    break;
                case LOCALE_enUS:
                default:
                    localizedEntry = LOCALE_VENDOR_0;
                    break;
            }

            AddGossipItemFor(player, GOSSIP_ICON_VENDOR, localizedEntry, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        }

        if (me->IsInnkeeper())
        {
            char const* localizedEntry;

            switch (player->GetSession()->GetSessionDbcLocale())
            {
                case LOCALE_koKR:
                    localizedEntry = LOCALE_INNKEEPER_1;
                    break;
                case LOCALE_frFR:
                    localizedEntry = LOCALE_INNKEEPER_2;
                    break;
                case LOCALE_deDE:
                    localizedEntry = LOCALE_INNKEEPER_3;
                    break;
                case LOCALE_zhCN:
                    localizedEntry = LOCALE_INNKEEPER_4;
                    break;
                case LOCALE_zhTW:
                    localizedEntry = LOCALE_INNKEEPER_5;
                    break;
                case LOCALE_esES:
                    localizedEntry = LOCALE_INNKEEPER_6;
                    break;
                case LOCALE_esMX:
                    localizedEntry = LOCALE_INNKEEPER_7;
                    break;
                case LOCALE_ruRU:
                    localizedEntry = LOCALE_INNKEEPER_8;
                    break;
                case LOCALE_ptBR:
                    localizedEntry = LOCALE_INNKEEPER_10;
                    break;
                case LOCALE_itIT:
                    localizedEntry = LOCALE_INNKEEPER_11;
                    break;
                case LOCALE_enUS:
                default:
                    localizedEntry = LOCALE_INNKEEPER_0;
                    break;
            }

            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, localizedEntry, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INN);
        }

        player->TalkedToCreature(me->GetEntry(), me->GetGUID());

        SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());

        return true;
    }

    bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
    {
        uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);

        ClearGossipMenuFor(player);

        if (action == GOSSIP_ACTION_INFO_DEF + 1 && IsHolidayActive(HOLIDAY_HALLOWS_END) && !player->HasAura(SPELL_TRICK_OR_TREATED))
        {
            player->CastSpell(player, SPELL_TRICK_OR_TREATED, true);

            if (urand(0, 1) == 0)
                player->CastSpell(player, SPELL_TREAT, true);
            else
            {
                uint32 trickspell = 0;

                switch (urand(0, 12))
                {
                    case 0:
                        trickspell = SPELL_TRICK;
                        break;
                    case 1:
                        trickspell = SPELL_LEPPER_GNOME_COSTUME;
                        break;
                    case 2:
                        trickspell = SPELL_MALE_GHOST_COSTUME;
                        break;
                    case 3:
                        trickspell = SPELL_FEMALE_GHOST_COSTUME;
                        break;
                    case 4:
                        trickspell = SPELL_MALE_NINJA_COSTUME;
                        break;
                    case 5:
                        trickspell = SPELL_FEMALE_NINJA_COSTUME;
                        break;
                    case 6:
                        trickspell = SPELL_MALE_PIRATE_COSTUME;
                        break;
                    case 7:
                        trickspell = SPELL_FEMALE_PIRATE_COSTUME;
                        break;
                    case 8:
                        trickspell = SPELL_SKELETON_COSTUME;
                        break;
                    case 9:
                        trickspell = SPELL_HALLOW_END_CANDY_1;
                        break;
                    case 10:
                        trickspell = SPELL_HALLOW_END_CANDY_2;
                        break;
                    case 11:
                        trickspell = SPELL_HALLOW_END_CANDY_3;
                        break;
                    case 12:
                        trickspell = SPELL_HALLOW_END_CANDY_4;
                        break;
                    default:
                        break;
                }

                player->CastSpell(player, trickspell, true);
            }

            CloseGossipMenuFor(player);

            return true;
        }

        CloseGossipMenuFor(player);

        switch (action)
        {
            case GOSSIP_ACTION_TRADE:
                player->GetSession()->SendListInventory(me->GetGUID());
                break;
            case GOSSIP_ACTION_INN:
                player->SetBindPoint(me->GetGUID());
                break;
            default:
                break;
        }

        return true;
    }
};

void AddSC_npc_innkeeper()
{
    RegisterCreatureAI(npc_innkeeper);
}
