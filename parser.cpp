#include "parser.h"
#include <yaml.h>
#include <vector>
#include <iostream>

namespace yamlman
{
    std::string const convert(yaml_char_t const* s)
    {
        std::string res;

        while(s && *s)
        {
            res.push_back(*s);
            ++s;
        }

        return res;
    }

    class parser::impl
    {
        public:
            typedef std::function<void(yaml_parser_t*)> yaml_parser_deleter_t;
            typedef std::unique_ptr<yaml_parser_t, yaml_parser_deleter_t> lp_parser_t;
        public:
            explicit impl(std::istream& istream) : _parser(make_parser(istream))
            {
            }
            ~impl()= default;
        public:
            void on_stream_start(stream_start_handler_t const& handler)
            {
                _stream_start_handlers.push_back(handler);
            }

            void on_stream_end(stream_end_handler_t const& handler)
            {
                _stream_end_handlers.push_back(handler);
            }

            void on_document_start(document_start_handler_t const& handler)
            {
                _document_start_handlers.push_back(handler);
            }

            void on_document_end(document_end_handler_t const& handler)
            {
                _document_end_handlers.push_back(handler);
            }

            void on_alias(alias_handler_t const& handler)
            {
                _alias_handlers.push_back(handler);
            }

            void on_scalar(scalar_handler_t const& handler)
            {
                _scalar_handlers.push_back(handler);
            }

            void on_sequence_start(sequence_start_handler_t const& handler)
            {
                _sequence_start_handlers.push_back(handler);
            }

            void on_sequence_end(sequence_end_handler_t const& handler)
            {
                _sequence_end_handlers.push_back(handler);
            }

            void on_mapping_start(mapping_start_handler_t const& handler)
            {
                _mapping_start_handlers.push_back(handler);
            }

            void on_mapping_end(mapping_end_handler_t const& handler)
            {
                _mapping_end_handlers.push_back(handler);
            }

            void parse()
            {
                auto deleter= [](yaml_event_t* event){
                    if(event)
                    {
                        yaml_event_delete(event);
                    }
                };
                typedef std::unique_ptr<yaml_event_t, decltype(deleter)> lp_event_t;

                yaml_event_t event;
                int done= 0;

                while(!done)
                {
                    lp_event_t pevent(&event, deleter);

                    if(!yaml_parser_parse(_parser.get(), pevent.get()))
                    {
                        break;
                    }

                    // process events
                    switch(event.type)
                    {
                        // Stylistic Event Attributes on any event
                        // start_mark - the position of the event beginning; attributes: index (in characters), line and column (starting from 0).
                        // end_mark   - the position of the event end; attributes: index (in characters), line and column (starting from 0).
                        case YAML_STREAM_START_EVENT:{
                            // Stylistic Event Attributes
                            // encoding - the document encoding; utf-8|utf-16-le|utf-16-be. 
                            stream_start_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }
                            switch(event.data.stream_start.encoding)
                            {
                                case YAML_UTF8_ENCODING:
                                    e.encoding("UTF-8");
                                    break;
                                case YAML_UTF16LE_ENCODING:
                                    e.encoding("UTF-16LE");
                                    break;
                                case YAML_UTF16BE_ENCODING:
                                    e.encoding("UTF-16BE");
                                    break;
                                case YAML_ANY_ENCODING:
                                    e.encoding("Any");
                                    break;
                                default:
                                    e.encoding("");
                                    break;
                            }

                            for(auto handler : _stream_start_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_STREAM_END_EVENT:{
                            stream_end_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }

                            for(auto handler : _stream_end_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_DOCUMENT_START_EVENT:{
                            // Stylistic Event Attributes
                            // version_directive - the version specified with the %YAML directive; the only valid value is 1.1; may be NULL.
                            // tag_directives    - a set of tag handles and the corresponding tag prefixes specified with the %TAG directive; tag handles should match !|!!|![0-9a-zA-Z_-]+! while tag prefixes should be prefixes of valid local or global tags; may be empty.
                            // implicit          - True if the document start indicator --- is not present.
                            document_start_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }
                            {
                                std::string version;
                                yaml_version_directive_t const* const vd= event.data.document_start.version_directive;

                                if(vd)
                                {
                                    version+= vd->major;
                                    version+= ".";
                                    version+= vd->minor;
                                }

                                e.version_directive(version);
                            }
                            {
                                e.tag_directives("");
                            }
                            e.implicit(event.data.document_start.implicit);

                            for(auto handler : _document_start_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_DOCUMENT_END_EVENT:{
                            // Stylistic Event Attributes
                            // implicit - True if the document end indicator ... is not present. 
                            document_end_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }
                            e.implicit(event.data.document_end.implicit);

                            for(auto handler : _document_end_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_ALIAS_EVENT:{
                            // Essential Event Attributes
                            // anchor - the alias anchor; [0-9a-zA-Z_-]+; not null.
                            alias_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }
                            e.anchor(convert(event.data.alias.anchor));

                            for(auto handler : _alias_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_SCALAR_EVENT:{
                            // Essential Event Attributes
                            // anchor          - the node anchor; [0-9a-zA-Z_-]+; may be NULL.
                            // tag             - the node tag; should either start with ! (local tag) or be a valid URL (global tag); may be NULL or ! in which case either plain_implicit or quoted_implicit should be True.
                            // plain_implicit  - True if the node tag may be omitted whenever the scalar value is presented in the plain style.
                            // quoted_implicit - True if the node tag may be omitted whenever the scalar value is presented in any non-plain style.
                            // value           - the scalar value; a valid utf-8 sequence and may contain NUL characters; not NULL.
                            // length          - the length of the scalar value.
                            //
                            // Stylistic Event Attributes
                            // style - the value style; plain|single-quoted|double-quoted|literal|folded.
                            scalar_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }
                            e.anchor(convert(event.data.scalar.anchor));
                            e.tag(convert(event.data.scalar.tag));
                            e.plain_implicit(event.data.scalar.plain_implicit);
                            e.quoted_implicit(event.data.scalar.quoted_implicit);
                            e.value(convert(event.data.scalar.value));
                            switch(event.data.scalar.style)
                            {
                                case YAML_PLAIN_SCALAR_STYLE:
                                    e.style("");
                                    break;
                                case YAML_SINGLE_QUOTED_SCALAR_STYLE:
                                    e.style("single-quoted");
                                    break;
                                case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
                                    e.style("double-quoted");
                                    break;
                                case YAML_LITERAL_SCALAR_STYLE:
                                    e.style("literal");
                                    break;
                                case YAML_FOLDED_SCALAR_STYLE:
                                    e.style("folded");
                                    break;
                                case YAML_ANY_SCALAR_STYLE:
                                default:
                                    e.style("");
                                    break;
                            }

                            for(auto handler : _scalar_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_SEQUENCE_START_EVENT:{
                            // Essential Event Attributes
                            // anchor   - the node anchor; [0-9a-zA-Z_-]+; may be NULL.
                            // tag      - the node tag; should either start with ! (local tag) or be a valid URL (global tag); may be NULL or ! in which case implicit should be True.
                            // implicit - True if the node tag may be omitted.
                            //
                            // Stylistic Event Attributes
                            // style - the sequence style; block|flow. 
                            sequence_start_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }
                            e.anchor(convert(event.data.sequence_start.anchor));
                            e.tag(convert(event.data.sequence_start.tag));
                            e.implicit(event.data.sequence_start.implicit);
                            switch(event.data.sequence_start.style)
                            {
                                case YAML_FLOW_SEQUENCE_STYLE:
                                    e.style("flow");
                                    break;
                                case YAML_BLOCK_SEQUENCE_STYLE:
                                    e.style("block");
                                    break;
                                case YAML_ANY_SEQUENCE_STYLE:
                                    e.style("any");
                                    break;
                                default:
                                    e.style("");
                                    break;
                            }

                            for(auto handler : _sequence_start_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_SEQUENCE_END_EVENT:{
                            sequence_end_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }

                            for(auto handler : _sequence_end_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_MAPPING_START_EVENT:{
                            // Essential Event Attributes
                            // anchor   - the node anchor; [0-9a-zA-Z_-]+; may be NULL.
                            // tag      - the node tag; should either start with ! (local tag) or be a valid URL (global tag); may be NULL or ! in which case implicit should be True.
                            // implicit - True if the node tag may be omitted.
                            //
                            // Stylistic Event Attributes
                            // style - the mapping style; block|flow. 
                            mapping_start_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }
                            e.anchor(convert(event.data.mapping_start.anchor));
                            e.tag(convert(event.data.mapping_start.tag));
                            e.implicit(event.data.mapping_start.implicit);
                            switch(event.data.mapping_start.style)
                            {
                                case YAML_FLOW_MAPPING_STYLE:
                                    e.style("flow");
                                    break;
                                case YAML_BLOCK_MAPPING_STYLE:
                                    e.style("block");
                                    break;
                                case YAML_ANY_MAPPING_STYLE:
                                    e.style("any");
                                    break;
                                default:
                                    e.style("");
                                    break;
                            }

                            for(auto handler : _mapping_start_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_MAPPING_END_EVENT:{
                            mapping_end_event e;

                            {
                                mark mark;

                                mark.line(event.start_mark.line);
                                mark.column(event.start_mark.column);
                                mark.index(event.start_mark.index);

                                e.start_mark(mark);
                            }
                            {
                                mark mark;

                                mark.line(event.end_mark.line);
                                mark.column(event.end_mark.column);
                                mark.index(event.end_mark.index);

                                e.end_mark(mark);
                            }

                            for(auto handler : _mapping_end_handlers)
                            {
                                handler(e);
                            }
                            break;
                        }
                        case YAML_NO_EVENT:
                        default:
                            break;
                    }

                    done= (event.type == YAML_STREAM_END_EVENT);
                }
            }
        private:
            static lp_parser_t make_parser(std::istream& istream)
            {
                lp_parser_t parser(new yaml_parser_t, [](yaml_parser_t* p){
                    if(p)
                    {
                        yaml_parser_delete(p);
                        delete p;
                    }
                });

                yaml_parser_initialize(parser.get());

                yaml_parser_set_input(
                    parser.get(),
                    [](void* ext, unsigned char* buffer, size_t size, size_t* size_read)->int{
                        std::istream* istream= static_cast<std::istream*>(ext);
                        std::istream& read= istream->read(reinterpret_cast<char*>(buffer), size);

                        *size_read= read.gcount();

                        // error:   0
                        // eof: (size_read, ret) = (0, 1)
                        // success: 1
                        return 1;
                    },
                    &istream
                );

                return parser;
            }
        private:
            lp_parser_t _parser;
            std::vector<stream_start_handler_t>   _stream_start_handlers;
            std::vector<stream_end_handler_t>     _stream_end_handlers;
            std::vector<document_start_handler_t> _document_start_handlers;
            std::vector<document_end_handler_t>   _document_end_handlers;
            std::vector<alias_handler_t>          _alias_handlers;
            std::vector<scalar_handler_t>         _scalar_handlers;
            std::vector<sequence_start_handler_t> _sequence_start_handlers;
            std::vector<sequence_end_handler_t>   _sequence_end_handlers;
            std::vector<mapping_start_handler_t>  _mapping_start_handlers;
            std::vector<mapping_end_handler_t>    _mapping_end_handlers;
    };

    parser::parser(std::istream& istream) : _impl(new impl(istream))
    {
    }

    parser::~parser()= default;

    parser& parser::on_stream_start(stream_start_handler_t const& handler)
    {
        _impl->on_stream_start(handler);
        return *this;
    }

    parser& parser::on_stream_end(stream_end_handler_t const& handler)
    {
        _impl->on_stream_end(handler);
        return *this;
    }

    parser& parser::on_document_start(document_start_handler_t const& handler)
    {
        _impl->on_document_start(handler);
        return *this;
    }

    parser& parser::on_document_end(document_end_handler_t const& handler)
    {
        _impl->on_document_end(handler);
        return *this;
    }

    parser& parser::on_alias(alias_handler_t const& handler)
    {
        _impl->on_alias(handler);
        return *this;
    }

    parser& parser::on_scalar(scalar_handler_t const& handler)
    {
        _impl->on_scalar(handler);
        return *this;
    }

    parser& parser::on_sequence_start(sequence_start_handler_t const& handler)
    {
        _impl->on_sequence_start(handler);
        return *this;
    }

    parser& parser::on_sequence_end(sequence_end_handler_t const& handler)
    {
        _impl->on_sequence_end(handler);
        return *this;
    }

    parser& parser::on_mapping_start(mapping_start_handler_t const& handler)
    {
        _impl->on_mapping_start(handler);
        return *this;
    }

    parser& parser::on_mapping_end(mapping_end_handler_t const& handler)
    {
        _impl->on_mapping_end(handler);
        return *this;
    }

    void parser::parse()
    {
        _impl->parse();
    }
} // namespace yaml

