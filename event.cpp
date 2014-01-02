#include "event.h"

namespace yamlman
{
    class base_event
    {
        public:
            int start_mark() const;
            int end_mark() const;
            void start_mark(int val);
            void end_mark(int val);
    };

    class stream_start_event : public base_event
    {
        public:
            std::string const& encoding() const;
            void encoding(std::string const& val);
    };

    class stream_end_event : public base_event
    {
    };

    class document_start_event : public base_event
    {
        public:
            std::string const& version_directive() const;
            std::string const& tag_directives() const;
            bool implicit() const;
            void version_directive(std::string const& val);
            void tag_directives(std::string const& val);
            void implicit(bool val);
    };

    class document_end_event : public base_event
    {
        public:
            bool implicit() const;
            void implicit(bool val);
    };

    class alias_event : public base_event
    {
        public:
            std::string const& anchor() const;
            void anchor(std::string const& val);
    };

    class scalar_event : public base_event
    {
        public:
            std::string const& anchor() const;
            std::string const& tag() const;
            bool plain_implicit() const;
            bool quoted_implicit() const;
            std::string const& value() const;
            size_t length() const;
            std::string const& style() const;
            void anchor(std::string const& val);
            void tag(std::string const& val);
            void plain_implicit(bool val);
            void quoted_implicit(bool val);
            void value(std::string const& val);
            void length(size_t val);
            void style(std::string const& val);
    };

    class sequence_start_event : public base_event
    {
        public:
            std::string const& anchor() const;
            std::string const& tag() const;
            bool implicit() const;
            std::string const& style() const;
            void anchor(std::string const& val);
            void tag(std::string const& val);
            void implicit(bool val);
            void style(std::string const& val);
    };

    class sequence_end_event : public base_event
    {
    };

    class mapping_start_event : public base_event
    {
        public:
            std::string const& anchor() const;
            std::string const& tag() const;
            bool implicit() const;
            std::string const& style() const;
            void anchor(std::string const& val);
            void tag(std::string const& val);
            void implicit(bool val);
            void style(std::string const& val);
    };

    class mapping_end_event : public base_event
    {
    };
} // namespace yamlman

#endif
