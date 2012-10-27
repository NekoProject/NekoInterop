// NekoInterop.Ruby.h

#pragma once

using namespace System;

namespace NekoInterop {

	public ref class Ruby
	{
		static bool initialized = false;
	private:
		static VALUE EvalBase(System::String ^code)
		{
			if (code == nullptr)
				return Qnil;

			if (code->Length == 0)
				return Qnil;

			cli::array<unsigned char>^ raw = System::Text::Encoding::UTF8->GetBytes(code);
			int len = raw->Length;
			
			char * str = new char[len + 1];
			for (int i = 0; i < len; i++)
				str[i] = raw[i];
			str[len] = 0;

			VALUE ret = rb_eval_string_protect(str, 0);

			delete []str;
			return ret;
		}

		static cli::array<unsigned char> ^ GetStringBytes(VALUE ptr)
		{
			const char * raw = RSTRING_PTR(ptr);
			const int len = RSTRING_LEN(ptr);

			cli::array<unsigned char>^ clistr = gcnew cli::array<unsigned char>(len);
			for(int i = 0; i < len; i++)
				clistr[i] = raw[i];
			
			return clistr;
		}

		static System::String ^ GetString(VALUE ptr)
		{
			return System::Text::Encoding::UTF8->GetString(GetStringBytes(ptr));
		}

		static System::Object ^ GetObject(VALUE obj)
		{
			if (obj == Qnil) {
				return nullptr;
			}
			else if (obj == Qtrue) {
				return true;
			}
			else if (obj == Qfalse) {
				return false;
			}
			else if (FIXNUM_P(obj)) {
				return FIX2INT(obj);
			}
		    else if (SYMBOL_P(obj)) {
				return GetString(rb_id2str(SYM2ID(obj)));
			}
			else {
				switch (BUILTIN_TYPE(obj)) {
					case T_STRING:
						return GetString(obj);
					default:
						VALUE v = rb_funcall(obj, rb_intern("to_s"), 0, 0);
						return GetObject(v);
				}
			}
			return nullptr;
		}
	public:
		static void Initialize()
		{
			if (initialized)
				return;

			int argc = 0;
			char **argv;

			ruby_sysinit(&argc, &argv);
			ruby_init();
			ruby_init_loadpath();

			initialized = true;
		}

		static System::Object ^ Eval(System::String ^code)
		{
			return GetObject(EvalBase(code));
		}
	};
}
