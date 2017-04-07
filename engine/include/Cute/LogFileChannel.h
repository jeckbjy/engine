#pragma once
#include "Cute/LogChannel.h"
#include "Cute/LogFile.h"
#include "Cute/FileStream.h"
#include "Cute/Timestamp.h"
#include "Cute/Timespan.h"
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

class RotateStrategy;
class PurgeStrategy;
class ArchiveStrategy;

/// A Channel that writes to a file. This class supports
/// flexible log file rotation and archiving, as well
/// as automatic purging of archived log files.
///
/// Only the message's text is written, followed
/// by a newline.
///
/// Chain this channel to a FormattingChannel with an
/// appropriate Formatter to control what is in the text. 
///
/// The FileChannel support log file rotation based
/// on log file size or time intervals.
/// Archived log files can be compressed in gzip format.
/// Older archived files can be automatically deleted
/// (purged).
///
/// The rotation strategy can be specified with the
/// "rotation" property, which can take one of the
/// following values:
///
///   * never:         no log rotation
///   * [day,][hh]:mm: the file is rotated on specified day/time
///                    day - day is specified as long or short day name (Monday|Mon, Tuesday|Tue, ... );
///                          day can be omitted, in which case log is rotated every day
///                    hh  - valid hour range is 00-23;
///                          hour can be omitted, in which case log is rotated every hour
///                    mm  - valid minute range is 00-59;
///                          minute must be specified
///   * daily:         the file is rotated daily
///   * weekly:        the file is rotated every seven days
///   * monthly:       the file is rotated every 30 days
///   * <n> minutes:   the file is rotated every <n> minutes, 
///                    where <n> is an integer greater than zero.
///   * <n> hours:     the file is rotated every <n> hours, where
///                    <n> is an integer greater than zero.
///   * <n> days:      the file is rotated every <n> days, where
///                    <n> is an integer greater than zero.
///   * <n> weeks:     the file is rotated every <n> weeks, where
///                    <n> is an integer greater than zero.
///   * <n> months:    the file is rotated every <n> months, where
///                    <n> is an integer greater than zero and
///                    a month has 30 days.
///   * <n>:           the file is rotated when its size exceeds
///                    <n> bytes.
///   * <n> K:         the file is rotated when its size exceeds
///                    <n> Kilobytes.
///   * <n> M:         the file is rotated when its size exceeds
///                    <n> Megabytes.
///
/// NOTE: For periodic log file rotation (daily, weekly, monthly, etc.),
/// the date and time of log file creation or last rotation is
/// written into the first line of the log file. This is because
/// there is no reliable way to find out the real creation date of
/// a file on many platforms (e.g., most Unix platforms do not
/// provide the creation date, and Windows has its own issues
/// with its "File System Tunneling Capabilities").
///
/// Using the "archive" property it is possible to specify
/// how archived log files are named. The following values
/// for the "archive" property are supported:
///
///   * number:     A number, starting with 0, is appended to
///                 the name of archived log files. The newest
///                 archived log file always has the number 0.
///                 For example, if the log file is named
///                 "access.log", and it fulfils the criteria
///                 for rotation, the file is renamed to
///                 "access.log.0". If a file named "access.log.0"
///                 already exists, it is renamed to "access.log.1",
///                 and so on.
///   * timestamp:  A timestamp is appended to the log file name.
///                 For example, if the log file is named
///                 "access.log", and it fulfils the criteria
///                 for rotation, the file is renamed to
///                 "access.log.20050802110300".
///
/// Using the "times" property it is possible to specify
/// time mode for the day/time based rotation. The following values
/// for the "times" property are supported:
///
///   * utc:        Rotation strategy is based on UTC time (default).
///   * local:      Rotation strategy is based on local time.
///
/// Archived log files can be compressed using the gzip compression
/// method. Compressing can be controlled with the "compress"
/// property. The following values for the "compress" property
/// are supported:
///
///   * true:       Compress archived log files.
///   * false:      Do not compress archived log files.
///
/// Archived log files can be automatically purged, either if
/// they reach a certain age, or if the number of archived
/// log files reaches a given maximum number. This is 
/// controlled by the purgeAge and purgeCount properties.
///
/// The purgeAge property can have the following values:
///
///   * <n> [seconds]: the maximum age is <n> seconds.
///   * <n> minutes:   the maximum age is <n> minutes.
///   * <n> hours:     the maximum age is <n> hours.
///   * <n> days:      the maximum age is <n> days.
///   * <n> weeks:     the maximum age is <n> weeks.
///   * <n> months:    the maximum age is <n> months, where a month has 30 days.
///
/// The purgeCount property has an integer value that specifies the maximum number
/// of archived log files. If the number is exceeded, archived log files are
/// deleted, starting with the oldest. When "none" or empty string are
/// supplied, they reset purgeCount to none (no purging).
///
/// The flush property specifies whether each log message is flushed
/// immediately to the log file (which may hurt application performance,
/// but ensures that everything is in the log in case of a system crash),
//  or whether it's allowed to stay in the system's file buffer for some time. 
/// Valid values are:
///
///   * true:  Every message is immediately flushed to the log file (default).
///   * false: Messages are not immediately flushed to the log file.
///
/// The rotateOnOpen property specifies whether an existing log file should be 
/// rotated (and archived) when the channel is opened. Valid values are:
///
///   * true:  The log file is rotated (and archived) when the channel is opened.
///   * false: Log messages will be appended to an existing log file,
///            if it exists (unless other conditions for a rotation are met). 
///            This is the default.
///
/// For a more lightweight file channel class, see SimpleFileChannel.
class CUTE_CORE_API LogFileChannel : public LogChannel
{
public:
	static const std::string PROP_PATH;
	static const std::string PROP_ROTATION;
	static const std::string PROP_ARCHIVE;
	static const std::string PROP_TIMES;
	static const std::string PROP_COMPRESS;
	static const std::string PROP_PURGEAGE;
	static const std::string PROP_PURGECOUNT;
	static const std::string PROP_FLUSH;
	static const std::string PROP_ROTATEONOPEN;

public:
	LogFileChannel(const String& path = "");
	~LogFileChannel();

	void open();
	void close();
	void log(const LogMessage& msg);
	void setProperty(const String& name, const String& value);
	String getProperty(const String& name) const;

	Timestamp creationDate() const;
	uint64_t size() const;
	const String& path() const;

protected:
	void setRotation(const std::string& rotation);
	void setArchive(const std::string& archive);
	void setCompress(const std::string& compress);
	void setPurgeAge(const std::string& age);
	void setPurgeCount(const std::string& count);
	void setFlush(const std::string& flush);
	void setRotateOnOpen(const std::string& rotateOnOpen);
	void purge();

private:
	bool	 setNoPurge(const std::string& value);
	int		 extractDigit(const std::string& value, std::string::const_iterator* nextToDigit = NULL) const;
	void	 setPurgeStrategy(PurgeStrategy* strategy);
	uint64_t extractFactor(const std::string& value, std::string::const_iterator start) const;

private:
	String			m_path;
	String			m_times;
	String			m_archive;
	String			m_purgeAge;
	String			m_purgeCount;
	String			m_rotation;
	bool			m_rotateOnOpen;
	bool			m_compress;
	bool			m_flush;
	LogFile*		m_file;
	RotateStrategy* m_rotateStrategy;
	PurgeStrategy*	m_purgeStrategy;
	ArchiveStrategy* m_archiveStrategy;
	Mutex			m_mutex;
};

CUTE_NS_END