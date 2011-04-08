#include <event_model/detail/OidContainer.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <sstream>
#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>
#include <boost/cstdint.hpp>
//#include <event_model/detail/EventModelGrammar.h>
#include <gtest/gtest.h>
#include <string>
#include <parser/NamespaceParsers.h>
#include <event_model/Descriptors.h>
#include <gtest/gtest.h>
#include <boost/cstdint.hpp>
#include <event_model/detail/OidContainer.h>

#include <typeinfo>
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

#include <event_model/RowAndTable.h>

using namespace event_model;

TEST(row_table_test, test_one)
{
    ContainerBuilder<EventTypeContainer> container_builder;
    typedef ContainerBuilder<EventTypeContainer>::entry_type et;

    Oid oid0("monkey",0);
    Oid oid2("zebra",2);
    Oid oid3("donkey",3);
    Oid oid5("baboon",5);
   
    EventTypeDescriptor type_d0; 
    EventTypeDescriptor type_d2;
    EventTypeDescriptor type_d3;
    EventTypeDescriptor type_d5;

    type_d0.set_is_primitive(true);
    type_d0.set_qualifier(ENTRY_REQUIRED);
    type_d0.set_type(VALUE_INT4);
    
    type_d2.set_is_primitive(true);
    type_d2.set_qualifier(ENTRY_REQUIRED);
    type_d2.set_type(VALUE_STRING);
    
    type_d3.set_is_primitive(true);
    type_d3.set_qualifier(ENTRY_REQUIRED);
    type_d3.set_type(VALUE_INT8);

    type_d5.set_is_primitive(true);
    type_d5.set_qualifier(ENTRY_REQUIRED);
    type_d5.set_type(VALUE_STRING);

    container_builder.SetEntry(et(oid0,type_d0));
    container_builder.SetEntry(et(oid2,type_d2));
    container_builder.SetEntry(et(oid3,type_d3));
    container_builder.SetEntry(et(oid5,type_d5)); 
    
    ASSERT_EQ(container_builder.size(), 6);
    ASSERT_EQ(container_builder.occupied_size(),4);

    EventTypeContainer cont = container_builder;
    ASSERT_EQ(cont.size(), 6);
    ASSERT_EQ(cont.occupied_size(),4);

    table_descriptor td( Oid("test",0), cont);
    ASSERT_EQ(td.entry().size(), 6);
    ASSERT_EQ(td.entry().occupied_size(),4);

    Row row(td);
    ASSERT_EQ(RowTypeAt(td,0), VALUE_INT4);
    ASSERT_EQ(RowTypeAt(td,1), VALUE_UNINITIALIZED);
    ASSERT_NE(RowTypeAt(td,2), VALUE_UNINITIALIZED);
    ASSERT_EQ(RowTypeAt(td,3), VALUE_INT8);
    ASSERT_EQ(RowTypeAt(td,4), VALUE_UNINITIALIZED);
    ASSERT_NE(RowTypeAt(td,5), VALUE_UNINITIALIZED);
    
    std::string test_string("0(0=5 , 3=12423)");    
    //const std::string test_string("0(0=5, 2=\"jungle fever\", 3=124234, 5=\"boring\")");
    EXPECT_TRUE(row << test_string);
    const value_variant_vector & v = row.get_row_vector(); 
    
    ASSERT_EQ( boost::get<int32_t>( v[0]),5);
    ASSERT_EQ( boost::get<int64_t>( v[3]),12423);

    row.reset();

    test_string ="0(0=2147483647 , 3=12423)";
    EXPECT_TRUE(row << test_string);

    row.reset();

    test_string ="0(0=5, 3=9223372036854775807)";
    EXPECT_TRUE(row << test_string);

    row.reset();

    test_string ="0(0=2147483648, 3=12423)";
    EXPECT_FALSE(row << test_string);
    EXPECT_TRUE(row.parsing_error());
    EXPECT_TRUE(row.was_parsing_value());
    EXPECT_EQ(row.was_parsing_ordinal(),0);
    EXPECT_EQ(row.parsing_value_type(),VALUE_INT4);

    row.reset();

    test_string ="0(0=5, 3=9223372036854775808)";
    EXPECT_FALSE(row << test_string);
    EXPECT_TRUE(row.parsing_error());
    EXPECT_TRUE(row.was_parsing_value());
    EXPECT_EQ(row.was_parsing_ordinal(),3);
    EXPECT_EQ(row.parsing_value_type(),VALUE_INT8);
}

int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}