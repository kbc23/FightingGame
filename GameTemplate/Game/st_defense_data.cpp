#include "stdafx.h"
#include "st_defense_data.h"

void StDefenseData::Update()
{
    //CountGuardBreakTime();
}

//void StDefenseData::DecreaseDefenseValue(const int decreaseDefenseValue)
//{
//    m_defenseValue -= decreaseDefenseValue;
//
//    // 防御値が０以下になったら
//    if (0 >= m_defenseValue) {
//        m_defenseValue = 0;
//        m_flagDefense = false;
//        m_flagGuardBreak = true;
//    }
//}

//void StDefenseData::CountGuardBreakTime()
//{
//    if (false == m_flagGuardBreak) {
//        return;
//    }
//
//    ++m_countGrardBreakTimer; // カウント
//
//    if (m_MAX_COUNT_GRARD_BRAEK_TIMER <= m_countGrardBreakTimer) {
//        m_countGrardBreakTimer = 0;
//        m_flagGuardBreak = false;
//        m_defenseValue = m_INIT_DEFENSE_VALUE;
//    }
//}