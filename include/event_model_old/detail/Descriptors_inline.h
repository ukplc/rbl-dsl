
///////////////////////////////////////////////////////////////////////////////
inline EventDescriptor::EventDescriptor( const Oid & oid, 
                                    const EventTypeContainer & etc)
    : base(oid,etc) {}

inline const VALUE_TYPE 
EventDescriptor::RowTypeAt(const uint32_t ordinal) const
{
    const EventTypeContainer::entry_type * etd_e = 
        entry().EntryAtordinal(ordinal);
    
    if( etd_e == NULL)
    {
        return VALUE_UNINITIALIZED;
    }
    else return etd_e->entry().type();
}
///////////////////////////////////////////////////////////////////////////////
inline GeneratorEventDescriptor::GeneratorEventDescriptor()
    :  types() {}

inline GeneratorEventDescriptor::GeneratorEventDescriptor
    ( const EventTypeContainer & etc) : types(etc) {}

inline void GeneratorEventDescriptor::serialize(SF::Archive & ar)
{ ar & types; }
///////////////////////////////////////////////////////////////////////////////
inline RelayEventDescriptor::RelayEventDescriptor()
    :  types() {}

inline RelayEventDescriptor::RelayEventDescriptor
    (const  EventTypeContainer & etc) : types(etc) {}

inline RelayEventDescriptor::RelayEventDescriptor
    (const MarshallEventDescriptor & rhs)
    : types(rhs.types) {}

inline RelayEventDescriptor::operator GeneratorEventDescriptor() const
    { return GeneratorEventDescriptor(types); }

inline void RelayEventDescriptor::serialize(SF::Archive & ar)
    { ar & types;}
///////////////////////////////////////////////////////////////////////////////
inline MarshallEventDescriptor::MarshallEventDescriptor
        (const EventTypeContainer & etc)
    : types(etc) {}

inline MarshallEventDescriptor::MarshallEventDescriptor()
    : types() {}

inline MarshallEventDescriptor::operator RelayEventDescriptor() const
    { return RelayEventDescriptor(types);  }
///////////////////////////////////////////////////////////////////////////////
inline MarshallEventDescriptorBuilder::MarshallEventDescriptorBuilder()
    : types() {}

inline MarshallEventDescriptorBuilder::operator MarshallEventDescriptor() const
    { return MarshallEventDescriptor(types); }

inline const Oid & MarshallEventDescriptorBuilder::oid() const
    { return self_oid_; }
///////////////////////////////////////////////////////////////////////////////
inline GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor()
    : name_(), events() {}

inline GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor
    (const std::string & name, const RelayEDC & events)
        : name_(name), events(events) {}

inline void GeneratorNamespaceDescriptor::serialize(SF::Archive & ar)
    { ar & name_ & events; }

inline const std::string & GeneratorNamespaceDescriptor::name() const
    { return name_; }

///////////////////////////////////////////////////////////////////////////////
inline RelayNamespaceDescriptor::RelayNamespaceDescriptor()
    : name_(), events() {}
inline RelayNamespaceDescriptor::RelayNamespaceDescriptor
    (const std::string & name , const MarshallEDC & events) 
        : name_(name),  events(events) {}
inline RelayNamespaceDescriptor::operator GeneratorNamespaceDescriptor() const
{
    return GeneratorNamespaceDescriptor(name_, events);
}
inline void RelayNamespaceDescriptor::serialize(SF::Archive & ar)
    { ar & name_ & events;   }
inline const std::string & RelayNamespaceDescriptor::name() const
    { return name_; }
///////////////////////////////////////////////////////////////////////////////
inline MarshallNamespaceDescriptor::MarshallNamespaceDescriptor(
    const std::string & name, 
    const MarshallEDC & EDC)
        : name_(name), events(EDC){}


inline MarshallNamespaceDescriptor::operator RelayNamespaceDescriptor() const
{
    return RelayNamespaceDescriptor(name_,events);
}

inline const std::string & MarshallNamespaceDescriptor::name() const
    { return name_; }

///////////////////////////////////////////////////////////////////////////////
inline MarshallNamespaceDescriptorBuilder::MarshallNamespaceDescriptorBuilder()
    : name_(),events() { }

inline MarshallNamespaceDescriptorBuilder::MarshallNamespaceDescriptorBuilder
    (const std::string & s) : name_(s)  { }

inline void MarshallNamespaceDescriptorBuilder::AddEventDescriptor
    (const MarshallEventDescriptorBuilder & medb, bool & ok)
{
    ok = true;
    EventDescriptorContainer::entry_type entry
        (medb.oid(), (MarshallEventDescriptor) medb);
    
    if( events.SetEntry(entry) != OP_NO_ERROR)
        ok = false; 
}

inline MarshallNamespaceDescriptorBuilder::operator 
    MarshallNamespaceDescriptor() const
{
    return MarshallNamespaceDescriptor(name_,events);
}

inline const std::string & MarshallNamespaceDescriptorBuilder::name() const
    { return name_; }
inline void MarshallNamespaceDescriptorBuilder::set_name(std::string & _name)
    { name_ = _name; }
inline const OP_RESPONSE MarshallNamespaceDescriptorBuilder::ContainsEventIdentifier
    (const Oid & id) const{ return events.ContainsEither(id); }
///////////////////////////////////////////////////////////////////////////////