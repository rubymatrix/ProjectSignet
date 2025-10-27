// Copyright 2025, Red Lotus Games

#include "SignetCombatAttributeSet.h"
#include "Net/UnrealNetwork.h"

#define DRAGON_COMBAT_ATTRS(X) \
	/* Special */ \
	X(Reraise) \
	X(Regen) \
	X(Regain) \
	X(Refresh) \
	X(RegenPercent) \
	X(RefreshPercent) \
	/* Attack & Accuracy */ \
	X(Attack) \
	X(AttackPercent) \
	X(Accuracy) \
	X(RangedAttack) \
	X(RangedAttackPercent) \
	X(RangedAccuracy) \
	X(MagicAttack) \
	X(MagicAccuracy) \
	/* Defensive Stats */ \
	X(Defense) \
	X(DefensePercent) \
	X(Evasion) \
	X(MagicDefense) \
	X(MagicEvasion) \
	X(RangedDefense) \
	X(RangedEvasion) \
	X(TacticalParry) \
	X(GuardPercent) \
	X(ShieldBlockRate) \
	X(Retaliation) \
	/* Weapon-based */ \
	X(MeleeRange) \
	X(DamageRating) \
	X(WeaponDelay) \
	X(WeaponRank) \
	X(WeaponRating) \
	X(WeaponDamage) \
	X(SubWeaponRank) \
	X(SubWeaponRating) \
	X(SubWeaponDamage) \
	X(RangedWeaponDelay) \
	X(RangedWeaponRank) \
	X(RangedWeaponRating) \
	X(RangedWeaponDamage) \
	/* Elemental Attack & Accuracy */ \
	X(FireAttack) \
	X(IceAttack) \
	X(WindAttack) \
	X(EarthAttack) \
	X(LightningAttack) \
	X(WaterAttack) \
	X(LightAttack) \
	X(DarkAttack) \
	X(FireAccuracy) \
	X(IceAccuracy) \
	X(WindAccuracy) \
	X(EarthAccuracy) \
	X(LightningAccuracy) \
	X(WaterAccuracy) \
	X(LightAccuracy) \
	X(DarkAccuracy) \
	/* Damage Bonuses */ \
	X(HPDamageBonus) \
	X(PetHPDamageBonus) \
	X(DamageLimit) \
	X(DamageLimitPercent) \
	X(MagicBurstBonus) \
	X(MagicBurstBonusII) \
	X(BreathDamageBonus) \
	X(DayNukeBonus) \
	X(Iridescence) \
	/* Enmity */ \
	X(Enmity) \
	X(EnmityLossReduction) \
	/* TP Gen */ \
	X(StoreTP) \
	X(InhibitTP) \
	X(FencerTPBonus) \
	X(TacticalGuard) \
	X(SubtleBlow) \
	X(SubtleBlowII) \
	X(OccultAcumen) \
	/* Weapon Skill */ \
	X(WeaponSkillAccuracy) \
	X(WeaponSkillDamage) \
	X(WeaponSkillDamage_FirstHit) \
	X(TPBonus) \
	X(SaveTP) \
	X(ConserveTP) \
	X(WeaponSkillSTRBonus) \
	X(WeaponSkillDEXBonus) \
	X(WeaponSkillVITBonus) \
	X(WeaponSkillAGIBonus) \
	X(WeaponSkillINTBonus) \
	X(WeaponSkillMNDBonus) \
	X(WeaponSkillCHRBonus) \
	/* Recovery */ \
	X(HpHeal) \
	X(MpHeal) \
	/* Skill Rates */ \
	X(CombatSkillUpRate) \
	X(MagicSkillUpRate) \
	/* Damage Reductions */ \
	X(DamageTaken) \
	X(PhysicalDamageTaken) \
	X(PhysicalDamageTakenII) \
	X(UncappedPhsyicalDamageTaken) \
	X(BreathDamageTaken) \
	X(UncappedBreathDamageTaken) \
	X(MagicDamageTaken) \
	X(MagicDamageTakenII) \
	X(UncappedMagicDamageTaken) \
	X(RangedDamageTaken) \
	X(UncappedRangedDamageTaken) \
	X(AreaDamageTaken) \
	X(SlashingDamageTaken) \
	X(PiercingDamageTaken) \
	X(ImpactDamageTaken) \
	X(HandToHandDamageTaken) \
	X(FireDamageTaken) \
	X(IceDamageTaken) \
	X(WindDamageTaken) \
	X(EarthDamageTaken) \
	X(LightningDamageTaken) \
	X(WaterDamageTaken) \
	X(LightDamageTaken) \
	X(DarkDamageTaken) \
	/* Damage Nullifications */ \
	X(NullifyDamage) \
	X(NullifyPhysicalDamage) \
	X(NullifyBreathDamage) \
	X(NullifyMagicDamage) \
	X(NullifyRangedDamage) \
	X(NullifyFireDamage) \
	X(NullifyIceDamage) \
	X(NullifyWindDamage) \
	X(NullifyEarthDamage) \
	X(NullifyLightningDamage) \
	X(NullifyWaterDamage) \
	X(NullifyLightDamage) \
	X(NullifyDarkDamage) \
	/* Damage Absorbs */ \
	X(AbsorbDamage) \
	X(AbsorbPhysicalDamage) \
	X(AbsorbMagicDamage) \
	X(AbsorbFireDamage) \
	X(AbsorbIceDamage) \
	X(AbsorbWindDamage) \
	X(AbsorbEarthDamage) \
	X(AbsorbLightningDamage) \
	X(AbsorbWaterDamage) \
	X(AbsorbLightDamage) \
	X(AbsorbDarkDamage) \
	/* Attack Speed */ \
	X(Delay) \
	X(DelayPercent) \
	X(RangedDelay) \
	X(RangedDelayPercent) \
	X(AbilityHaste) \
	X(MagicHaste) \
	X(GearHaste) \
	X(TwoHandAbilityHaste) \
	X(DualWield) \
	X(MartialArts) \
	/* Attack Mods */ \
	X(Smite) \
	X(FencerCritHitRate) \
	X(CriticalHitRate) \
	X(CriticalHitDamage) \
	X(RangedCritHitDamage) \
	X(CritHitEvasion) \
	X(CritHitDefense) \
	X(MagicCritHitRate) \
	X(MagicCritHitDamage) \
	X(MagicDamage) \
	X(KickDamage) \
	/* Cure Mods */ \
	X(CurePotency) \
	X(CurePotencyII) \
	X(CurePotencyRecieved) \
	X(CurePotencyBonus) \
	/* Multi Attack */ \
	X(MaxSwings) \
	X(AdditionalSwingChance) \
	X(DoubleAttackRate) \
	X(DoubleAttackDamage) \
	X(TripleAttackRate) \
	X(QuadAttackRate) \
	X(ExtraKickAttack) \
	X(ExtraDualWieldAttack) \
	/* Spell Stuff */ \
	X(ClearMind) \
	X(ConserveMP) \
	X(Blink) \
	X(Stoneskin) \
	X(EnfeeblingMagicPotency) \
	X(EnfeeblingMagicDuration) \
	X(EnhancingMagicPotency) \
	X(EnhancingMagicDuration) \
	/* Ninjutsu */ \
	X(Utsusemi) \
	X(UtsusemiBonus) \
	X(NinjaTool) \
	X(NinjutsuBonus) \
	X(NinjutsuBonusInnin) \
	X(Daken) \
	X(NinjutsuDuration) \
	/* Casting Times */ \
	X(QuickMagic) \
	X(FastCast) \
	X(UncappedFastCast) \
	X(GrimoireCast) \
	X(CureCastTime) \
	X(ElementalCastTime) \
	X(SongCastTime) \
	X(SpellInterruptionRate) \
	/* Skill Chains */ \
	X(SKillChainBonus) \
	X(SkillChainDamage) \
	X(SKillChainDuration) \
	/* Killer Effects */ \
	X(VerminKiller) \
	X(BirdKiller) \
	X(AmorphKiller) \
	X(LizardKiller) \
	X(AquanKiller) \
	X(PlantoidKiller) \
	X(BeastKiller) \
	X(UndeadKiller) \
	X(ArcanaKiller) \
	X(DragonKiller) \
	X(DemonKiller) \
	X(EmptyKiller) \
	X(HumanoidKiller) \
	X(LuminianKiller) \
	X(LuminionKiller) \
	/* Resistances */ \
	X(SleepResistance) \
	X(PoisonResistance) \
	X(ParalyzeResistance) \
	X(BlindResistance) \
	X(SilenceResistance) \
	X(VirusResistance) \
	X(PetrifyResistance) \
	X(BindResistance) \
	X(CurseResistance) \
	X(GravityResistance) \
	X(SlowResistance) \
	X(StunResistance) \
	X(CharmResistance) \
	X(AmnesiaResistance) \
	X(LullabyResistance) \
	X(DeathResistance) \
	X(StatusResistance) \
	/* Enspells */ \
	X(Enspell) \
	X(EnspellDamage) \
	X(EnspellDamageBonus) \
	X(EnspellChance) \
	/* Spikes */ \
	X(Spikes) \
	X(SpikesDamage) \
	X(SpikesDamageBonus) \
	/* Songs */ \
	X(MinneEffect) \
	X(MinuetEffect) \
	X(PaeonEffect) \
	X(RequiemEffect) \
	X(ThrenodyEffect) \
	X(MadrigalEffect) \
	X(MamboEffect) \
	X(LullabyEffect) \
	X(EtudeEffect) \
	X(BalladEffect) \
	X(MarchEffect) \
	X(FinaleEffect) \
	X(CarolEffect) \
	X(MazurkaEffect) \
	X(ElegyEffect) \
	X(PreludeEffect) \
	X(HymnusEffect) \
	X(VirelaiEffect) \
	X(ScherzoEffect) \
	X(AllSongsEffect) \
	X(MaximumSongsBonus) \
	X(SongDurationBonus) \
	X(SongRecastDelay) \
	/* Affinities */ \
	X(FireAffinityDamage) \
	X(FireAffinityAccuracy) \
	X(FireAffinityPerpetuation) \
	X(IceAffinityDamage) \
	X(IceAffinityAccuracy) \
	X(IceAffinityPerpetuation) \
	X(WindAffinityDamage) \
	X(WindAffinityAccuracy) \
	X(WindAffinityPerpetuation) \
	X(EarthAffinityDamage) \
	X(EarthAffinityAccuracy) \
	X(EarthAffinityPerpetuation) \
	X(LightningAffinityDamage) \
	X(LightningAffinityAccuracy) \
	X(LightningAffinityPerpetuation) \
	X(WaterAffinityDamage) \
	X(WaterAffinityAccuracy) \
	X(WaterAffinityPerpetuation) \
	X(LightAffinityDamage) \
	X(LightAffinityAccuracy) \
	X(LightAffinityPerpetuation) \
	X(DarkAffinityDamage) \
	X(DarkAffinityAccuracy) \
	X(DarkAffinityPerpetuation) \
	/* Points */ \
	X(ExpBonus) \
	X(CapacityBonus) \
	X(MasterBonus)

USignetCombatAttributeSet::USignetCombatAttributeSet()
{
	DRAGON_COMBAT_ATTRS(DRAGON_INIT)
}


#define DRAGON_DEFINE(Name) DRAGON_DEFINE_ONREP(USignetCombatAttributeSet, Name)
DRAGON_COMBAT_ATTRS(DRAGON_DEFINE);
#undef DRAGON_DEFINE

void USignetCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

#define SIGNET_REP(Name) DRAGON_DO_REP(USignetCombatAttributeSet, Name)
	DRAGON_COMBAT_ATTRS(SIGNET_REP);
#undef SIGNET_REP
}

#undef DRAGON_COMBAT_ATTRS