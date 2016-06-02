#ifndef _UTIL_URL_ENCODE_ITERATOR_INCLUDED_
#define _UTIL_URL_ENCODE_ITERATOR_INCLUDED_

#include <stdexcept>
#include <iterator>

namespace util
{
  namespace url
  {
    namespace dict
    {
      static char const base16[] = {
        '0','1','2','3','4','5','6','7',
        '8','9','A','B','C','D','E','F'
      };

      inline
      bool check_digit(char ch)
      {
        return
          ch >= '0' && ch <= '9' ||
          ch >= 'A' && ch <= 'F' ||
          ch >= 'a' && ch <= 'f';
      }

      inline
      int char_digit(char ch)
      {
        return (
          ch <= '9'
            ? (ch - '0')
            : (ch & 7) + 9
        );
      }

      // Character codes that have to be encoded/decoded
      // - ascii control char codes:      [0 - 31], 127
      // - none ascii control char codes: [128 - 255]
      // - reserved char codes:           [36, 38, 43, 44, 47, 58, 59, 61, 63, 64]
      // - unsafe char codes:             [32, 34, 60, 62, 35, 37, 123, 125, 124, 92, 94, 126, 91, 93, 96]
      static char const char_table[] =
      {
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  00 -  15
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  16 -  31
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  , -, .,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, //  32 -  47
      //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  ,  ,  ,  ,  ,  ,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, //  48 -  63
      // , A, B, C, D, E, F, G, H, I, J, K, L, M, N, O,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //  64 -  79
      //P, Q, R, S, T, U, V, W, X, Y, Z,  ,  ,  ,  , _,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, //  80 -  95
      // , a, b, c, d, e, f, g, h, i, j, k, l, m, n, o,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //  96 - 111
      //p, q, r, s, t, u, v, w, x, y, z,  ,  ,  ,  ,  ,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, // 112 - 127
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 128 - 143
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 144 - 159
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 160 - 175
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 176 - 191
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 192 - 207
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 208 - 223
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 224 - 239
      // ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // 240 - 255
      };

      inline
      bool need_encoding(int ch)
      {
        return (char_table[ch] == 0);
      }

      enum { space_char = ' ', plus_char = '+', percent_char = '%' };
    } // namespace <dict>

    template<
      typename outputT
    >
    class encode_iterator
      : public std::iterator<
          std::output_iterator_tag,
          void,
          void,
          void,
          void
        >
    {
    public:
      typedef encode_iterator<outputT>
        this_type;
    public:
      explicit
      encode_iterator(outputT output)
        : output_(output)
      {}
    public:
      this_type& operator=(char ch)
      {
        this->process(ch);
        return *this;
      }

      this_type& operator*()
      {
        return *this;
      }

      this_type& operator++()
      {
        return *this;
      }

      this_type& operator++(int)
      {
        return *this;
      }
    private:
      void process(char ch)
      {
        typedef unsigned char uchar_type;
        if(!dict::need_encoding(uchar_type(ch)))
        {
          *(output_++) = ch;
        }
        else
        {
          if(ch == dict::space_char)
          {
            *(output_++) = dict::plus_char;
          }
          else
          {
            *(output_++) = dict::percent_char;
            *(output_++) = dict::base16[(uchar_type(ch) >> 4) & 0x0f];
            *(output_++) = dict::base16[uchar_type(ch) & 0x0f];
          }
        }
      }
    private:
      outputT output_;
    }; // class <encode_iterator>

    template<
      typename iteratorT
    >
    inline
    encode_iterator<iteratorT> encoder(iteratorT const& output)
    {
      return encode_iterator<iteratorT>(output);
    }

    template<
      typename outputT
    >
    class decode_iterator
      : public std::iterator<
          std::output_iterator_tag,
          void,
          void,
          void,
          void
        >
    {
    public:
      typedef decode_iterator<outputT>
        this_type;
    public:
      explicit
      decode_iterator(outputT output)
        : impl_(new implementation(output))
      {}

      decode_iterator(decode_iterator const& rhs)
        : impl_(rhs.impl_)
      {
        if(impl_)
        {
          impl_->addref();
        }
      }

      decode_iterator& operator=(decode_iterator const& rhs)
      {
        if(this != &rhs)
        {
          if(impl_ && impl_->release() == 0)
          {
            delete impl_;
          }
          impl_ = rhs.impl_;
          if(impl_)
          {
            impl_->addref();
          }
        }
        return (*this);
      }

      ~decode_iterator()
      {
        if(impl_ && impl_->release() == 0)
        {
          impl_->sync();
          delete impl_;
        }
      }
    public:
      this_type& operator=(char ch)
      {
        impl_->process(ch);
        return *this;
      }

      this_type& operator*()
      {
        return *this;
      }

      this_type& operator++()
      {
        return *this;
      }

      this_type& operator++(int)
      {
        return *this;
      }

      this_type& sync()
      {
        if(impl_)
        {
          impl_->sync();
        }
        return *this;
      }
    private:
      struct implementation
      {
        enum state_type {
          regular,
          escape,
          half,
          decode_error
        };

        implementation(outputT o)
          : refcnt(1),
            state(regular),
            bits(0),
            output(o)
        {}

        int addref()
        {
          return ++refcnt;
        }

        int release()
        {
          return --refcnt;
        }

        void process(unsigned char ch)
        {
          if(state != decode_error)
          {
            switch(ch)
            {
              case dict::percent_char:
                if(state != regular)
                {
                  state = decode_error;
                }
                else
                {
                  state = escape;
                  bits = 0;
                }
                break;
              case dict::plus_char:
                if(state == regular)
                {
                  *(output++) = dict::space_char;
                }
                else
                {
                  state = decode_error;
                }
                break;
              default:
                switch(state)
                {
                case regular:
                  *(output++) = ch;
                  break;
                case escape:
                  if( dict::check_digit(ch) )
                  {
                    bits = dict::char_digit(ch);
                    state = half;
                  }
                  else
                  {
                    state = decode_error;
                  }
                  break;
                case half:
                  if( dict::check_digit(ch) )
                  {
                    *(output++) = ((bits << 4) | dict::char_digit(ch));
                    state = regular;
                  }
                  else
                  {
                    state = decode_error;
                  }
                } // switch(state)
            } // switch(ch)
          } // if(state != decoder_error)

          if( state == decode_error )
          {
            throw std::runtime_error( "invalid urlencoded sequence" );
          }
        }

        void sync()
        {
          state = regular;
          bits = 0;
        }

        int         refcnt;
        state_type  state;
        int         bits;
        outputT     output;
      }; // struct <implementation>

      implementation* impl_;
    }; // namespace <decode_iterator>

    template<
      typename iteratorT
    >
    inline
    decode_iterator<iteratorT> decoder(iteratorT const& output)
    {
      return decode_iterator<iteratorT>(output);
    }
  } // namespace <url>
} // namespace <util>

#endif // _UTIL_URL_ENCODE_ITERATOR_INCLUDED_