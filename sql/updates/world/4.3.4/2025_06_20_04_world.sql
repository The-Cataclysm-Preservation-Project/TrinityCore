UPDATE `spell_dbc` SET `DurationIndex`= 105 WHERE `Id`= 32733;
DELETE FROM `spelleffect_dbc` WHERE `SpellId`= 32733;
INSERT INTO `spelleffect_dbc` (`Id`, `SpellId`, `EffectIndex`, `Effect`, `EffectAura`, `EffectImplicitTargetA`, `Comment`) VALUES
(160116, 32733, 0, 6, 4, 1, '');

DELETE FROM `spell_script_names` WHERE `ScriptName`= 'spell_pal_blessed_life';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(-31828, 'spell_pal_blessed_life');
