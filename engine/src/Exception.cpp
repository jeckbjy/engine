#include "Cute/Exception.h"

CUTE_NS_BEGIN

Exception::Exception(int otherCode) 
	: m_code(otherCode), m_nested(0)
{
}

Exception::Exception(const String& msg, int otherCode) 
	: m_msg(msg), m_code(otherCode), m_nested(0)
{
}

Exception::Exception(const String& msg, const String& arg, int otherCode) 
	: m_msg(msg), m_code(otherCode), m_nested(0)
{
	if (!arg.empty())
	{
		m_msg.append(": ");
		m_msg.append(arg);
	}
}

Exception::Exception(const String& msg, const Exception& nestedException, int otherCode) 
	: m_msg(msg), m_code(otherCode), m_nested(nestedException.clone())
{
}

Exception::Exception(const Exception& exc) 
	: std::exception(exc), m_msg(exc.m_msg), m_code(exc.m_code)
{
	m_nested = exc.m_nested ? exc.m_nested->clone() : 0;
}

Exception::~Exception() throw()
{
	delete m_nested;
}

Exception& Exception::operator = (const Exception& exc)
{
	if (&exc != this)
	{
		Exception* newPNested = exc.m_nested ? exc.m_nested->clone() : 0;
		delete m_nested;
		m_msg = exc.m_msg;
		m_nested = newPNested;
		m_code = exc.m_code;
	}
	return *this;
}

const char* Exception::name() const throw()
{
	return "Exception";
}

const char* Exception::className() const throw()
{
	return typeid(*this).name();
}

const char* Exception::what() const throw()
{
	return name();
}

String Exception::displayText() const
{
	String txt = name();
	if (!m_msg.empty())
	{
		txt.append(": ");
		txt.append(m_msg);
	}
	return txt;
}

void Exception::extendedMessage(const String& arg)
{
	if (!arg.empty())
	{
		if (!m_msg.empty()) m_msg.append(": ");
		m_msg.append(arg);
	}
}

Exception* Exception::clone() const
{
	return new Exception(*this);
}

void Exception::rethrow() const
{
	throw *this;
}

CUTE_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
CUTE_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
CUTE_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
CUTE_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
CUTE_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
CUTE_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
CUTE_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
CUTE_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
CUTE_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
CUTE_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
CUTE_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
CUTE_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

CUTE_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
CUTE_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
CUTE_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
CUTE_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
CUTE_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
CUTE_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
CUTE_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
CUTE_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
CUTE_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
CUTE_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
CUTE_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
CUTE_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
CUTE_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
CUTE_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

CUTE_IMPLEMENT_EXCEPTION(InterruptedException, RuntimeException, "Interrupted")
CUTE_IMPLEMENT_EXCEPTION(IndexOutOfBoundsException, RuntimeException, "Index out of bounds")
CUTE_IMPLEMENT_EXCEPTION(UnsupportedOperationException, RuntimeException, "Unsupported operation")
CUTE_IMPLEMENT_EXCEPTION(EmptyStackException, RuntimeException, "Empty stack")
CUTE_IMPLEMENT_EXCEPTION(StackOverflowException, RuntimeException, "Stack overflow")
CUTE_IMPLEMENT_EXCEPTION(ArithmeticException, RuntimeException, "Arithmetic error")

CUTE_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
CUTE_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
CUTE_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
CUTE_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
CUTE_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
CUTE_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
CUTE_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
CUTE_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
CUTE_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
CUTE_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
CUTE_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
CUTE_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
CUTE_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
CUTE_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
CUTE_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
CUTE_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
CUTE_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")
CUTE_IMPLEMENT_EXCEPTION(TooManyURIRedirectsException, RuntimeException, "Too many URI redirects")
CUTE_IMPLEMENT_EXCEPTION(URISyntaxException, SyntaxException, "Bad URI syntax")

CUTE_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
CUTE_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")

CUTE_IMPLEMENT_EXCEPTION(NetException, IOException, "Net Exception")
CUTE_IMPLEMENT_EXCEPTION(InvalidAddressException, NetException, "Invalid address")
CUTE_IMPLEMENT_EXCEPTION(InvalidSocketException, NetException, "Invalid socket")
CUTE_IMPLEMENT_EXCEPTION(ServiceNotFoundException, NetException, "Service not found")
CUTE_IMPLEMENT_EXCEPTION(ConnectionAbortedException, NetException, "Software caused connection abort")
CUTE_IMPLEMENT_EXCEPTION(ConnectionResetException, NetException, "Connection reset by peer")
CUTE_IMPLEMENT_EXCEPTION(ConnectionRefusedException, NetException, "Connection refused")
CUTE_IMPLEMENT_EXCEPTION(DNSException, NetException, "DNS error")
CUTE_IMPLEMENT_EXCEPTION(HostNotFoundException, DNSException, "Host not found")
CUTE_IMPLEMENT_EXCEPTION(NoAddressFoundException, DNSException, "No address found")
CUTE_IMPLEMENT_EXCEPTION(InterfaceNotFoundException, NetException, "Interface not found")
CUTE_IMPLEMENT_EXCEPTION(NoMessageException, NetException, "No message received")
CUTE_IMPLEMENT_EXCEPTION(MessageException, NetException, "Malformed message")
CUTE_IMPLEMENT_EXCEPTION(MultipartException, MessageException, "Malformed multipart message")
CUTE_IMPLEMENT_EXCEPTION(HTTPException, NetException, "HTTP Exception")
CUTE_IMPLEMENT_EXCEPTION(NotAuthenticatedException, HTTPException, "No authentication information found")
CUTE_IMPLEMENT_EXCEPTION(UnsupportedRedirectException, HTTPException, "Unsupported HTTP redirect (protocol change)")
CUTE_IMPLEMENT_EXCEPTION(FTPException, NetException, "FTP Exception")
CUTE_IMPLEMENT_EXCEPTION(SMTPException, NetException, "SMTP Exception")
CUTE_IMPLEMENT_EXCEPTION(POP3Exception, NetException, "POP3 Exception")
CUTE_IMPLEMENT_EXCEPTION(ICMPException, NetException, "ICMP Exception")
CUTE_IMPLEMENT_EXCEPTION(NTPException, NetException, "NTP Exception")
CUTE_IMPLEMENT_EXCEPTION(HTMLFormException, NetException, "HTML Form Exception")
CUTE_IMPLEMENT_EXCEPTION(WebSocketException, NetException, "WebSocket Exception")
CUTE_IMPLEMENT_EXCEPTION(UnsupportedFamilyException, NetException, "Unknown or unsupported socket family.")

CUTE_NS_END
