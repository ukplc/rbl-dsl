#include <event_processing/detail/event_string_generator.h>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

event_string_generator_grammar::event_string_generator_grammar()
    : event_string_generator_grammar::base_type(base_rule),
      current_value_type(event_model::VALUE_UNINITIALIZED),
      current_ordinal(0)
{
    using karma::eps;
    using karma::char_;
    using karma::int_;
    using karma::_1;
    using karma::_r1;
    using karma::_r2;
    using karma::_a;
    using karma::_b;
    using karma::repeat;
    using event_model::value_variant_vector;
    using event_model::EventDescriptor;
    using namespace event_model; 
//    entry_rule = 
   
    #define _CO                                                             \
    phoenix::bind(&event_string_generator_grammar::current_ordinal,*this)

    #define _CVT_CURRENT(ordinal)                                           \
    phoenix::bind(&EventDescriptor::RowTypeAt, _r1, ordinal)
    
    #define _CVT_LOCAL                                                      \
    phoenix::bind(&event_string_generator_grammar::current_value_type,*this)
    

    base_rule = 
        eps  [ _a = phoenix::bind(&value_variant_vector::size, _r2)] <<
        int_ [ _1 = phoenix::bind(&EventDescriptor::ordinal, _r1) ] <<
        char_('(') <<
        repeat(_a) [
            eps[_CVT_LOCAL = _CVT_CURRENT(_CO)]  << 
            ( eps( _CVT_LOCAL != VALUE_UNINITIALIZED) <<
              int_[_1=_CO] << char_("=") << 
                (
                     (  eps(_CVT_LOCAL == VALUE_INT4)
                        //<< int_[phoenix::val(1)];
                     )
                   | (  eps(_CVT_LOCAL == VALUE_INT8)
                     )
                   | (  eps(_CVT_LOCAL == VALUE_STRING)
                     )
                 )
              | eps
            )
            << eps[ _CO = _CO+1 ]
            << karma::auto_(_b)
        ] <<
        char_(')')
    ;
}