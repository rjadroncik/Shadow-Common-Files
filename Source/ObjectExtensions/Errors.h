#pragma once

#ifdef WIN32

#ifdef OBJECT_EXTENSIONS_EXPORTS
#define OBJECT_EXTENSIONS_API __declspec(dllexport)
#else
#define OBJECT_EXTENSIONS_API __declspec(dllimport)
#endif

#else

#define OBJECT_EXTENSIONS_API

#endif // WIN32

#include <SCFStandard.h>

//Reserved error codes: 0x0001 - 0x0100

namespace SCFBase
{
	enum Errors
	{
		//Generic Errors, should not be used as they are not very descriptive
		ErrorIllegalOperation = 0x0001,
		ErrorInvalidArgument,
		ErrorAccessDenied,
		ErrorBufferOverrun,
		ErrorUnexpectedCodeExecution,
		ErrorOutOfMemory,

		//File Errors
		ErrorFileNotFound,
		ErrorFileReadingPastEndOfFile,
		ErrorFileInvalidFilePosition,
		ErrorFileFailedOpen,
		ErrorFileFailedClose,
		ErrorFileFailedDelete,
		ErrorFileFailedWrite,
		ErrorFileFailedRead,
		ErrorFileFailedErase,
		ErrorFileFailedCreate,
		ErrorFileFailedSizeSet,
		ErrorFileFailedSizeGet,
		ErrorFileFailedRename,
		ErrorFileFailedMove,
		ErrorFileFailedCopy,
		ErrorFileFailedWritableSet,
		ErrorFileFailedWritableGet,

		//Directory Errors
		ErrorDirectoryNotFound,
		ErrorDirectoryFailedDelete,
		ErrorDirectoryFailedCreate,
		ErrorDirectoryFailedErase,
		ErrorDirectoryFailedRename,
		ErrorDirectoryFailedMove,
		ErrorDirectoryFailedCopy,
		ErrorDirectoryFailedSizeGet,
		ErrorDirectoryFailedWritableSet,
		ErrorDirectoryFailedWritableGet,

		//Object Errors
		ErrorObjectFailedCreation,
		ErrorObjectFailedConstructorRegistration,
		ErrorObjectFailedConstructorQuery,
		ErrorSerializationNotSupported,

		//Streams
		ErrorStreamTypeMismatch,
	};

	#ifdef _BETA
	#define SCFError(errorCode) CError::Last(errorCode, SCFTEXT(__FUNCSIG__))
	#else
	#define SCFError(errorCode) CError::Last(errorCode)
	#endif

	class OBJECT_EXTENSIONS_API CString;
	class OBJECT_EXTENSIONS_API CDictionaryInt64;
	class OBJECT_EXTENSIONS_API IStreamWrite;
	class OBJECT_EXTENSIONS_API IStreamWriteText;
	class OBJECT_EXTENSIONS_API CStreamWriteTextUCS2;
	class OBJECT_EXTENSIONS_API CMemoryBlock;
	class OBJECT_EXTENSIONS_API CStreamMemoryWrite;

	class OBJECT_EXTENSIONS_API CError
	{
	public:
		//These get/set last error value
		static ENUM Last();
		static void Last(_IN ENUM eError);
		static void Last(_IN ENUM eError, _IN CString& rFuncSig);

	public:
		//Returns a pointer to the error-string of the last error-code, if such a string
		//exists (do NOT delete the returned object)
		static CString* LastString();

	public:
		//Resets the last error value
		static void Reset();

	public:
		//These handle the error-code -> error-string mappings
		static CString* String(_IN ENUM eError);
		static void     String(_IN ENUM eError, _IN _REF CString& rString);

	public:
		//The default storage is a memory block, which u can query
		//& clear or write to file or whatever :)
		static CMemoryBlock* DefaultStorage();

	public:
		//If u want to store error reports somewhere else than the default storage
		//(i.e. a file) then u can specify a new write stream
		static void              Stream(_INOUT _REF IStreamWriteText* pStream);
		static IStreamWriteText* Stream();

	private:
		CError();
		~CError();
	};
};


void SCFErrorInitialize();
void SCFErrorCleanUp();

