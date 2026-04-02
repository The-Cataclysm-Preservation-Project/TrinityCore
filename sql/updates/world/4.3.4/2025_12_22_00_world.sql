-- 2025_03_14_00_world.sql updated spell 29266 to use 
-- `spell_gen_feign_death_all_flags_no_uninteractible`
-- to fix issues with uninteractible dead NPCs.
-- 
-- For example, Lieutenant Walden (ID 34863), the very
-- first NPC to complete the very first quest of the 
-- Worgen starting zone, was uninteractible for non-GM 
-- players.
-- 
-- However, 2025_03_15_00_world.sql then attempted to 
-- delete the previous, non-existent entry
-- `spell_gen_feign_death_all_flags_uninteractible`
-- and... re-inserts it.
-- This results in two entries for spell 29266, with 
-- the uninteractible one wrongfully taking over.
-- 
-- The following resolves the conflict.
DELETE FROM `spell_script_names` 
      WHERE `spell_id`   = 29266 
        AND `ScriptName` = 'spell_gen_feign_death_all_flags_uninteractible';
