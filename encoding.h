namespace yamlman
{
    class encoding
    {
        public:
            explicit encoding(yaml_encoding_t const& encoding) : _encoding(encoding){}
        public:
            yaml_encoding_t const get() const
            {
                return _encoding;
            }
            std::string const stringify() const
            {
                switch(_encoding)
                {
                    case YAML_UTF8_ENCODING:
                        return std::string("UTF-8");
                    case YAML_UTF16LE_ENCODING:
                        return std::string("UTF-16LE");
                    case YAML_UTF16BE_ENCODING:
                        return std::string("UTF-16BE");
                    case YAML_ANY_ENCODING:
                        return std::string("Any");
                    default:
                        return std::string("");
                }
            }
        private:
            yaml_encoding_t const _encoding;
    };

    bool operator == (encoding const& lhs, encoding const& rhs)
    {
        return lhs.get() == rhs.get();
    }

    bool operator != (encoding const& lhs, encoding const& rhs)
    {
        return !(lhs == rhs);
    }

    bool operator == (encoding const& lhs, yaml_encoding_t const& rhs)
    {
        return lhs == encoding(rhs);
    }

    bool operator != (encoding const& lhs, yaml_encoding_t const& rhs)
    {
        return !(lhs == rhs);
    }

    bool operator == (yaml_encoding_t const& lhs, encoding const& rhs)
    {
        return encoding(lhs) == rhs;
    }

    bool operator != (yaml_encoding_t const& lhs, encoding const& rhs)
    {
        return !(lhs == rhs);
    }
} // namespace yaml
