#pragma once
#include "Value.h"

namespace SCFBase
{
	//Using Gregorian calendar
	//1 year = 365.2425 days 

	class OBJECT_EXTENSIONS_API CDateTime : public CValue
	{
	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; Parse(*this, rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static CDateTime& Parse                           (_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static void       Parse(_OUT CDateTime& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
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
		SCF::USHORT Year()                       _GET { return m_usYear; }
		void        Year(_IN SCF::USHORT usYear) _SET { m_usYear = usYear; }

		SCF::BYTE Month()                      _GET { return m_ucMonth; }
		void      Month(_IN SCF::BYTE ucMonth) _SET { m_ucMonth = ucMonth; }

		SCF::BYTE Day()                    _GET { return m_ucDay; }
		void      Day(_IN SCF::BYTE ucDay) _SET { m_ucDay = ucDay; }

	public:
		SCF::BYTE Hour()                     _GET { return m_ucHour; }
		void      Hour(_IN SCF::BYTE ucHour) _SET { m_ucHour = ucHour; }

		SCF::BYTE Minute()                       _GET { return m_ucMinute; }
		void      Minute(_IN SCF::BYTE ucMinute) _SET { m_ucMinute = ucMinute; }

		SCF::BYTE Second()                       _GET { return m_ucSecond; }
		void      Second(_IN SCF::BYTE ucSecond) _SET { m_ucSecond = ucSecond; }

		SCF::USHORT Milliseconds()                               _GET { return m_usMilliseconds; }
		void        Milliseconds(_IN SCF::USHORT usMilliseconds) _SET { m_usMilliseconds = usMilliseconds; }

	public:
		SCF::ENUM DayOfWeek() _GET;

	protected:
		SCF::USHORT m_usMilliseconds;
		SCF::USHORT m_usYear;

		SCF::BYTE m_ucMonth;
		SCF::BYTE m_ucDay;
		SCF::BYTE m_ucHour;
		SCF::BYTE m_ucMinute;
		SCF::BYTE m_ucSecond;
	};
};
