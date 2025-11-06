#include "Grades.h"

// =====================
// Static table data
// =====================

const TArray<TArray<uint8>> UGrades::JobGrades = {
    { {0,0,0,0,0,0,0,0,0} },   // NON
    { {2,0,1,3,4,3,6,6,5} },   // WAR
    { {1,0,3,2,1,6,7,4,5} },   // MNK
    { {5,3,4,6,4,5,5,1,3} },   // WHM
    { {6,2,6,3,6,3,1,5,4} },   // BLM
    { {4,4,4,4,5,5,3,3,4} },   // RDM
    { {4,0,4,1,4,2,3,7,7} },   // THF
    { {3,6,2,5,1,7,7,3,3} },   // PLD
    { {3,6,1,3,3,4,3,7,7} },   // DRK
    { {3,0,4,3,4,6,5,5,1} },   // BST
    { {4,0,4,4,4,6,4,4,2} },   // BRD
    { {5,0,5,4,4,1,5,4,5} },   // RNG
    { {2,0,3,3,3,4,5,5,4} },   // SAM
    { {4,0,3,2,3,2,4,7,6} },   // NIN
    { {3,0,2,4,3,4,6,5,3} },   // DRG
    { {7,1,6,5,6,4,2,2,2} },   // SMN
    { {4,4,5,5,5,5,5,5,5} },   // BLU
    { {4,0,5,3,5,2,3,5,5} },   // COR
    { {4,0,5,2,4,3,5,6,3} },   // PUP
    { {4,0,4,3,5,2,6,6,2} },   // DNC
    { {5,4,6,4,5,4,3,4,3} },   // SCH
    { {3,2,6,4,5,4,3,3,4} },   // GEO
    { {3,6,3,4,5,2,4,4,6} }    // RUN
};

const TArray<TArray<uint8>> UGrades::RaceGrades = {
    { {4,4,4,4,4,4,4,4,4} },   // Hume
    { {4,4,4,4,4,4,4,4,4} },   // Hume
    { {3,5,2,5,3,6,6,2,4} },   // Elvaan
    { {3,5,2,5,3,6,6,2,4} },   // Elvaan
    { {7,1,6,4,5,3,1,5,4} },   // Tarutaru
    { {7,1,6,4,5,3,1,5,4} },   // Tarutaru
    { {4,4,5,1,5,2,4,5,6} },   // Mithra
    { {1,7,3,4,1,5,5,4,6} }    // Galka
};

const TArray<TArray<float>> UGrades::HPScale = {
    { {0,0,0,0,0} },
    { {19,9,1,3,3} },
    { {17,8,1,3,3} },
    { {16,7,1,3,3} },
    { {14,6,0,3,3} },
    { {13,5,0,2,2} },
    { {11,4,0,2,2} },
    { {10,3,0,2,2} }
};

const TArray<TArray<float>> UGrades::MPScale = {
    { {0,0,0} },
    { {16,6,4} },
    { {14,5,4} },
    { {12,4,4} },
    { {10,3,4} },
    { {8,2,3} },
    { {6,1,2} },
    { {4,0.5f,1} }
};

const TArray<TArray<float>> UGrades::StatScale = {
    { {0,0,0,0} },
    { {5,0.50f,0.10f,0.35f} },
    { {4,0.45f,0.20f,0.35f} },
    { {4,0.40f,0.25f,0.35f} },
    { {3,0.35f,0.35f,0.35f} },
    { {3,0.30f,0.35f,0.35f} },
    { {2,0.25f,0.40f,0.35f} },
    { {2,0.20f,0.40f,0.35f} }
};

const TArray<TArray<float>> UGrades::MobHPScale = {
    { {0,0,0} },
    { {36,9,1} },
    { {33,8,1} },
    { {32,7,1} },
    { {29,6,0} },
    { {27,5,0} },
    { {24,4,0} },
    { {22,3,0} }
};

const TArray<TArray<float>> UGrades::MobRBI = {
    { {0,0} },
    { {1,0} },
    { {2,0} },
    { {3,3} },
    { {4,7} },
    { {5,14} }
};

// =====================
// Static function impls
// =====================

uint8 UGrades::GetJobGrade(uint8 Job, uint8 Stat)
{
    return JobGrades[Job][Stat];
}

uint8 UGrades::GetRaceGrade(uint8 Race, uint8 Stat)
{
    return RaceGrades[Race][Stat];
}

float UGrades::GetHPScale(uint8 Rank, uint8 Scale)
{
    return HPScale[Rank][Scale];
}

float UGrades::GetMPScale(uint8 Rank, uint8 Scale)
{
    return MPScale[Rank][Scale];
}

float UGrades::GetStatScale(uint8 Rank, uint8 Scale)
{
    return StatScale[Rank][Scale];
}

float UGrades::GetMobHPScale(uint8 Rank, uint8 Scale)
{
    return MobHPScale[Rank][Scale];
}

float UGrades::GetMobRBI(uint8 Rank, uint8 Scale)
{
    return MobRBI[Rank][Scale];
}
