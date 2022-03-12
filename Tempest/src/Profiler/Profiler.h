#pragma once
#include <chrono>
#include <string>
#include <vector>

//#ifdef _DEBUG
#define TO_STRING(x) #x
#define PROFILER_MARKER(zone) Tempest::Profile::ProfileThis marker(TO_STRING(zone), __FUNCTION__, __FILE__, __LINE__)
#define NAMED_PROFILER_MARKER(x, zone) Tempest::Profile::ProfileThis x(TO_STRING(zone), TO_STRING(x), __FILE__, __LINE__)
#define END_NAMED_MARKER(x) x.End()
//#else
//#define TO_STRING(x) 
//#define PROFILER_MARKER(zone) 
//#define NAMED_PROFILER_MARKER(x, zone) 
//#define END_NAMED_MARKER(x) 
//#endif


namespace Tempest::Profile
{
	using current_clock = std::chrono::high_resolution_clock;
	using time_point = std::chrono::time_point<current_clock>;
	using duration = std::chrono::nanoseconds;

	struct FunctionKey
	{
		FunctionKey(
			const char* const zone,
			const char* const functionName,
			const char* const fileName,
			const unsigned int lineNumber
		);

		bool operator==(const FunctionKey& rhs) const;

		const std::string m_Zone;
		const std::string m_FunctionName;
		const std::string m_Filename;
		const size_t m_LineNumber;
	};

	struct FunctionData
	{
		size_t m_Calls = 0;
		//uint64_t m_Cycles = 0;
		duration m_Duration{ 0 };
	};

	struct SequenceData
	{
		SequenceData(const FunctionKey* key, const time_point& timePoint, bool openOrClose);

		const FunctionKey* m_Key;
		time_point m_TimePoint;
		bool m_Open;
	};
	using Function = const FunctionKey*;
	using FunctionSequence = std::vector<SequenceData>;
	using FrameSnapshot = std::tuple<time_point, duration, FunctionSequence>;
	using FunctionList = std::vector<Function>;

	class Profiler
	{
	public:
		static void Init();
		static void Shutdown();

		static void ToLog();
		static void FrameStart();

		static void OnPlay();

		bool IsRecording();

		void StartRecording();
		void StopRecording();

		bool IsStartOnPlay();
		void SetStartOnPlay(bool);

		bool IsHistoryDisabled();
		void SetHistoryCapture(bool);

		// UI Functions
		const FrameSnapshot& GetFrameSnapshot(size_t frame);
		const FunctionList& GetFunctionList();
		const FunctionData& GetFunctionData(Function key);

		size_t GetTotalCallCount();
		size_t GetTotalFrameCount();
		time_point GetFrameStartTime();
		duration GetTotalTime();
		duration GetDeltaTime();
		duration GetTimeElapsed();
		uint32_t GetZoneColor(const std::string& zoneName);
	};

	class ProfileThis
	{
	public:
		ProfileThis(
			const char* const zone,
			const char* const functionName,
			const char* const fileName,
			const unsigned int lineNumber);
		~ProfileThis();

		void End();

	private:
		bool m_OnStack;
	};
}
