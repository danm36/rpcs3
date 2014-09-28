#pragma once
#include "Emu/SysCalls/lv2/sys_lwmutex.h"
#include "Emu/SysCalls/lv2/sys_lwcond.h"
#include "Emu/SysCalls/lv2/sys_spu.h"

// Core return codes.
enum
{
	CELL_SPURS_CORE_ERROR_AGAIN        = 0x80410701,
	CELL_SPURS_CORE_ERROR_INVAL        = 0x80410702,
	CELL_SPURS_CORE_ERROR_NOMEM        = 0x80410704,
	CELL_SPURS_CORE_ERROR_SRCH         = 0x80410705,
	CELL_SPURS_CORE_ERROR_PERM         = 0x80410709,
	CELL_SPURS_CORE_ERROR_BUSY         = 0x8041070A,
	CELL_SPURS_CORE_ERROR_STAT         = 0x8041070F,
	CELL_SPURS_CORE_ERROR_ALIGN        = 0x80410710,
	CELL_SPURS_CORE_ERROR_NULL_POINTER = 0x80410711,
};

//
enum
{
	CELL_SPURS_POLICY_MODULE_ERROR_AGAIN        = 0x80410801,
	CELL_SPURS_POLICY_MODULE_ERROR_INVAL        = 0x80410802,
	CELL_SPURS_POLICY_MODULE_ERROR_NOSYS        = 0x80410803,
	CELL_SPURS_POLICY_MODULE_ERROR_NOMEM        = 0x80410804,
	CELL_SPURS_POLICY_MODULE_ERROR_SRCH         = 0x80410805,
	CELL_SPURS_POLICY_MODULE_ERROR_NOENT        = 0x80410806,
	CELL_SPURS_POLICY_MODULE_ERROR_NOEXEC       = 0x80410807,
	CELL_SPURS_POLICY_MODULE_ERROR_DEADLK       = 0x80410808,
	CELL_SPURS_POLICY_MODULE_ERROR_PERM         = 0x80410809,
	CELL_SPURS_POLICY_MODULE_ERROR_BUSY         = 0x8041080A,
	CELL_SPURS_POLICY_MODULE_ERROR_ABORT        = 0x8041080C,
	CELL_SPURS_POLICY_MODULE_ERROR_FAULT        = 0x8041080D,
	CELL_SPURS_POLICY_MODULE_ERROR_CHILD        = 0x8041080E,
	CELL_SPURS_POLICY_MODULE_ERROR_STAT         = 0x8041080F,
	CELL_SPURS_POLICY_MODULE_ERROR_ALIGN        = 0x80410810,
	CELL_SPURS_POLICY_MODULE_ERROR_NULL_POINTER = 0x80410811,
};

// Task return codes.
enum
{
	CELL_SPURS_TASK_ERROR_AGAIN        = 0x80410901,
	CELL_SPURS_TASK_ERROR_INVAL        = 0x80410902,
	CELL_SPURS_TASK_ERROR_NOMEM        = 0x80410904,
	CELL_SPURS_TASK_ERROR_SRCH         = 0x80410905,
	CELL_SPURS_TASK_ERROR_NOEXEC       = 0x80410907,
	CELL_SPURS_TASK_ERROR_PERM         = 0x80410909,
	CELL_SPURS_TASK_ERROR_BUSY         = 0x8041090A,
	CELL_SPURS_TASK_ERROR_FAULT        = 0x8041090D,
	CELL_SPURS_TASK_ERROR_ALIGN        = 0x80410910,
	CELL_SPURS_TASK_ERROR_STAT         = 0x8041090F,
	CELL_SPURS_TASK_ERROR_NULL_POINTER = 0x80410911,
	CELL_SPURS_TASK_ERROR_FATAL        = 0x80410914,
	CELL_SPURS_TASK_ERROR_SHUTDOWN     = 0x80410920,
};

// SPURS defines.
enum SPURSKernelInterfaces
{
	CELL_SPURS_MAX_SPU = 8,
	CELL_SPURS_MAX_WORKLOAD = 16,
	CELL_SPURS_MAX_WORKLOAD2 = 32,
	CELL_SPURS_MAX_PRIORITY = 16,
	CELL_SPURS_NAME_MAX_LENGTH = 15,
	CELL_SPURS_SIZE = 4096,
	CELL_SPURS_SIZE2 = 8192,
	CELL_SPURS_ALIGN = 128,
	CELL_SPURS_ATTRIBUTE_SIZE = 512,
	CELL_SPURS_ATTRIBUTE_ALIGN = 8,
	CELL_SPURS_INTERRUPT_VECTOR = 0x0,
	CELL_SPURS_LOCK_LINE = 0x80,
	CELL_SPURS_KERNEL_DMA_TAG_ID = 31,
};

enum RangeofEventQueuePortNumbers
{
	CELL_SPURS_STATIC_PORT_RANGE_BOTTOM = 15,
	CELL_SPURS_DYNAMIC_PORT_RANGE_TOP = 16,
	CELL_SPURS_DYNAMIC_PORT_RANGE_BOTTOM = 63,
};

enum SPURSTraceTypes
{
	CELL_SPURS_TRACE_TAG_LOAD = 0x2a,
	CELL_SPURS_TRACE_TAG_MAP = 0x2b,
	CELL_SPURS_TRACE_TAG_START = 0x2c,
	CELL_SPURS_TRACE_TAG_STOP = 0x2d,
	CELL_SPURS_TRACE_TAG_USER = 0x2e,
	CELL_SPURS_TRACE_TAG_GUID = 0x2f,
};

// SPURS task defines.
enum TaskConstants
{
	CELL_SPURS_MAX_TASK = 128,
	CELL_SPURS_TASK_TOP = 0x3000,
	CELL_SPURS_TASK_BOTTOM = 0x40000,
	CELL_SPURS_MAX_TASK_NAME_LENGTH = 32,
};

class SPURSManager;
class SPURSManagerEventFlag;
class SPURSManagerTaskset;

struct CellSpurs;

enum SpursAttrFlags : u32
{
	SAF_NONE = 0x0,
	SAF_EXIT_IF_NO_WORK = 0x1,
	SAF_SECOND_VERSION = 0x4,

	SAF_UNKNOWN_FLAG_9                = 0x00400000,
	SAF_UNKNOWN_FLAG_8                = 0x00800000,
	SAF_UNKNOWN_FLAG_7                = 0x01000000,
	SAF_SYSTEM_WORKLOAD_ENABLED       = 0x02000000,
	SAF_SPU_PRINTF_ENABLED            = 0x10000000,
	SAF_SPU_TGT_EXCLUSIVE_NON_CONTEXT = 0x20000000,
	SAF_SPU_MEMORY_CONTAINER_SET      = 0x40000000,
	SAF_UNKNOWN_FLAG_0                = 0x80000000,
};

struct CellSpursAttribute
{
	static const uint align = 8;
	static const uint size = 512;

	union
	{
		// raw data
		u8 _u8[size];
		struct { be_t<u32> _u32[size / sizeof(u32)]; };

		// real data
		struct
		{
			be_t<u32> revision;    // 0x0
			be_t<u32> sdkVersion;  // 0x4
			be_t<u32> nSpus;       // 0x8
			be_t<s32> spuPriority; // 0xC
			be_t<s32> ppuPriority; // 0x10
			bool exitIfNoWork;     // 0x14
			char prefix[15];       // 0x15 (not a NTS)
			be_t<u32> prefixSize;  // 0x24
			be_t<u32> flags;       // 0x28 (SpursAttrFlags)
			be_t<u32> container;   // 0x2C
			be_t<u32> unk0;        // 0x30
			be_t<u32> unk1;        // 0x34
			u8 swlPriority[8];     // 0x38
			be_t<u32> swlMaxSpu;   // 0x40
			be_t<u32> swlIsPreem;  // 0x44
		} m;

		// alternative implementation
		struct
		{
		} c;
	};
};

typedef void(*CellSpursShutdownCompletionEventHook)(vm::ptr<CellSpurs>, u32 wid, vm::ptr<void> arg);

// Core CellSpurs structures
struct CellSpurs
{
	static const uint align = 128;
	static const uint size = 0x2000; // size of CellSpurs2
	static const uint size1 = 0x1000; // size of CellSpurs
	static const uint size2 = 0x1000;

	struct _sub_str1
	{
		static const uint size = 0x80;

		u8 unk0[0x20];
		be_t<u64> sem; // 0x20
		u8 unk1[0x8];
		vm::bptr<CellSpursShutdownCompletionEventHook, 1, u64> hook; // 0x30
		vm::bptr<void, 1, u64> hookArg; // 0x38
		u8 unk2[0x40];
	};

	struct _sub_str2
	{
		static const uint size = 0x80;

		be_t<u32> unk0;
		be_t<u32> unk1;
		be_t<u32> unk2;
		be_t<u32> unk3;
		be_t<u64> port; // 0x10
		u8 unk_[0x68];
	};

	struct _sub_str3
	{
		static const uint size = 0x20;

		vm::bptr<const void, 1, u64> wklPm; // policy module
		be_t<u64> wklArg; // spu argument
		be_t<u32> wklSize;
		atomic_t<u8> wklCopy;
		be_t<u64> wklPriority;
	};

	struct _sub_str4
	{
		static const uint size = 0x10;

		vm::bptr<const char, 1, u64> nameClass;
		vm::bptr<const char, 1, u64> nameInstance;
	};

	struct _sub_x70
	{
		be_t<u16> wklFlag1; // 0x70
		u8 unk2; // 0x72
		u8 unk3;
		u8 flags1;
		u8 unk5;
		u8 nSpus;
		u8 unk7;
	};

	struct _sub_x78
	{
		be_t<u16> wklFlag2;
		u8 unk[6];
	};

	union
	{
		// raw data
		u8 _u8[size];
		std::array<be_t<u32>, size / sizeof(u32)> _u32;

		// real data
		struct
		{
			u8 wklZ1[0x10];       // 0x0
			u8 wklZ2[0x10];       // 0x10
			u8 wklA[0x10];        // 0x20
			u8 wklB[0x10];        // 0x30
			u8 wklMinCnt[0x10];   // 0x40
			atomic_t<u32> wklMaxCnt[4]; // 0x50
			u8 unknown0[0x6C - 0x60];
			be_t<u32> unk18;      // 0x6C
			atomic_t<_sub_x70> x70; // 0x70
			atomic_t<_sub_x78> x78; // 0x78
			u8 wklC1[0x10];       // 0x80
			u8 wklD1[0x10];       // 0x90
			u8 wklE1[0x10];       // 0xA0
			atomic_t<u32> wklMsk1;// 0xB0
			atomic_t<u32> wklMsk2;// 0xB4
			atomic_t<u8> unk23[8];// 0xB8
			u8 unk6[0x10];        // 0xC0 (SPU port at 0xc9)
			u8 wklC2[0x10];       // 0xD0
			u8 wklD2[0x10];       // 0xE0
			u8 wklE2[0x10];       // 0xF0
			_sub_str1 wklF1[0x10];// 0x100
			be_t<u64> unk22;      // 0x900
			u8 unknown7[0x980 - 0x908];
			be_t<u64> semPrv;     // 0x980
			be_t<u32> unk11;      // 0x988
			be_t<u32> unk12;      // 0x98C
			be_t<u64> unk13;      // 0x990
			u8 unknown4[0xB00 - 0x998];
			_sub_str3 wklG1[0x10];// 0xB00
			be_t<u64> unk7;       // 0xD00
			be_t<u64> unk8;       // 0xD08
			be_t<u32> unk9;       // 0xD10
			u8 unk10;             // 0xD14
			u8 unknown5[0xD20 - 0xD15];
			be_t<u64> ppu0;       // 0xD20
			be_t<u64> ppu1;       // 0xD28
			be_t<u32> spuTG;      // 0xD30
			be_t<u32> spus[8];    // 0xD34
			u8 unknown3[0xD5C - 0xD54];
			be_t<u32> queue;      // 0xD5C
			be_t<u32> port;       // 0xD60
			atomic_t<u8> unk19[4]; // 0xD64 (used in wakeup)
			atomic_t<u32> enableEH; // 0xD68
			be_t<u32> unk21;      // 0xD6C
			sys_spu_image spuImg; // 0xD70
			be_t<u32> flags;      // 0xD80
			be_t<s32> spuPriority;// 0xD84
			be_t<u32> ppuPriority;// 0xD88
			char prefix[0x0f];    // 0xD8C
			u8 prefixSize;        // 0xD9B
			be_t<u32> unk5;       // 0xD9C
			be_t<u32> revision;   // 0xDA0
			be_t<u32> sdkVersion; // 0xDA4
			atomic_t<u64> spups;// 0xDA8
			sys_lwmutex_t mutex;  // 0xDB0
			sys_lwcond_t cond;    // 0xDC8
			u8 unknown9[0xE00 - 0xDD0];
			_sub_str4 wklH1[0x10];// 0xE00
			_sub_str2 sub3;       // 0xF00
			u8 unknown6[0x1000 - 0xF80];
			_sub_str3 wklG2[0x10];// 0x1000
			_sub_str1 wklF2[0x10];// 0x1200
			_sub_str4 wklH2[0x10];// 0x1A00
		} m;

		// alternative implementation
		struct
		{
			SPURSManager *spurs;
		} c;
	};
};

typedef CellSpurs CellSpurs2;

struct CellSpursWorkloadAttribute
{
	static const uint align = 8;
	static const uint size = 512;

	union
	{
		// raw data
		u8 _u8[size];

		// real data
		struct
		{
			be_t<u32> revision;
			be_t<u32> sdkVersion;
			vm::bptr<const void> pm;
			be_t<u32> size;
			be_t<u64> data;
			u8 priority[8];
			be_t<u32> minContention;
			be_t<u32> maxContention;
			vm::bptr<const char> nameClass;
			vm::bptr<const char> nameInstance;
			vm::bptr<CellSpursShutdownCompletionEventHook> hook;
			vm::bptr<void> hookArg;
		} m;
	};
};

struct CellSpursEventFlag
{
	SPURSManagerEventFlag *eventFlag;
};

struct CellSpursTaskset
{
	SPURSManagerTaskset *taskset;
};

struct CellSpursInfo
{
	be_t<s32> nSpus;
	be_t<s32> spuThreadGroupPriority;
	be_t<s32> ppuThreadPriority;
	bool exitIfNoWork;
	bool spurs2;
	be_t<u32> traceBuffer_addr;     //void *traceBuffer;
	be_t<u64> traceBufferSize;
	be_t<u32> traceMode;
	be_t<u32> spuThreadGroup;       //typedef u32 sys_spu_thread_group_t;
	be_t<u32> spuThreads[8];        //typedef u32 sys_spu_thread_t;
	be_t<u32> spursHandlerThread0;
	be_t<u32> spursHandlerThread1;
	s8 namePrefix[CELL_SPURS_NAME_MAX_LENGTH+1];
	be_t<u64> namePrefixLength;
	be_t<u32> deadlineMissCounter;
	be_t<u32> deadlineMeetCounter;
	//u8 padding[];
};

struct CellSpursExceptionInfo
{
	be_t<u32> spu_thread;
	be_t<u32> spu_npc;
	be_t<u32> cause;
	be_t<u64> option;
};

struct CellSpursTraceInfo
{
	be_t<u32> spu_thread[8];
	be_t<u32> count[8];
	be_t<u32> spu_thread_grp;
	be_t<u32> nspu;
	//u8 padding[];
};

struct CellTraceHeader
{
	u8 tag;
	u8 length;
	u8 cpu;
	u8 thread;
	be_t<u32> time;
};

struct CellSpursTracePacket
{
	struct header_struct
	{
		u8 tag;
		u8 length;
		u8 spu;
		u8 workload;
		be_t<u32> time;
	} header;

	struct data_struct
	{
		struct load_struct
		{
			be_t<u32> ea;
			be_t<u16> ls;
			be_t<u16> size;
		} load;

		struct map_struct
		{
			be_t<u32> offset;
			be_t<u16> ls;
			be_t<u16> size;
		} map;

		struct start_struct
		{
			s8 module[4];
			be_t<u16> level;
			be_t<u16> ls;
		} start;

		be_t<u64> user;
		be_t<u64> guid;
	} data;
};

// Exception handlers.
//typedef void (*CellSpursGlobalExceptionEventHandler)(vm::ptr<CellSpurs> spurs, vm::ptr<const CellSpursExceptionInfo> info, 
//													   u32 id, vm::ptr<void> arg);
//
//typedef void (*CellSpursTasksetExceptionEventHandler)(vm::ptr<CellSpurs> spurs, vm::ptr<CellSpursTaskset> taskset, 
//													    u32 idTask, vm::ptr<const CellSpursExceptionInfo> info, vm::ptr<void> arg);

struct CellSpursTasksetInfo
{
	//CellSpursTaskInfo taskInfo[CELL_SPURS_MAX_TASK];
	be_t<u64> argument;
	be_t<u32> idWorkload;
	be_t<u32> idLastScheduledTask; //typedef unsigned CellSpursTaskId
	be_t<u32> name_addr;
	be_t<u32> exceptionEventHandler_addr;
	be_t<u32> exceptionEventHandlerArgument_addr; //void *exceptionEventHandlerArgument
	be_t<u64> sizeTaskset;
	//be_t<u8> reserved[];
};

struct CellSpursTaskset2
{
	be_t<u8> skip[10496];
};

struct CellSpursTasksetAttribute2
{
	be_t<u32> revision;
	be_t<u32> name_addr;
	be_t<u64> argTaskset;
	u8 priority[8];
	be_t<u32> maxContention;
	be_t<s32> enableClearLs;
	be_t<s32> CellSpursTaskNameBuffer_addr; //??? *taskNameBuffer
	//be_t<u32> __reserved__[];
};

// cellSpurs task structures.
struct CellSpursTaskNameBuffer
{
	char taskName[CELL_SPURS_MAX_TASK][CELL_SPURS_MAX_TASK_NAME_LENGTH];
};

struct CellSpursTraceTaskData
{
	be_t<u32> incident;
	be_t<u32> task;
};

typedef be_t<u32> be_u32;
typedef be_t<u64> be_u64;

struct CellSpursTaskArgument
{
	be_u32 u32[4];
	be_u64 u64[2];
};

struct CellSpursTaskLsPattern
{
	be_u32 u32[4];
	be_u64 u64[2];
};

struct CellSpursTaskAttribute2
{
	be_t<u32> revision;
	be_t<u32> sizeContext;
	be_t<u64> eaContext;
	CellSpursTaskLsPattern lsPattern;
	be_t<u32> name_addr;
	//be_t<u32> __reserved__[];
};

struct CellSpursTaskExitCode
{
	unsigned char skip[128];
};

struct CellSpursTaskInfo
{
	CellSpursTaskLsPattern lsPattern;
	CellSpursTaskArgument argument;
	const be_t<u32> eaElf_addr; //void *eaElf
	const be_t<u32> eaContext_addr; //void *eaContext
	be_t<u32> sizeContext;
	be_t<u8> state;
	be_t<u8> hasSignal;
	const be_t<u32> CellSpursTaskExitCode_addr;
	u8 guid[8];
	//be_t<u8> reserved[];
};

struct CellSpursTaskBinInfo
{
	be_t<u64> eaElf;
	be_t<u32> sizeContext;
	be_t<u32> __reserved__;
	CellSpursTaskLsPattern lsPattern;
};

s64 spursAttachLv2EventQueue(vm::ptr<CellSpurs> spurs, u32 queue, vm::ptr<u8> port, s32 isDynamic, bool wasCreated);
s64 spursWakeUp(vm::ptr<CellSpurs> spurs);
