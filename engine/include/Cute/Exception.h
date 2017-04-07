#pragma once
#include "Cute/Foundation.h"
#include <stdexcept>

CUTE_NS_BEGIN

class CUTE_CORE_API Exception : public std::exception
{
public:
	Exception(const String& msg, int code = 0);
	Exception(const String& msg, const String& arg, int code = 0);
	Exception(const String& msg, const Exception& nestedExc, int code = 0);
	Exception(const Exception& exc);
	~Exception() throw();
	Exception& operator = (const Exception& exc);

	virtual const char* name() const throw();
	virtual const char* className() const throw();
	virtual const char* what() const throw();
	const Exception* nested() const;
	const String& message() const;
	int code() const;
	String displayText() const;
	virtual Exception* clone() const;
	virtual void rethrow() const;

protected:
	Exception(int code = 0);
	void message(const String& msg);
	void extendedMessage(const String& arg);

private:
	String		m_msg;
	int			m_code;
	Exception*  m_nested;
};

//
// inlines
//
inline const Exception* Exception::nested() const
{
	return m_nested;
}

inline const String& Exception::message() const
{
	return m_msg;
}

inline void Exception::message(const String& msg)
{
	m_msg = msg;
}

inline int Exception::code() const
{
	return m_code;
}

//
// Macros for quickly declaring and implementing exception classes.
// Unfortunately, we cannot use a template here because character
// pointers (which we need for specifying the exception name)
// are not allowed as template arguments.
//
#define CUTE_DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE)							\
	class API CLS: public BASE														\
	{																				\
	public:																			\
		CLS(int code = CODE);														\
		CLS(const String& msg, int code = CODE);									\
		CLS(const String& msg, const String& arg, int code = CODE);					\
		CLS(const String& msg, const Cute::Exception& exc, int code = CODE);		\
		CLS(const CLS& exc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& exc);											\
		const char* name() const throw();											\
		const char* className() const throw();										\
		Cute::Exception* clone() const;												\
		void rethrow() const;														\
	};

#define CUTE_DECLARE_EXCEPTION(API, CLS, BASE) \
	CUTE_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)

#define CUTE_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int otherCode): BASE(otherCode)														\
	{																								\
	}																								\
	CLS::CLS(const String& msg, int otherCode): BASE(msg, otherCode)								\
	{																								\
	}																								\
	CLS::CLS(const String& msg, const String& arg, int otherCode): BASE(msg, arg, otherCode)		\
	{																								\
	}																								\
	CLS::CLS(const String& msg, const Cute::Exception& exc, int otherCode)							\
		: BASE(msg, exc, otherCode)																	\
	{																								\
	}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
	{																								\
	}																								\
	CLS::~CLS() throw()																				\
	{																								\
	}																								\
	CLS& CLS::operator = (const CLS& exc)															\
	{																								\
		BASE::operator = (exc);																		\
		return *this;																				\
	}																								\
	const char* CLS::name() const throw()															\
	{																								\
		return NAME;																				\
	}																								\
	const char* CLS::className() const throw()														\
	{																								\
		return typeid(*this).name();																\
	}																								\
	Cute::Exception* CLS::clone() const																\
	{																								\
		return new CLS(*this);																		\
	}																								\
	void CLS::rethrow() const																		\
	{																								\
		throw *this;																				\
	}

//
// Standard exception classes
//
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, LogicException, Exception)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, AssertionViolationException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NullPointerException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NullValueException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, BugcheckException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, InvalidArgumentException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NotImplementedException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, RangeException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, IllegalStateException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, InvalidAccessException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, SignalException, LogicException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, UnhandledException, LogicException)

CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, RuntimeException, Exception)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NotFoundException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ExistsException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, TimeoutException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, SystemException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, RegularExpressionException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, LibraryLoadException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, LibraryAlreadyLoadedException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NoThreadAvailableException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, PropertyNotSupportedException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, PoolOverflowException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NoPermissionException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, OutOfMemoryException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, DataException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, InterruptedException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, IndexOutOfBoundsException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, UnsupportedOperationException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, EmptyStackException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, StackOverflowException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ArithmeticException, RuntimeException)

CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, DataFormatException, DataException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, SyntaxException, DataException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, CircularReferenceException, DataException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, PathSyntaxException, SyntaxException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, IOException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ProtocolException, IOException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, FileException, IOException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, FileExistsException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, FileNotFoundException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, PathNotFoundException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, FileReadOnlyException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, FileAccessDeniedException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, CreateFileException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, OpenFileException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, WriteFileException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ReadFileException, FileException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, UnknownURISchemeException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, TooManyURIRedirectsException, RuntimeException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, URISyntaxException, SyntaxException)

CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ApplicationException, Exception)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, BadCastException, RuntimeException)

CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NetException, IOException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, InvalidAddressException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, InvalidSocketException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ServiceNotFoundException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ConnectionAbortedException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ConnectionResetException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ConnectionRefusedException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, DNSException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, HostNotFoundException, DNSException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NoAddressFoundException, DNSException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, InterfaceNotFoundException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NoMessageException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, MessageException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, MultipartException, MessageException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, HTTPException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NotAuthenticatedException, HTTPException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, UnsupportedRedirectException, HTTPException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, FTPException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, SMTPException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, POP3Exception, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, ICMPException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, NTPException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, HTMLFormException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, WebSocketException, NetException)
CUTE_DECLARE_EXCEPTION(CUTE_CORE_API, UnsupportedFamilyException, NetException)

CUTE_NS_END
