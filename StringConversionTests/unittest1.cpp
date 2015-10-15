#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringConversionTests
{		
	using namespace StringConversion;
	using namespace StringHelpers;

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestsBeginning)
		{
			Logger::WriteMessage(">> Tests Beginning...\n");
		}

		TEST_METHOD(TestRemoveBackSlashes)
		{
			auto str = SquashSlashes("\\\\Hello.com\\World");

			Logger::WriteMessage("Removing Back Slashes");
			Assert::AreEqual("/Hello.com/World", str.c_str());
		}


		TEST_METHOD(TestRemoveLastCharIfEqualsAny)
		{
			CString str = _T("/hey/there/");

			CString result = RemoveLastCharIfEqualsAny(str, {_T("/")});

			CString expected = _T("/hey/there");
			Logger::WriteMessage("TestRemoveLastCharIfEqualsAny");
			Assert::AreEqual(cstring2tstring(expected), cstring2tstring(result));
		}

		TEST_METHOD(TestJoinPaths)
		{
			CString p1 = _T("Directory");
			CString p2 = _T("there.bmp");

			CString result = JoinPaths(p1, p2);

			CString expected = _T("Directory/there.bmp");
			Logger::WriteMessage("TestJoinPaths");
			Assert::AreEqual(cstring2tstring(expected), cstring2tstring(result));
		}


		/* - - - -  Test String Conversions  - - - - - */


		TEST_METHOD(TestConvertStringToCString)
		{
			std::wstring str = L"this std string will be converted to CString ÿ}WPÝ. Lets see what happens..";
			CString dst = _T("");
			CString expected = _T("this std string will be converted to CString ÿ}WPÝ. Lets see what happens..");

			dst = tstring2cstring(str);

			Logger::WriteMessage("Converting std::string to CString");
			Assert::AreEqual(dst.GetBuffer(), expected.GetBuffer());
		}

		TEST_METHOD(TestConvertMBToWideChar_ChartoCString)
		{
			// The strings are both utf-8

			CString dst;     // wide chars   (is like wstring)
			char* src;		 // 'multi-byte' 
			src = "Hey There!";

			int err = MultiByteToWideCharUTF8(src, dst);

			Logger::WriteMessage("MultiByteToWideCharUTF8");
			Assert::IsTrue(err > 0);
		}


		TEST_METHOD(TestConvertMBToWideChar_CharToTString)
		{
			// The strings are both utf-8

			tstring dst;     // wide chars   (is like wstring)
			char* src;		 // 'multi-byte' 
			src = "Hey There!";

			MultiByteToWideCharUTF8_Tstring(src, dst);

			Logger::WriteMessage("MultiByteToWideCharUTF8_Tstring");
		}


		/// More String Conversion

		TEST_METHOD(Test_chars2cstring)
		{
			const char chars[] = "Hey There Chars 2 CString!!";
			CString result = chars2cstring(chars);
			CString expected = _T("Hey There Chars 2 CString!!");

			Logger::WriteMessage("Test_chars2cstring");
			Assert::AreEqual(cstring2tstring(expected), cstring2tstring(result));
		}


		TEST_METHOD(Test_cstring2chars)
		{
			CString str = _T("Hey There CString 2 Chars");
			char* arr = cstring2chars(str);
			CString expected = _T("Directory/there.bmp");

			Logger::WriteMessage("Test_cstring2chars");
		}


		TEST_METHOD(Test_stdstring2cstring)
		{
			
		}

		TEST_METHOD(Test_cstring2stdstring)
		{
			
		}

		TEST_METHOD(Test_STD2CSTR)
		{
			
		}


		TEST_METHOD(Test_CSTR2STD)
		{
			
		}

		TEST_METHOD(Test_STD2TSTR)
		{
			
		}

		TEST_METHOD(Test_TSTR2STD)
		{
			
		}

		TEST_METHOD(Test_CSTR2TSTR)
		{
			
		}


		TEST_METHOD(Test_TSTR2CSTR)
		{
			
		}


		TEST_METHOD(Test_rawbytes2tstring)
		{
			
		}


		/// Test String Conversion Abstraction ///


		TEST_METHOD(Test_StringAbstraction_TemplateMethods)
		{
			std::string s1 = "hey there";

			CString c1 = ConvertString<std::string>(s1);
			tstring t1 = ConvertString<std::string>(s1);
			
			tstring t2 = ConvertString<CString>(c1);
			//std::string s2 = ConvertString<CString>(c1); /// TODO: These dont compile, why..

			//std::string s3 = ConvertString<tstring>(t1);
			CString c3 = ConvertString<tstring>(t1);


			Logger::WriteMessage(s1.c_str());
			Logger::WriteMessage(c1);
			Logger::WriteMessage(t1.c_str());

			Logger::WriteMessage("");
			Logger::WriteMessage(c3);
			Logger::WriteMessage(t2.c_str());


			Assert::AreEqual(c1, _T("hey there"));
		}



		TEST_METHOD(TestsComplete)
		{
			Logger::WriteMessage("\n>> Tests Complete...");
		}

	};
}