#ifndef YAMLMAN_PARSER_H_
#define YAMLMAN_PARSER_H_

#include "event.h"
#include <functional>
#include <memory>
#include <iostream>

namespace yamlman
{
    class parser
    {
        public:
            typedef std::function<void(stream_start_event const&)>   stream_start_handler_t;
            typedef std::function<void(stream_end_event const&)>     stream_end_handler_t;
            typedef std::function<void(document_start_event const&)> document_start_handler_t;
            typedef std::function<void(document_end_event const&)>   document_end_handler_t;
            typedef std::function<void(alias_event const&)>          alias_handler_t;
            typedef std::function<void(scalar_event const&)>         scalar_handler_t;
            typedef std::function<void(sequence_start_event const&)> sequence_start_handler_t;
            typedef std::function<void(sequence_end_event const&)>   sequence_end_handler_t;
            typedef std::function<void(mapping_start_event const&)>  mapping_start_handler_t;
            typedef std::function<void(mapping_end_event const&)>    mapping_end_handler_t;
        public:
            explicit parser(std::istream& istream);
            ~parser();
        public:
            parser& on_stream_start(stream_start_handler_t const& handler);
            parser& on_stream_end(stream_end_handler_t const& handler);
            parser& on_document_start(document_start_handler_t const& handler);
            parser& on_document_end(document_end_handler_t const& handler);
            parser& on_alias(alias_handler_t const& handler);
            parser& on_scalar(scalar_handler_t const& handler);
            parser& on_sequence_start(sequence_start_handler_t const& handler);
            parser& on_sequence_end(sequence_end_handler_t const& handler);
            parser& on_mapping_start(mapping_start_handler_t const& handler);
            parser& on_mapping_end(mapping_end_handler_t const& handler);
            void parse();
        private:
            class impl;
            std::unique_ptr<impl> _impl;
    };
} // namespace yaml

#endif // YAMLMAN_PARSER_H_
