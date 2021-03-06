/*!
*@brief	ゲーム時間クラス。
* シングルトン。
*/
#pragma once

#include "Stopwatch.h"
#include <queue>

class TkEngine;

/*!
 *@brief	ゲーム時間。
 */
class GameTime{
	GameTime() 
	{
	}
	~GameTime()
	{
	}
public:
	/*!
	 *@brief	1フレームの経過時間を取得(単位・秒)。
	 */
	const float GetFrameDeltaTime() const
	{
		//固定FPSにする。可変は要検討。
		//return 1.0f / 30.0f;
		return m_frameDeltaTime;
	}
	void PushFrameDeltaTime(float deltaTime)
	{
		m_frameDeltaTimeQue.push_back(deltaTime);
		if (m_frameDeltaTimeQue.size() > 30.0f) {
			float totalTime = 0.0f;
			for (auto time : m_frameDeltaTimeQue) {
				totalTime += time;
			}
			//平均値をとる。
			m_frameDeltaTime = min(1.0f / 30.0f, totalTime / m_frameDeltaTimeQue.size());
			m_frameDeltaTimeQue.pop_front();
		}
	}
	/// <summary>
	/// 計測開始
	/// </summary>
	/// <remark>
	/// 本関数はエンジン内でのみ使用します。
	/// ユーザーは使用しないでください。
	/// </remark>
	void BeginMeasurement()
	{
		//計測開始。
		m_sw.Start();
	}
	/// <summary>
	/// 計測終了
	/// </summary>
	/// <remark>
	/// 本関数はエンジン内でのみ使用します。
	/// ユーザーは使用しないでください。
	/// </remark>
	void EndMeasurement()
	{
		m_sw.Stop();
		PushFrameDeltaTime(static_cast<float>(m_sw.GetElapsed()));
	}
private:
	friend class TkEngine;
	Stopwatch m_sw;
	std::list<float> m_frameDeltaTimeQue;
	float		m_frameDeltaTime = 1.0f / 60.0f;		//1フレームの経過時間。



public:
	/// <summary>
	/// 1フレームの経過時間を固定化させます。
	/// </summary>
	/// <remark>
	/// どんな時にこれを使うのか？
	/// 例えば、完全同期型のオンラインマルチプレイなど。
	/// 完全同期型のオンラインゲームでは、各クライアント間でゲームの進行速度を一致させる必要があります。
	/// ですので、可変フレームレートでなく、固定フレームレートでゲームを作ります。
	/// そのような場合にゲーム時間を固定化させてください。
	/// </remark>
	/// <param name="fixedFrameDeltaTime"></param>
	void EnableFixedFrameDeltaTime(float fixedFrameDeltaTime)
	{
		m_fixedFrameDeltaTime = fixedFrameDeltaTime;
		m_isFixedFrameDeltaTime = true;
	}
	/// <summary>
	/// 1フレームの経過時間の固定化を解除します。
	/// </summary>
	void DisableFixedFrameDeltaTime()
	{
		m_isFixedFrameDeltaTime = false;
	}


private:
	float		m_fixedFrameDeltaTime = 1.0f / 60.0f;	// 固定経過時間。
	bool		m_isFixedFrameDeltaTime = false;		// 1フレームの経過時間を固定化する。




};
