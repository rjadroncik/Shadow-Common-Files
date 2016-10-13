#pragma once
#include "Value.h"

namespace SCFBase
{
	//Using Gregorian calendar
	//1 year = 365.2425 days 

	class OBJECT_EXTENSIONS_API CDateTime : public CValue
	{
	public:
		UINT Parse(_IN CString& rString) { UINT uiCharsParsed = 0; Parse(*this, rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		ValueKind Kind() _GET { return ValueDateTime; }

	public:
		static CDateTime& Parse                           (_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static void       Parse(_OUT CDateTime& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString    Print(_IN  CDateTime& rValue);

	public:
		CString ToString() _GET;

	public:
		CDateTime();
		CDateTime(_IN CString& rString);
		CDateTime(_IN CDateTime& rValue);

		virtual ~CDateTime();

	public:
		bool IsSmallerThen(_IN CObject& rObject) const;
		bool IsEqualTo    (_IN CObject& rObject) const;

	public:
		USHORT Year()                  _GET { return m_usYear; }
		void   Year(_IN USHORT usYear) _SET { m_usYear = usYear; }

		BYTE Month()                 _GET { return m_ucMonth; }
		void Month(_IN BYTE ucMonth) _SET { m_ucMonth = ucMonth; }

		BYTE Day()               _GET { return m_ucDay; }
		void Day(_IN BYTE ucDay) _SET { m_ucDay = ucDay; }

	public:
		BYTE Hour()                _GET { return m_ucHour; }
		void Hour(_IN BYTE ucHour) _SET { m_ucHour = ucHour; }

		BYTE Minute()                  _GET { return m_ucMinute; }
		void Minute(_IN BYTE ucMinute) _SET { m_ucMinute = ucMinute; }

		BYTE Second()                  _GET { return m_ucSecond; }
		void Second(_IN BYTE ucSecond) _SET { m_ucSecond = ucSecond; }

		USHORT Milliseconds()                          _GET { return m_usMilliseconds; }
		void   Milliseconds(_IN USHORT usMilliseconds) _SET { m_usMilliseconds = usMilliseconds; }

	public:
		ENUM DayOfWeek() _GET;

	protected:
		USHORT m_usMilliseconds;
		USHORT m_usYear;

		BYTE m_ucMonth;
		BYTE m_ucDay;
		BYTE m_ucHour;
		BYTE m_ucMinute;
		BYTE m_ucSecond;
	};
};
