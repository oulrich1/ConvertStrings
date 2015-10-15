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
		virtual operator char*() {
			static_assert(false, "Must specialize " typeid(your_class).name() ".");
		}
	};


	template <>
	class ConvertString<std::string> : _ConvertStringBase<std::string> {
	public: 
		ConvertString(std::string input) : _ConvertStringBase(input) {}
		virtual operator std::string() {
			return this->Str();
		}
		virtual operator tstring() {
			tstring dst;
			string2tstring(this->Str(), dst);
			return dst;
		}
		virtual operator CString() {
			CString dst;
			stdstring2cstring(this->Str(), dst);
			return dst;
		}
		virtual operator const char*() {
			return this->Str().c_str();
		}
	};

	template <>
	class ConvertString<CString> : _ConvertStringBase<CString> {
	public:
		ConvertString(CString input) : _ConvertStringBase(input) {}
		virtual operator std::string() {
			std::string dst;
			cstring2stdstring(Str(), dst);
			return dst;
		}
		virtual operator tstring() {
			return cstring2tstring(Str());
		}
		virtual operator CString() {
			return Str();
		}
		virtual operator char*() {
			return 0;
		}
	};

	template <>
	class ConvertString<tstring> : _ConvertStringBase<tstring> {
	public:
		ConvertString(tstring input) : _ConvertStringBase(input) {}
		virtual operator std::string() {
			std::string dst;
			tstring2string(Str(), dst);
			return dst;
		}
		virtual operator tstring() {
			return this->Str();
		}
		virtual operator CString() {
			return tstring2cstring(Str());
		}
		virtual operator char*() {
			return 0;
		}
	};
}

