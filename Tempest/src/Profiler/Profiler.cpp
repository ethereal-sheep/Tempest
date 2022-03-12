#include "Core.h"
#include "Logger/Log.h"
#include <intrin.h>

//#include <intrin.h>
#include "Profiler.h"
#include "ProfilerColor.h"

//#pragma intrinsic(__rdtsc)


namespace Tempest::Profile
{
	class ProfilerImplementation
	{
		enum class State
		{
			STOPPED, RECORDING, START
		};


		struct FunctionHash
		{
			size_t operator()(const FunctionKey& hashable) const
			{
				return (std::hash<std::string>()(hashable.m_Filename) ^ hashable.m_LineNumber);
			}
		};

		using ProfileZones = std::unordered_map<std::string, uint32_t>;
		using PerformanceLog = std::unordered_map<FunctionKey, FunctionData, FunctionHash>;
		using PerformanceStack = std::vector<const FunctionKey*>;
		using Snapshots = std::vector<FrameSnapshot>;

		uint64_t getCPUCycle()
		{
			return __rdtsc();
		}

		time_point getCurrentTime()
		{
			return current_clock::now();
		}
	public:
		ProfilerImplementation()
		{
			m_State = State::STOPPED;
			Reset();
		}
		~ProfilerImplementation() = default;

		void onEnter(
			const char* const zone,
			const char* const fileName,
			const char* const functionName,
			const unsigned int lineNumber)
		{
			
			if (m_State == State::STOPPED)
				return;

			//add zone
			if(!m_Zones.count(zone))
				m_Zones.insert(std::make_pair(zone, ++m_ZoneIndex));

			//uint64_t endCycleCount = getCPUCycle();
			time_point endTime = getCurrentTime();

			//std::assert m_Stack.size()

			//Update the total time spent processing the previous function
			//uint64_t runCycleCount = endCycleCount - m_LastCycleCount; // -m_CallTime;
			duration runTime = endTime - m_LastTime;

			//m_Log[*m_Stack.back()].m_Cycles += runCycleCount;
			m_Log[*m_Stack.back()].m_Duration += runTime;

			//m_TotalCycleCount += runCycleCount;
			m_TotalTime += runTime;

			FunctionKey newData{ zone, fileName, functionName, lineNumber };

			bool exist = m_Log.count(newData);

			auto [iterator, unused] = m_Log.insert(std::make_pair(newData, FunctionData{}));

			// update the call count
			++m_TotalCallCount;
			++(iterator->second).m_Calls;

			m_Stack.push_back(&iterator->first);
			m_CurrentSequence.emplace_back(m_Stack.back(), endTime, true);

			if (!exist)
				m_FnList.emplace_back(m_Stack.back());


			//m_LastCycleCount = getCPUCycle();
			m_LastTime = getCurrentTime();
		}

		void onExit()
		{
			if (m_State == State::STOPPED)
				return;

			//uint64_t endCycleCount = getCPUCycle();
			time_point endTime = getCurrentTime();

			//uint64_t runCycleCount = endCycleCount - m_LastCycleCount; // -m_CallTime;
			duration runTime = endTime - m_LastTime;


			//m_Log[*m_Stack.back()].m_Cycles += runCycleCount;
			m_Log[*m_Stack.back()].m_Duration += runTime;

			//m_TotalCycleCount += runCycleCount;
			m_TotalTime += runTime;

			m_CurrentSequence.emplace_back(m_Stack.back(), endTime, false);
			m_Stack.pop_back();

			//m_LastCycleCount = getCPUCycle();
			m_LastTime = getCurrentTime();
		}

		void OnFrameStart()
		{
			time_point frameStartTime = getCurrentTime();
			time_point temp = m_LastFrameTime;
			m_FrameTime = frameStartTime - m_LastFrameTime;
			m_LastFrameTime = frameStartTime;

			
			switch (m_State)
			{
			case State::STOPPED:
				if (!m_RequestRecord)
					return;
				else
					m_State = State::START;
				[[fallthrough]];

			case State::START:
				Reset();
				m_State = State::RECORDING;
				[[fallthrough]];

			case State::RECORDING:

				if (!m_RequestRecord)
				{
					m_State = State::STOPPED;
					return;
				}

				++m_TotalFrameCount;
				m_TimeElapsed = frameStartTime - m_StartTime;
				m_Snapshots.push_back(std::make_tuple(temp, m_FrameTime, std::move(m_CurrentSequence)));
				m_CurrentSequence.clear();

				break;
				
			}
		}


		// reverts back to default constructed state;
		void Reset()
		{
			m_Zones.clear();
			m_Stack.clear();
			m_Log.clear();

			m_FnList.clear();
			m_CurrentSequence.clear();
			m_Snapshots.clear();

			m_ZoneIndex = 0;
			m_TotalCallCount = 0;
			m_TotalFrameCount = 0;

			//m_LastCycleCount = getCPUCycle();
			//m_TotalCycleCount = 0;

			m_TotalTime = m_TimeElapsed = duration::zero();

			m_Zones.insert(std::make_pair("NONE", 0));
			m_Log.insert(std::make_pair(FunctionKey{ "NONE", "Root", __FILE__, 0 }, FunctionData{}));
			m_Stack.push_back(&(*m_Log.begin()).first);
			// emplace empty
			m_Snapshots.emplace_back();

			m_StartTime = getCurrentTime();
			m_LastTime = getCurrentTime();
		}

		void log()
		{
			for (auto& [key, data] : m_Log)
			{
				std::cout << key.m_FunctionName << std::endl;
				std::cout << key.m_Filename << ":" << key.m_LineNumber << std::endl;
				std::cout << "Zone: " << key.m_Zone << std::endl;
				std::cout << "Calls: " << data.m_Calls << std::endl;
				//std::cout << "Cycles: " << data.m_Cycles << std::endl;
				std::cout << "Duration: " << data.m_Duration.count() / 1000000.f << " ms" << std::endl;
				std::cout << "Percentage: " << 100.f * data.m_Duration.count() / m_TotalTime.count() << "%" << std::endl;
				std::cout << std::endl;
			}
			std::cout << "Total Function Calls: " << m_TotalCallCount << std::endl;
			std::cout << "Total Frames: " << m_TotalFrameCount << std::endl;
			//std::cout << "Total Cycles: " << m_TotalCycleCount << std::endl;
			std::cout << "Total Time: " << m_TotalTime.count() / 1000000.f << " ms" << std::endl;
			std::cout << std::endl;

		}

		inline const FrameSnapshot& getFrameSnapshot(size_t frame)
		{
			// assert 
			return m_Snapshots[frame];
		}
		inline const FunctionList& getFunctionList()
		{
			return m_FnList;
		}
		inline const FunctionData& getFunctionData(Function key)
		{
			// assert
			return m_Log[*key];
		}

		inline size_t getTotalCallCount()
		{
			return m_TotalCallCount;
		}

		inline size_t getTotalFrameCount()
		{
			return m_TotalFrameCount;
		}
		inline time_point getFrameStartTime()
		{
			return m_LastFrameTime;
		}
		inline duration getTotalTime()
		{
			return m_TotalTime;
		}

		inline duration getDeltaTime()
		{
			return m_FrameTime;
		}
		inline duration getTimeElapsed()
		{
			return m_TimeElapsed;
		}
		inline uint32_t getZoneColor(const std::string& name)
		{
			return color_array[m_Zones[name]];
		}

		inline bool isRecording()
		{
			return (m_State == State::RECORDING);
		}

		inline void startRecording()
		{
			m_RequestRecord = true;
		}

		inline void stopRecording()
		{
			m_RequestRecord = false;
		}

		inline bool onPlayStartRecording()
		{
			return m_RecordOnPlay;
		}

		inline void setPlayOnStart(bool set)
		{
			m_RecordOnPlay = set;
		}

		inline bool isHistoryDisabled()
		{
			return m_DisableHistory;
		}

		inline void setHistoryCapture(bool set)
		{
			m_DisableHistory = set;
		}

	private:
		ProfileZones m_Zones;
		PerformanceLog m_Log;
		PerformanceStack m_Stack;

		FunctionList m_FnList;
		FunctionSequence m_CurrentSequence;
		Snapshots m_Snapshots;

		size_t m_TotalCallCount = 0;
		size_t m_TotalFrameCount = 0;
		uint32_t m_ZoneIndex = 0;

		/*uint64_t m_LastCycleCount = 0;
		uint64_t m_TotalCycleCount = 0;*/

		time_point m_StartTime;
		duration m_TimeElapsed;

		time_point m_LastTime;
		duration m_TotalTime{ 0 };

		time_point m_LastFrameTime;
		duration m_FrameTime{ 0 };

		State m_State = State::STOPPED;
		bool m_RequestRecord = false;

		bool m_RecordOnPlay = false;
		bool m_DisableHistory = false;

	};

	static std::unique_ptr<ProfilerImplementation> s_Impl;

	FunctionKey::FunctionKey(
		const char* const zone,
		const char* const functionName,
		const char* const fileName,
		const unsigned int lineNumber
	) :
		m_Zone{ zone },
		m_FunctionName{ functionName },
		m_Filename{ fileName },
		m_LineNumber{ lineNumber } {}

	bool FunctionKey::operator==(const FunctionKey& rhs) const
	{
		if (m_LineNumber == rhs.m_LineNumber &&
			m_Filename == rhs.m_Filename)
		{
			return true;
		}
		return false;
	}

	SequenceData::SequenceData(const FunctionKey* key, const time_point& timePoint, bool openOrClose)
		: m_Key{ key }, m_TimePoint{ timePoint }, m_Open{ openOrClose } {}

	void Profiler::Init()
	{
		LOG("Profiler Online");
		s_Impl = std::make_unique<ProfilerImplementation>();
	}
	void Profiler::Shutdown()
	{
		s_Impl.reset();
	}
	void Profiler::ToLog()
	{
		s_Impl->log();
	}

	void Profiler::FrameStart()
	{
		// generate snapshot
		s_Impl->OnFrameStart();
	}

	void Profiler::OnPlay()
	{
		if (s_Impl->onPlayStartRecording())
		{
			s_Impl->startRecording();
		}
	}

	bool Profiler::IsStartOnPlay()
	{
		return s_Impl->onPlayStartRecording();
	}

	void Profiler::SetStartOnPlay(bool set)
	{
		s_Impl->setPlayOnStart(set);
	}

	bool Profiler::IsHistoryDisabled()
	{
		return s_Impl->isHistoryDisabled();

	}
	void Profiler::SetHistoryCapture(bool set)
	{
		s_Impl->setHistoryCapture(set);
	}

	bool Profiler::IsRecording()
	{
		return s_Impl->isRecording();
	}

	void Profiler::StartRecording()
	{
		s_Impl->startRecording();
	}

	void Profiler::StopRecording()
	{
		s_Impl->stopRecording();
	}

	// UI Functions

	const FrameSnapshot& Profiler::GetFrameSnapshot(size_t frame)
	{
		return s_Impl->getFrameSnapshot(frame);
	}
	const FunctionList& Profiler::GetFunctionList()
	{
		return s_Impl->getFunctionList();
	}
	const FunctionData& Profiler::GetFunctionData(Function key)
	{
		return s_Impl->getFunctionData(key);
	}


	size_t Profiler::GetTotalCallCount()
	{
		return s_Impl->getTotalCallCount();
	}
	size_t Profiler::GetTotalFrameCount()
	{
		return s_Impl->getTotalFrameCount();
	}
	time_point Profiler::GetFrameStartTime()
	{
		return s_Impl->getFrameStartTime();
	}
	duration Profiler::GetTotalTime()
	{
		return s_Impl->getTotalTime();
	}
	duration Profiler::GetDeltaTime()
	{
		return s_Impl->getDeltaTime();
	}
	duration Profiler::GetTimeElapsed()
	{
		return s_Impl->getTimeElapsed();
	}

	uint32_t Profiler::GetZoneColor(const std::string& zoneName)
	{
		return s_Impl->getZoneColor(zoneName);
	}

	ProfileThis::ProfileThis(
		const char* const zone,
		const char* const functionName,
		const char* const fileName,
		const unsigned int lineNumber) : m_OnStack{ true }
	{
		s_Impl->onEnter(zone, functionName, fileName, lineNumber);
	}

	ProfileThis::~ProfileThis()
	{
		End();
	}

	void ProfileThis::End()
	{
		if (m_OnStack)
		{
			s_Impl->onExit();
			m_OnStack = false;
		}
		
	}
}