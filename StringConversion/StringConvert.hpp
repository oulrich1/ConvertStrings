#pragma once

#include <afx.h>

#include "StringConversion.h"

/// This template helper should be used in tandem with the StringConversion Lib.
/// - This depends on the StringConversion Lib definitions.
/// - This is seperated out from the Lib because these are template methods..


namespace StringConversion {
	template <typename InputType>
	class _ConvertStringBase {
	public:
		_ConvertStringBase(InputType input) {
			m_input = input;
		}
		InputType Str()  { return m_input; }
	protected:
		InputType m_input;
	};



	template <typename InputType>
	class ConvertString : _ConvertStringBase<InputType> {
	public:
		virtual operator std::string() {
			static_assert(false, "Must specialize " typeid(your_class).name() ".");
		}
		virtual operator tstring() {
			static_assert(false, "Must specialize " typeid(your_class).name() ".");
		}
		virtual operator CString() {
			static_assert(false, "Must specialize " typeid(your_class).name() ".");
		}
		virtual operator const char*() {
			static_assert(false, "Must specialize " typeid(your_class).name() ".");
		}
	};


	template <>
	class ConvertString<std::string> : _ConvertStringBase<std::string> {
	public: 
		ConvertString(std::string input) : _ConvertStringBase(input) {}
		virtual operator std::string() {
			return Str();
		}
		virtual operator tstring() {
			return STD2TSTR(Str());
		}
		virtual operator CString() {
			return STD2CSTR(Str());
		}
		virtual operator const char*() { 
			// return val must be deleted when finished with the data
			return cstring2chars(chars2cstring(Str().c_str()));
		}
	};

	template <>
	class ConvertString<CString> : _ConvertStringBase<CString> {
	public:
		ConvertString(CString input) : _ConvertStringBase(input) {}
		virtual operator std::string() {
			return CSTR2STD(Str());
		}
		virtual operator tstring() {
			return CSTR2TSTR(Str());
		}
		virtual operator CString() {
			return Str();
		}
		virtual operator const char*() {
			// return val must be deleted when finished with the data
			return cstring2chars(Str());
		}
	};

	template <>
	class ConvertString<tstring> : _ConvertStringBase<tstring> {
	public:
		ConvertString(tstring input) : _ConvertStringBase(input) {}
		virtual operator std::string() {
			return TSTR2STD(Str());
		}
		virtual operator tstring() {
			return Str();
		}
		virtual operator CString() {
			return TSTR2CSTR(Str());
		}
		virtual operator const char*() {
			// return val must be deleted when finished with the data
			return cstring2chars(tstring2cstring(Str()));
		}
	};
}
