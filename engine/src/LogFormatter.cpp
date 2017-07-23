//! Logging
#include "Cute/LogFormatter.h"
#include "Cute/Number.h"
#include "Cute/DateTime.h"
#include "Cute/Timestamp.h"
#include "Cute/Timezone.h"
#include "Cute/Environment.h"
#include "Cute/StringTokenizer.h"

CUTE_NS_BEGIN

LogFormatter::LogFormatter()
	: m_localTime(false)
{
	parsePriorityNames();
}

LogFormatter::LogFormatter(const String& pattern)
	: m_localTime(false)
	, m_pattern(pattern)
{
	parsePriorityNames();
	parsePattern();
}

LogFormatter::~LogFormatter()
{

}

void LogFormatter::format(const LogMessage& msg)
{
	String text;
	Timestamp timestamp = msg.getTime();
	bool localTime = m_localTime;
	if (localTime)
	{
		timestamp += Timezone::utcOffset()*Timestamp::resolution();
		timestamp += Timezone::dst()*Timestamp::resolution();
	}
	DateTime dateTime = timestamp;
	for (Vector<PatternAction>::iterator ip = m_actions.begin(); ip != m_actions.end(); ++ip)
	{
		text.append(ip->prepend);
		switch (ip->key)
		{
		case 's': text.append(msg.getSource()); break;
		case 't': text.append(msg.getText()); break;
		case 'l': text.appends((int)msg.getLevel()); break;
		case 'p': text.append(getPriorityName((int)msg.getLevel())); break;
		case 'q': text += getPriorityName((int)msg.getLevel()).at(0); break;
		case 'P': text.appends((int)msg.getPid()); break;
		case 'T': text.append(msg.getThread()); break;
		case 'I': text.appends(msg.getTid()); break;
		case 'O': text.appends(msg.getTid()); break;
		case 'N': text.append(Environment::nodeName()); break;
		case 'U': text.append(msg.getFile() ? msg.getFile() : ""); break;
		case 'u': text.appends(msg.getLine()); break;
		case 'w': text.append(DateTime::WEEKDAY_NAMES[dateTime.dayOfWeek()], 0, 3); break;
		case 'W': text.append(DateTime::WEEKDAY_NAMES[dateTime.dayOfWeek()]); break;
		case 'b': text.append(DateTime::MONTH_NAMES[dateTime.month() - 1], 0, 3); break;
		case 'B': text.append(DateTime::MONTH_NAMES[dateTime.month() - 1]); break;
		case 'd': text.append0(dateTime.day(), 2); break;
		case 'e': text.appends(dateTime.day()); break;
		case 'f': text.appends(dateTime.day(), 2); break;
		case 'm': text.append0(dateTime.month(), 2); break;
		case 'n': text.appends(dateTime.month()); break;
		case 'o': text.appends(dateTime.month(), 2); break;
		case 'y': text.append0(dateTime.year() % 100, 2); break;
		case 'Y': text.append0(dateTime.year(), 4); break;
		case 'H': text.append0(dateTime.hour(), 2); break;
		case 'h': text.append0(dateTime.hourAMPM(), 2); break;
		case 'a': text.append(dateTime.isAM() ? "am" : "pm"); break;
		case 'A': text.append(dateTime.isAM() ? "AM" : "PM"); break;
		case 'M': text.append0(dateTime.minute(), 2); break;
		case 'S': text.append0(dateTime.second(), 2); break;
		case 'i': text.append0(dateTime.millisecond(), 3); break;
		case 'c': text.appends(dateTime.millisecond() / 100); break;
		case 'F': text.append0(dateTime.millisecond() * 1000 + dateTime.microsecond(), 6); break;
        case 'z': DateTime::tzdISO(text, localTime ? Timezone::tzd() : DateTime::UTC_ZONE_DIFF); break;
		case 'Z': DateTime::tzdRFC(text, localTime ? Timezone::tzd() : DateTime::UTC_ZONE_DIFF); break;
		case 'E': text.appends((int64)msg.getTime().epochTime()); break;
		case 'v':
			if (ip->length > msg.getSource().length())	//append spaces
				text.append(msg.getSource()).append(ip->length - msg.getSource().length(), ' ');
			else if (ip->length && ip->length < msg.getSource().length()) // crop
				text.append(msg.getSource(), msg.getSource().length() - ip->length, ip->length);
			else
				text.append(msg.getSource());
			break;
		case 'x':
			//try
			//{
			//	// donot support!!!
			//	//text.append(msg[ip->property]);
			//}
			//catch (...)
			//{
			//}
			//break;
			break;
		case 'L':
			if (!localTime)
			{
				localTime = true;
				timestamp += Timezone::utcOffset()*Timestamp::resolution();
				timestamp += Timezone::dst()*Timestamp::resolution();
				dateTime = timestamp;
			}
			break;
		}
	}

	msg.setMessage(text);
}

void LogFormatter::parsePattern()
{
	m_actions.clear();
	String::ConstIterator it = m_pattern.begin();
	String::ConstIterator end = m_pattern.end();
	PatternAction endAct;
	while (it != end)
	{
		if (*it == '%')
		{
			if (++it != end)
			{
				PatternAction act;
				act.prepend = endAct.prepend;
				endAct.prepend.clear();

				if (*it == '[')
				{
					act.key = 'x';
					++it;
					String prop;
					while (it != end && *it != ']') prop += *it++;
					if (it == end) --it;
					act.property = prop;
				}
				else
				{
					act.key = *it;
					if ((it + 1) != end && *(it + 1) == '[')
					{
						it += 2;
						String number;
						while (it != end && *it != ']') number += *it++;
						if (it == end) --it;
						number.parse(act.length);
					}
				}
				m_actions.push_back(act);
				++it;
			}
		}
		else
		{
			endAct.prepend += *it++;
		}
	}
	if (endAct.prepend.size())
	{
		m_actions.push_back(endAct);
	}
}

void LogFormatter::setPattern(const String& pattern)
{
	m_pattern = pattern;
	parsePattern();
}

const String& LogFormatter::getPattern() const
{
	return m_pattern;
}

void LogFormatter::setLocal(bool value)
{
	m_localTime = value;
}

namespace
{
	static String priorities[] =
	{
		"",
		"Fatal",
		"Critical",
		"Error",
		"Warning",
		"Notice",
		"Information",
		"Debug",
		"Trace"
	};
}

void LogFormatter::parsePriorityNames()
{
	for (int i = 0; i <= 8; i++)
	{
		m_priorities[i] = priorities[i];
	}

	if (!m_priorityNames.empty())
	{
		StringTokenizer st(m_priorityNames, ",;", StringTokenizer::TOK_TRIM);
		if (st.count() == 8)
		{
			for (int i = 1; i <= 8; i++)
			{
				m_priorities[i] = st[i - 1];
			}
		}
		else throw SyntaxException("priorityNames property must specify a comma-separated list of 8 property names");
	}
}

const String& LogFormatter::getPriorityName(int prio)
{
	cute_assert(1 <= prio && prio <= 8);
	return m_priorities[prio];
}

CUTE_NS_END
