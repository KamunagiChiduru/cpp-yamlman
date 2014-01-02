#ifndef YAMLMAN_EVENT_H_
#define YAMLMAN_EVENT_H_

#include <string>

namespace yamlman
{
    class mark
    {
        public:
            int line() const{ return _line; }
            int column() const{ return _column; }
            int index() const{ return _index; }
            void line(int val){ _line= val; }
            void column(int val){ _column= val; }
            void index(int val){ _index= val; }
        private:
            int _line, _column, _index;
    };

    class base_event
    {
        public:
            mark start_mark() const{ return _start; }
            mark end_mark() const{ return _end; }
            void start_mark(mark const& val){ _start= val; }
            void end_mark(mark const& val){ _end= val; }
        private:
            mark _start, _end;
    };

    class stream_start_event : public base_event
    {
        public:
            std::string const& encoding() const{ return _encoding; }
            void encoding(std::string const& val){ _encoding= val; }
        private:
            std::string _encoding;
    };

    class stream_end_event : public base_event
    {
    };

    class document_start_event : public base_event
    {
        public:
            std::string const& version_directive() const{ return _version_directive; }
            std::string const& tag_directives() const{ return _tag_directives; }
            bool implicit() const{ return _implicit; }
            void version_directive(std::string const& val){ _version_directive= val; }
            void tag_directives(std::string const& val){ _tag_directives= val; }
            void implicit(bool val){ _implicit= val; }
        private:
            std::string _version_directive, _tag_directives;
            bool _implicit;
    };

    class document_end_event : public base_event
    {
        public:
            bool implicit() const{ return _implicit; }
            void implicit(bool val){ _implicit= val; }
        private:
            bool _implicit;
    };

    class alias_event : public base_event
    {
        public:
            std::string const& anchor() const{ return _anchor; }
            void anchor(std::string const& val){ _anchor= val; }
        private:
            std::string _anchor;
    };

    class scalar_event : public base_event
    {
        public:
            std::string const& anchor() const{ return _anchor; }
            std::string const& tag() const{ return _tag; }
            bool plain_implicit() const{ return _plain_implicit; }
            bool quoted_implicit() const{ return _quotec_implicit; }
            std::string const& value() const{ return _value; }
            std::string const& style() const{ return _style; }
            void anchor(std::string const& val){ _anchor= val; }
            void tag(std::string const& val){ _tag= val; }
            void plain_implicit(bool val){ _plain_implicit= val; }
            void quoted_implicit(bool val){ _quotec_implicit= val; }
            void value(std::string const& val){ _value= val; }
            void style(std::string const& val){ _style= val; }
        private:
            std::string _anchor, _tag;
            bool _plain_implicit, _quotec_implicit;
            std::string _value;
            std::string _style;
    };

    class sequence_start_event : public base_event
    {
        public:
            std::string const& anchor() const{ return _anchor; }
            std::string const& tag() const{ return _tag; }
            bool implicit() const{ return _implicit; }
            std::string const& style() const{ return _style; }
            void anchor(std::string const& val){ _anchor= val; }
            void tag(std::string const& val){ _tag= val; }
            void implicit(bool val){ _implicit= val; }
            void style(std::string const& val){ _style= val; }
        private:
            std::string _anchor, _tag;
            bool _implicit;
            std::string _style;
    };

    class sequence_end_event : public base_event
    {
    };

    class mapping_start_event : public base_event
    {
        public:
            std::string const& anchor() const{ return _anchor; }
            std::string const& tag() const{ return _tag; }
            bool implicit() const{ return _implicit; }
            std::string const& style() const{ return _style; }
            void anchor(std::string const& val){ _anchor= val; }
            void tag(std::string const& val){ _tag= val; }
            void implicit(bool val){ _implicit= val; }
            void style(std::string const& val){ _style= val; }
        private:
            std::string _anchor, _tag;
            bool _implicit;
            std::string _style;
    };

    class mapping_end_event : public base_event
    {
    };
} // namespace yamlman

#endif
