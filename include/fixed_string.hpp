#pragma once

#include <cstring>
#include <stdexcept>
#include <string>

namespace fappy {
        class fixed_string_lenth_exception : public std::runtime_error
        {
                public:
                fixed_string_lenth_exception(const char* msg)
                  : std::runtime_error(msg)
                {
                }
        };

        /*! \brief Fixed length basic string class.
         * \tparam CharT Char type.
         * \tparam Size Size of string.
         */
        template <typename CharT, std::size_t Size>
        class basic_fixed_string
        {
                using chart_type          = CharT;
                using variable_len_string = std::basic_string<chart_type>;

                public:
                basic_fixed_string()
                {
                        std::memset(chars_, 0, Size);
                }

                basic_fixed_string(const chart_type* chars)
                {
                        length_ = std::min(Size, std::strlen(chars) + 1);
                        std::memcpy(chars_, chars, sizeof(chart_type) * length_);
                }

                /*! \brief Constructor from variadic length string (aka \link std::basic_string).
                 */
                basic_fixed_string(const variable_len_string& other)
                {
                        length_ = std::min(Size, other.size());
                        std::memcpy(chars_, other.data(), sizeof(chart_type) * length_);
                }

                basic_fixed_string(const basic_fixed_string& other)
                {
                        std::memcpy(chars_, other.chars_, sizeof(chart_type) * Size);
                }

                basic_fixed_string(basic_fixed_string&& /*other*/)
                {
                }

                basic_fixed_string& operator=(const basic_fixed_string& other)
                {
                        if (other != *this) {
                                copy(other.size(), other.chars_);
                                return *this;
                        } else {
                                return *this;
                        }
                }

                chart_type& operator[](std::size_t index)
                {
                        return chars_[index];
                }

                const chart_type& operator[](std::size_t index) const
                {
                        return chars_[index];
                }

                /*! \brief Current string size (not null chars).
                 */
                std::size_t size() const
                {
                        return length_;
                }

                /*! \brief Maximum string length.
                 */
                constexpr std::size_t max_size() const
                {
                        return Size;
                }

                const chart_type* raw() const
                {
                        return chars_;
                }

                variable_len_string substr(std::size_t start, std::size_t len) const
                {
                        if (start >= this->length_)
                                throw fixed_string_lenth_exception(
                                  "Cannot get substring from start position that more than string lenth!");
                        const auto len__ = std::min(this->length_ - start, len + 1);
                        variable_len_string ret(chars_ + start, chars_ + (start + len__));
                        return ret;
                }

                bool operator==(const chart_type* other) const
                {
                        const auto len__ = std::min(this->length_, std::strlen(other));
                        return std::memcmp(chars_, other, len__) == 0;
                }

                bool operator==(const basic_fixed_string& other) const
                {
                        return other == chars_;
                }

                bool operator!=(const basic_fixed_string& other) const
                {
                        return !(other == this->chars_);
                }

                private:
                void copy(const std::size_t size, const chart_type* chars)
                {
                        length_ = size;
                        std::memcpy(this->chars_, chars, sizeof(chart_type) * this->length_);
                }

                chart_type chars_[Size];
                std::size_t length_;
        };

        /*! \brief Fixed string with `char' type base template alias.
         */
        template <std::size_t Size>
        using fixed_string_char = basic_fixed_string<char, Size>;
}