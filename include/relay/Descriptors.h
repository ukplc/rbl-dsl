#ifndef _RELAY_DESCRIPTORS_H
#define _RELAY_DESCRIPTORS_H
#include "event_model/DescriptorsCommon.h"
#include "generator/Descriptors.h"

//TODO centralize the SF forward declarations
namespace SF
{
    class Archive;
}

namespace event_model
{
// RelayEventDescriptor ///////////////////////////////////////////////////
class RelayEventDescriptor : public EventDescriptorBase
{
public:
  RelayEventDescriptor();
  RelayEventDescriptor (  const Oid & oid, const ordinal_type ordinal_, 
                          const EventTypeContainer & etc);
  operator GeneratorEventDescriptor() const;
  friend void serialize(SF::Archive & ar,RelayEventDescriptor & red);
};
//-----------------------------------------------------------------------//
    
// RelayNamespaceDescriptor ///////////////////////////////////////////////
class RelayNamespaceDescriptor : 
  public  NamespaceDescriptorBase<RelayEventDescriptor>
{
public:
  RelayNamespaceDescriptor();
  RelayNamespaceDescriptor(   const  std::string & name_in,
                              const  ordinal_type ordinal_in,
                              const  EventDescriptorContainer & edc_in);
  operator GeneratorNamespaceDescriptor() const;

  friend void serialize(SF::Archive & ar, RelayNamespaceDescriptor & rnd);
};
//-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
    // RelayEventDescriptor ///////////////////////////////////////////////////
    inline RelayEventDescriptor::RelayEventDescriptor()
    {
    }
    inline RelayEventDescriptor::
    RelayEventDescriptor ( const Oid & oid,
                           const ordinal_type ordinal_,
                           const EventTypeContainer & etc)
        : EventDescriptorBase(oid,ordinal_, etc) 
    {
    }
    inline RelayEventDescriptor::operator GeneratorEventDescriptor() const
    {
        GeneratorEventDescriptor ged(   event_oid_type_pair_.Id(),
                                        namespace_ordinal_,
                                        event_oid_type_pair_.entry());
        return ged;
    }

    //-----------------------------------------------------------------------//
    // RelayNamespaceDescriptor ///////////////////////////////////////////////
    RelayNamespaceDescriptor::RelayNamespaceDescriptor()
        : NamespaceDescriptorBase<RelayEventDescriptor>() 
    {
    }
    RelayNamespaceDescriptor::RelayNamespaceDescriptor
        ( const  std::string & name_in, 
          const  ordinal_type ordinal_in, 
          const EventDescriptorContainer & edc_in)
            : NamespaceDescriptorBase<RelayEventDescriptor>(name_in, ordinal_in,edc_in)
    {
    }
    RelayNamespaceDescriptor::operator GeneratorNamespaceDescriptor() const
    {
        GeneratorNamespaceDescriptor::EventDescriptorContainer gedc;
        events_.SlicingPopulate(gedc);
        GeneratorNamespaceDescriptor casted(name_,ordinal_,gedc);
        return casted;    
    }
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//
}
#endif
