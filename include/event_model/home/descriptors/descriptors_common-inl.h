#ifndef RBL_EM_HOME_DESCRIPTORS_INL_COMMON_H
#define RBL_EM_HOME_DESCRIPTORS_INL_COMMON_H
#include "descriptors_common.h"
namespace rubble { namespace event_model {
// EventTypeDescriptor ////////////////////////////////////////////////////////
  inline EventTypeDescriptor::EventTypeDescriptor()
  : m_qualifier(ENTRY_UNINITIALIZED), 
    m_type(), 
    m_primitive(false) {} 

  template<typename T>
  inline EventTypeDescriptor::EventTypeDescriptor( 
    EVENT_DESCRIPTOR_QUALIFIER _qualifier, 
    T,
    bool primitive_in)
    : m_qualifier(_qualifier),
      m_type( get_type_ordinal_f<T>::pos::value ),
      m_primitive(primitive_in) {}

  inline const bool 
  EventTypeDescriptor::is_primitive() const {return m_primitive; } 

  inline const EVENT_DESCRIPTOR_QUALIFIER 
  EventTypeDescriptor::qualifier() const
    { return m_qualifier; }

  inline const type_ordinal_type EventTypeDescriptor::type() const 
  { 
    return m_type; 
  }

  inline void EventTypeDescriptor::set_is_primitive(bool _is_primitive)
  {
    m_primitive = _is_primitive;
  }
  inline void EventTypeDescriptor::set_qualifier
    (EVENT_DESCRIPTOR_QUALIFIER _qualifier)
  {
    m_qualifier = _qualifier;
  }
  template<typename T> 
  inline void EventTypeDescriptor::set_type(T)
  {
    m_type = T();
  }
  inline void EventTypeDescriptor::set_type_using_ordinal
  (const type_ordinal_type ordinal)
  {
    m_type = ordinal;
  }
  //---------------------------------------------------------------------------//

  // EventDescriptorBase ////////////////////////////////////////////////////////
  inline EventDescriptorBase::EventDescriptorBase() 
    : m_namespace_ordinal(), m_event_oid_type_pair()
  {
  }
  inline EventDescriptorBase::EventDescriptorBase
    ( const Oid & oid,
      const ordinal_type ordinal_,
      const EventTypeContainer & etc)
        : m_event_oid_type_pair(oid,etc), 
          m_namespace_ordinal (ordinal_) {}

  inline const std::size_t EventDescriptorBase::type_container_size() const
  {
    return m_event_oid_type_pair.entry().size();        
  }
  inline const ordinal_type EventDescriptorBase::ordinal() const
  {
    return m_event_oid_type_pair.Id().ordinal();
  }
  inline const OidName EventDescriptorBase::name() const
  {
    return m_event_oid_type_pair.Id().name();
  }

  inline const Oid & EventDescriptorBase::oid() const
  {
    return m_event_oid_type_pair.Id();
  }

  inline const Oid * EventDescriptorBase::TypeOidAt(const ordinal_type ordinal) const
  {
    const EventTypeContainer::entry_type * et = 
      m_event_oid_type_pair.entry().EntryAtordinal(ordinal);
    
    if(et != NULL)
      return & et->Id();
    else
      return NULL;
  }

  inline const Oid * EventDescriptorBase::TypeOidWithName(const OidName & name) const
  {
    const EventTypeContainer::entry_type * et = 
      m_event_oid_type_pair.entry().EntryWithName(name);
    
    if(et != NULL)
      return & et->Id();
    else
      return NULL;

  }

  inline const EventTypeDescriptor * EventDescriptorBase::
  TypeAt(const ordinal_type ordinal) const
  {
    return m_event_oid_type_pair.entry()[ordinal];
  }

  inline const type_ordinal_type 
  EventDescriptorBase::TypeValueTypeAt(const ordinal_type ordinal) const
  {
    const EventTypeDescriptor * etd = m_event_oid_type_pair.entry()[ordinal];
    if( etd == NULL)
      return get_type_ordinal_f<rbl_undefined>::pos::value;
    else
      return etd->type();
  }

  inline const std::size_t EventDescriptorBase::
  type_container_occupied_size() const
  {
    return m_event_oid_type_pair.entry().occupied_size();
  } 
  //-----------------------------------------------------------------------//
    
  // NamespaceDescriptorBase ////////////////////////////////////////////////
  template<class EventDescriptor>
  inline NamespaceDescriptorBase<EventDescriptor>
  ::NamespaceDescriptorBase()
  {

  }

  template<class EventDescriptor> 
  inline NamespaceDescriptorBase<EventDescriptor>::
  NamespaceDescriptorBase(  const std::string & name_in,
                            const ordinal_type ordinal)
    : m_name(name_in), m_ordinal(ordinal), m_events()
  {
  }

  template<class EventDescriptor> 
  inline NamespaceDescriptorBase<EventDescriptor>::
  NamespaceDescriptorBase(  const std::string & name_in,
                            const ordinal_type ordinal,
                            const t_edc & con)
    : m_name(name_in), m_ordinal(ordinal), m_events(con)
  {
  }

  template<class EventDescriptor>
  inline const std::string & NamespaceDescriptorBase<EventDescriptor>
  ::name()
  {
    return m_name;
  }
  template<class EventDescriptor>
  inline const ordinal_type NamespaceDescriptorBase<EventDescriptor>
  ::ordinal()
  {
    return m_ordinal;
  }

  template<class EventDescriptor>
  inline const std::size_t NamespaceDescriptorBase<EventDescriptor>::
  event_container_size() const
  {
    return m_events.size();
  }
  template<class EventDescriptor>
  inline const std::size_t NamespaceDescriptorBase<EventDescriptor>::
  event_container_occupied_size() const
  {
    return  m_events.occupied_size();    
  }
  template<class EventDescriptor>
  inline const OP_RESPONSE NamespaceDescriptorBase<EventDescriptor>::
  ContainsEventIdentifier(const Oid & oid) const
  {
    return m_events.ContainsEither(oid); 
  }

  template<class EventDescriptor>
  inline const EventDescriptor *
  NamespaceDescriptorBase<EventDescriptor>::
  EventAt(const ordinal_type ordinal) const
  {
    return m_events[ordinal];        
  }
  template<class EventDescriptor>
  inline const EventDescriptor *
  NamespaceDescriptorBase<EventDescriptor>::
  EventWithName(const OidName & name_in) const
  {
    return m_events[name_in];
  }

  template<class EventDescriptor>
  inline const typename NamespaceDescriptorBase<EventDescriptor>::t_edc &
  NamespaceDescriptorBase<EventDescriptor>::events() const
  {
    return m_events;
  }
//---------------------------------------------------------------------------//
} } 
#endif
