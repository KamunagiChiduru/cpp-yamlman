#include "parser.h"
#include "event.h"
#include <iostream>

std::ostream& operator << (std::ostream& ostream, yamlman::mark const& mark)
{
    ostream
        << "(line, column, index) == "
        << "(" << mark.line() << ", " << mark.column() << ", " << mark.index() << ")"
    ;
    return ostream;
}

int main(int argc, char const* argv[])
{
    using namespace yamlman;

    parser parser(std::cin);

    parser
        .on_stream_start([](stream_start_event const& e){
            std::cout
                << "[stream start]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << "[encoding: " << e.encoding() << "]"
                << std::endl;
        })
        .on_document_start([](document_start_event const& e){
            std::cout << "[document start]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << "[version: " << e.version_directive() << "]"
                << std::endl;
        })
        .on_alias([](alias_event const& e){
            std::cout << "[alias]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << "[anchor: " <<  e.anchor() << "]"
                << std::endl;
        })
        .on_scalar([](scalar_event const& e){
            std::cout << "[scalar]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << "[anchor: " << e.anchor() << "]"
                << "[tag: " << e.tag() << "]"
                << "[value: " << e.value() << "]"
                << "[plain_implicit: " << e.plain_implicit() << "]"
                << "[quoted_implicit: " << e.quoted_implicit() << "]"
                << "[style: " << e.style() << "]"
                << std::endl;
        })
        .on_mapping_start([](mapping_start_event const& e){
            std::cout << "[mapping start]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << "[anchor: " << e.anchor() << "]"
                << "[tag: " << e.tag() << "]"
                << "[implicit: " << e.implicit() << "]"
                << "[style: " << e.style() << "]"
                << std::endl;
        })
        .on_mapping_end([](mapping_end_event const& e){
            std::cout << "[mapping end]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << std::endl;
        })
        .on_sequence_start([](sequence_start_event const& e){
            std::cout << "[sequence start]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << "[anchor: " << e.anchor() << "]"
                << "[tag: " << e.tag() << "]"
                << "[implicit: " << e.implicit() << "]"
                << "[style: " << e.style() << "]"
                << std::endl;
        })
        .on_sequence_end([](sequence_end_event const& e){
            std::cout << "[sequence end]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << std::endl;
        })
        .on_document_end([](document_end_event const& e){
            std::cout << "[document end]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << "[implicit: " << e.implicit() << "]"
                << std::endl;
        })
        .on_stream_end([](stream_end_event const& e){
            std::cout << "[stream end]"
                << "[start: " << e.start_mark() << "]"
                << "[end: " << e.end_mark() << "]"
                << std::endl;
        })
        .parse()
    ;

    return 0;
}

