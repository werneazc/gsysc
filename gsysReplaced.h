/*
** This file is part of gSysC.
**
** gSysC is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** gSysC is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with gSysC with the file ``LICENSE''; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*****************************************************************************
    ``Author'': Christian J. Eibl
    last modified: 2005-01-31
 *****************************************************************************
    Content:
      This class is a reimplementation or just a copy of the SystemC classes
      sc_in, sc_out, sc_inout und sc_signal. Copying these classes to gsys_in,
      gsys_out, gsys_inout and gsys_signal was necessary due to the work
      with the templates. Now gSysC can access the current values of the
      gsys_Signals. 
      Simply deriving did not work ...
      
 *****************************************************************************/
				      
/*****************************************************************************
    Most of the following source code is taken unchanged from
    the SystemC classe. Therefore the license statement 
    of SystemC and the authors are given below, too!
 *****************************************************************************/
					  
/*****************************************************************************
 *****************************************************************************/
					   
#ifndef GSYS_SIGNALPORT_H
#define GSYS_SIGNALPORT_H



//=====================================================================================//

/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2002 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 2.3 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 ******************************************************************************/

/*****************************************************************************
 *
 *   sc_signal.h -- The sc_signal<T> primitive channel class.
 *
 *   Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21
 *
 ******************************************************************************/

/*****************************************************************************
 *
 *   MODIFICATION LOG - modifiers, enter your name, affiliation, date and
 *   changes you are making here.
 *
 *   Name, Affiliation, Date:
 *   Description of Modification:
 *
 ******************************************************************************/

/*****************************************************************************
 *
 *   sc_signal_ports.h -- The sc_signal<T> port classes.
 *
 *   Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21
 *
 ******************************************************************************/

/*****************************************************************************
 *
 *   MODIFICATION LOG - modifiers, enter your name, affiliation, date and
 *   changes you are making here.
 *
 *   Name, Affiliation, Date:  Jason Elbaum, Motorola, Inc., 2001-11-12
 *   Description of Modification:  Added a static, private, otherwise
 *                                 unused data member to the sc_in
 *                                 and sc_inout classes to address
 *                                 a bug in the GNU compiler *only*.
 *                                 This works around a bug in g++ 2.95.2
 *                                 regarding implicit casting from a
 *                                 templated class to a C++ intrinsic type.
 *
 ******************************************************************************/


#include <sysc/communication/sc_port.h>
#include <sysc/communication/sc_prim_channel.h>
#include <sysc/communication/sc_signal_ifs.h>
#include <sysc/communication/sc_event_finder.h>
#include <sysc/kernel/sc_event.h>
#include <sysc/kernel/sc_process_handle.h>
#include <sysc/kernel/sc_simcontext.h>
#include <sysc/datatypes/bit/sc_logic.h>
#include <sysc/tracing/sc_trace.h>
#include <sysc/utils/sc_string.h>
#include <typeinfo>
#include "gsysSignal.h"
#include "gsysPort.h"
#include "gsysMain.h"
#include "gsysRegister.h"
#include <stdio.h>
#include <sstream>

using sc_dt::sc_logic;
using sc_dt::SC_LOGIC_0;
using sc_dt::SC_LOGIC_1;
using sc_dt::SC_LOGIC_Z;
using sc_dt::SC_LOGIC_X;


// to avoid code bloat in gsys_signal<T>

extern
void
sc_signal_invalid_writer( const char* name,
			  const char* kind,
			  const char* first_writer,
			  const char* second_writer );


// ----------------------------------------------------------------------------
//  CLASS : gsys_signal<T>
//
//  The gsys_signal<T> primitive channel class.
// ----------------------------------------------------------------------------

template <class T>
class gsys_signal
: public sc_signal_inout_if<T>,
  public sc_prim_channel
{
private:
    gsysSignal *gSignal;
    
public:

    // constructors

    gsys_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
          m_output( 0 ), m_cur_val( T() ), m_new_val( T() ),
          m_delta( ~sc_dt::UINT64_ONE ), m_writer( 0 )
	{
          gSignal = new gsysSignal();
          gSignal->setName("unnamedSignal");
          (new gsysMain())->getRegModule()->registerSignal(gSignal,this);
	}

    explicit gsys_signal( const char* name_ )
	: sc_prim_channel( name_ ),
          m_output( 0 ), m_cur_val( T() ), m_new_val( T() ),
          m_delta( ~sc_dt::UINT64_ONE ), m_writer( 0 )
	{
          gSignal = new gsysSignal();
          gSignal->setName((char*) name_);
          (new gsysMain())->getRegModule()->registerSignal(gSignal,this);
	}


    // destructor (does nothing)

    virtual ~gsys_signal()
	{}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ return m_value_changed_event; }


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ return m_value_changed_event; }


    // read the current value
    virtual const T& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const T& get_data_ref() const
        { return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return ( simcontext()->delta_count() == m_delta + 1 ); }

    // write the new value
    virtual void write( const T& );


    // other methods

    operator const T& () const
	{ return read(); }


    gsys_signal<T>& operator = ( const T& a )
	{ write( a ); return *this; }

    gsys_signal<T>& operator = ( const gsys_signal<T>& a )
	{ write( a.read() ); return *this; }


    const T& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
#ifdef DEBUG_SYSTEMC
	{ ::sc_trace( tf, get_data_ref(), name() ); }
#else
	{}
#endif


    virtual void print( ostream& ) const;
    virtual void dump( ostream& ) const;


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    virtual void update();

    void check_writer();

protected:

    sc_port_base* m_output; // used for static design rule checking

    T             m_cur_val;
    T             m_new_val;

    sc_event      m_value_changed_event;

    uint64        m_delta; // delta of last event

    sc_process_b* m_writer; // used for dynamic design rule checking

private:

    // disabled
    gsys_signal( const gsys_signal<T>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T>
const char* const gsys_signal<T>::kind_string = "gsys_signal";


template <class T>
inline
void
gsys_signal<T>::register_port( sc_port_base& port_, const char* if_typename_ )
{
#ifdef DEBUG_SYSTEMC
    sc_string nm( if_typename_ );
    if( nm == typeid( sc_signal_inout_if<T> ).name() ) {
	// an out or inout port; only one can be connected
	if( m_output != 0 ) {
	    sc_signal_invalid_writer( name(), kind(),
				      m_output->name(), port_.name() );
	}
	m_output = &port_;
    }
#endif
}


// write the new value

template <class T>
inline
void
gsys_signal<T>::write( const T& value_ )
{
#ifdef DEBUG_SYSTEMC
    check_writer();
#endif
    #ifdef DEBUG_GSYSC
    std::cout<<"\nGSYS_SIGNAL ("<<gSignal<<") write-call with parameter "<<value_<<";  a Read() before this gives '"<<read()<<"'"<<std::endl;
    #endif
	    
    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }

    ostringstream ostr;
    ostr << value_ << ends;
    gSignal->setValue((char*) ostr.str().c_str());
    #ifdef DEBUG_GSYSC
    std::cout<<"Read() after write gives '"<<read()<<"', ostr.str(): "<<ostr.str()<<",  as C-String: "<<ostr.str().c_str()<<std::endl<<std::endl;
    #endif
}


template <class T>
inline
void
gsys_signal<T>::print( ostream& os ) const
{
    os << m_cur_val;
}

template <class T>
inline
void
gsys_signal<T>::dump( ostream& os ) const
{
    os << "     name = " << name() << endl;
    os << "    value = " << m_cur_val << endl;
    os << "new value = " << m_new_val << endl;
}


template <class T>
inline
void
gsys_signal<T>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_cur_val = m_new_val;
	m_value_changed_event.notify_delayed();
	m_delta = simcontext()->delta_count();
    }
}


template <class T>
inline
void
gsys_signal<T>::check_writer()
{
    sc_process_b* writer = sc_get_curr_process_handle();
    if( m_writer == 0 ) {
	m_writer = writer;
    } else if( m_writer != writer ) {
	sc_signal_invalid_writer( name(), kind(),
				  m_writer->name(), writer->name() );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : gsys_signal<bool>
//
//  Specialization of gsys_signal<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class gsys_signal<bool>
: public sc_signal_inout_if<bool>,
  public sc_prim_channel
{
private:
    gsysSignal *gSignal;
    
public:

    // constructors

    gsys_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
          m_output( 0 ),
          m_cur_val( false ),
          m_new_val( false ),
          m_delta( ~sc_dt::UINT64_ONE ),
          m_writer( 0 )
	{
            gSignal = new gsysSignal();
            gSignal->setName("unnamedSignal");
            (new gsysMain())->getRegModule()->registerSignal(gSignal,this);
	}

    explicit gsys_signal( const char* name_ )
	: sc_prim_channel( name_ ),
          m_output( 0 ),
          m_cur_val( false ),
          m_new_val( false ),
          m_delta( ~sc_dt::UINT64_ONE ),
          m_writer( 0 )
	{
          gSignal = new gsysSignal();
	  gSignal->setName((char*) name_);
	  (new gsysMain())->getRegModule()->registerSignal(gSignal,this);
	}


    // destructor (does nothing)

    virtual ~gsys_signal()
	{}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ return m_value_changed_event; }


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ return m_value_changed_event; }

    // get the positive edge event
    virtual const sc_event& posedge_event() const
	{ return m_posedge_event; }

    // get the negative edge event
    virtual const sc_event& negedge_event() const
	{ return m_negedge_event; }


    // read the current value
    virtual const bool& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const bool& get_data_ref() const
        { return m_cur_val; }


    // was there a value changed event?
    virtual bool event() const
        { return ( simcontext()->delta_count() == m_delta + 1 ); }

    // was there a positive edge event?
    virtual bool posedge() const
	{ return ( event() && m_cur_val ); }

    // was there a negative edge event?
    virtual bool negedge() const
	{ return ( event() && ! m_cur_val ); }


    // write the new value
    virtual void write( const bool& );


    // delayed evaluation
    virtual const sc_signal_bool_deval& delayed() const;


    // other methods

    operator const bool& () const
	{ return read(); }


    gsys_signal<bool>& operator = ( const bool& a )
	{ write( a ); return *this; }

    gsys_signal<bool>& operator = ( const gsys_signal<bool>& a )
	{ write( a.read() ); return *this; }


    const bool& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
#ifdef DEBUG_SYSTEMC
	{ ::sc_trace( tf, get_data_ref(), name() ); }
#else
	{}
#endif


    virtual void print( ostream& ) const;
    virtual void dump( ostream& ) const;


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    virtual void update();

    void check_writer();

protected:

    sc_port_base* m_output; // used for static design rule checking

    bool          m_cur_val;
    bool          m_new_val;

    sc_event      m_value_changed_event;
    sc_event      m_posedge_event;
    sc_event      m_negedge_event;

    uint64        m_delta; // delta of last event

    sc_process_b* m_writer; // used for dynamic design rule checking

private:

    // disabled
    gsys_signal( const gsys_signal<bool>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
gsys_signal<bool>::register_port( sc_port_base& port_, const char* if_typename_ )
{
#ifdef DEBUG_SYSTEMC
    sc_string nm( if_typename_ );
    if( nm == typeid( sc_signal_inout_if<bool> ).name() ) {
	// an out or inout port; only one can be connected
	if( m_output != 0 ) {
	    sc_signal_invalid_writer( name(), kind(),
				      m_output->name(), port_.name() );
	}
	m_output = &port_;
    }
#endif
}


// write the new value

inline
void
gsys_signal<bool>::write( const bool& value_ )
{
#ifdef DEBUG_SYSTEMC
    check_writer();
#endif
    #ifdef DEBUG_GSYSC
    std::cout<<"\nGSYS_SIGNAL ("<<gSignal<<") write-call with parameter "<<value_<<";  a Read() before this gives '"<<read()<<"'"<<std::endl;
    #endif
	    
    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }

    ostringstream ostr;
    ostr << value_ << ends;
    gSignal->setValue((char*) ostr.str().c_str());
    #ifdef DEBUG_GSYSC
    std::cout<<"Read() after write gives '"<<read()<<"', ostr.str(): "<<ostr.str()<<",  as C-String: "<<ostr.str().c_str()<<std::endl<<std::endl;
    #endif
}


// delayed evaluation

inline
const sc_signal_bool_deval&
gsys_signal<bool>::delayed() const
{
    const sc_signal_in_if<bool>* iface = this;
    return reinterpret_cast<const sc_signal_bool_deval&>( *iface );
}


inline
void
gsys_signal<bool>::print( ostream& os ) const
{
    os << m_cur_val;
}

inline
void
gsys_signal<bool>::dump( ostream& os ) const
{
    os << "     name = " << name() << endl;
    os << "    value = " << m_cur_val << endl;
    os << "new value = " << m_new_val << endl;
}


inline
void
gsys_signal<bool>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_cur_val = m_new_val;
	m_value_changed_event.notify_delayed();
	if( m_cur_val ) {
	    m_posedge_event.notify_delayed();
	} else {
	    m_negedge_event.notify_delayed();
	}
	m_delta = simcontext()->delta_count();
    }
}


inline
void
gsys_signal<bool>::check_writer()
{
    sc_process_b* writer = sc_get_curr_process_handle();
    if( m_writer == 0 ) {
	m_writer = writer;
    } else if( m_writer != writer ) {
	sc_signal_invalid_writer( name(), kind(),
				  m_writer->name(), writer->name() );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : gsys_signal<sc_logic>
//
//  Specialization of gsys_signal<T> for type sc_logic.
// ----------------------------------------------------------------------------

template <>
class gsys_signal<sc_logic>
: public sc_signal_inout_if<sc_logic>,
  public sc_prim_channel
{
private:
    gsysSignal *gSignal;
    
public:

    // constructors

    gsys_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
          m_output( 0 ),
	  m_cur_val(),
	  m_new_val(),
          m_delta( ~sc_dt::UINT64_ONE ),
	  m_writer( 0 )
	{
            gSignal = new gsysSignal();
            gSignal->setName("unnamedSignal");
            (new gsysMain())->getRegModule()->registerSignal(gSignal,this);
	}

    explicit gsys_signal( const char* name_ )
	: sc_prim_channel( name_ ),
          m_output( 0 ),
	  m_cur_val(),
	  m_new_val(),
          m_delta( ~sc_dt::UINT64_ONE ),
	  m_writer( 0 )
	{
           gSignal = new gsysSignal();
           gSignal->setName((char*) name_);
           (new gsysMain())->getRegModule()->registerSignal(gSignal,this);
	}


    // destructor (does nothing)

    virtual ~gsys_signal()
	{}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ return m_value_changed_event; }


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ return m_value_changed_event; }

    // get the positive edge event
    virtual const sc_event& posedge_event() const
	{ return m_posedge_event; }

    // get the negative edge event
    virtual const sc_event& negedge_event() const
	{ return m_negedge_event; }


    // read the current value
    virtual const sc_logic& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const sc_logic& get_data_ref() const
        { return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return ( simcontext()->delta_count() == m_delta + 1 ); }

    // was there a positive edge event?
    virtual bool posedge() const
	{ return ( event() && m_cur_val == SC_LOGIC_1 ); }

    // was there a negative edge event?
    virtual bool negedge() const
	{ return ( event() && m_cur_val == SC_LOGIC_0 ); }


    // write the new value
    virtual void write( const sc_logic& );


    // delayed evaluation
    virtual const sc_signal_logic_deval& delayed() const;


    // other methods

    operator const sc_logic& () const
	{ return read(); }


    gsys_signal<sc_logic>& operator = ( const sc_logic& a )
	{ write( a ); return *this; }

    gsys_signal<sc_logic>& operator = ( const gsys_signal<sc_logic>& a )
	{ write( a.read() ); return *this; }


    const sc_logic& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
#ifdef DEBUG_SYSTEMC
	{ ::sc_trace( tf, get_data_ref(), name() ); }
#else
	{}
#endif


    virtual void print( ostream& ) const;
    virtual void dump( ostream& ) const;


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    virtual void update();

    void check_writer();

protected:

    sc_port_base* m_output; // used for static design rule checking

    sc_logic      m_cur_val;
    sc_logic      m_new_val;

    sc_event      m_value_changed_event;
    sc_event      m_posedge_event;
    sc_event      m_negedge_event;

    uint64        m_delta; // delta of last event

    sc_process_b* m_writer; // used for dynamic design rule checking

private:

    // disabled
    gsys_signal( const gsys_signal<sc_logic>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
gsys_signal<sc_logic>::register_port( sc_port_base& port_,
				    const char* if_typename_ )
{
#ifdef DEBUG_SYSTEMC
    sc_string nm( if_typename_ );
    if( nm == typeid( sc_signal_inout_if<sc_logic> ).name() ) {
	// an out or inout port; only one can be connected
	if( m_output != 0 ) {
	    sc_signal_invalid_writer( name(), kind(),
				      m_output->name(), port_.name() );
	}
	m_output = &port_;
    }
#endif
}


// write the new value

inline
void
gsys_signal<sc_logic>::write( const sc_logic& value_ )
{
#ifdef DEBUG_SYSTEMC
    check_writer();
#endif
    #ifdef DEBUG_GSYSC
    std::cout<<"\nGSYS_SIGNAL ("<<gSignal<<") write-call with parameter "<<value_<<";  a Read() before this gives '"<<read()<<"'"<<std::endl;
    #endif
	   
    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }

    ostringstream ostr;
    ostr << value_ << ends;
    gSignal->setValue((char*) ostr.str().c_str());
    #ifdef DEBUG_GSYSC
    std::cout<<"Read() after write gives '"<<read()<<"', ostr.str(): "<<ostr.str()<<",  as C-String: "<<ostr.str().c_str()<<std::endl<<std::endl;
    #endif
}


// delayed evaluation

inline
const sc_signal_logic_deval&
gsys_signal<sc_logic>::delayed() const
{
    const sc_signal_in_if<sc_logic>* iface = this;
    return reinterpret_cast<const sc_signal_logic_deval&>( *iface );
}


inline
void
gsys_signal<sc_logic>::print( ostream& os ) const
{
    os << m_cur_val;
}

inline
void
gsys_signal<sc_logic>::dump( ostream& os ) const
{
    os << "     name = " << name() << endl;
    os << "    value = " << m_cur_val << endl;
    os << "new value = " << m_new_val << endl;
}


inline
void
gsys_signal<sc_logic>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_cur_val = m_new_val;
	m_value_changed_event.notify_delayed();
	if( m_cur_val == SC_LOGIC_1 ) {
	    m_posedge_event.notify_delayed();
	} else if( m_cur_val == SC_LOGIC_0 ) {
	    m_negedge_event.notify_delayed();
	}
	m_delta = simcontext()->delta_count();
    }
}



inline
void
gsys_signal<sc_logic>::check_writer()
{
    sc_process_b* writer = sc_get_curr_process_handle();
    if( m_writer == 0 ) {
	m_writer = writer;
    } else if( m_writer != writer ) {
	sc_signal_invalid_writer( name(), kind(),
				  m_writer->name(), writer->name() );
    }
}


// ----------------------------------------------------------------------------

template <class T>
inline
ostream&
operator << ( ostream& os, const gsys_signal<T>& a )
{
    return ( os << a.read() );
}



//typedef sc_pvector<sc_trace_params*> sc_trace_params_vec;


// ----------------------------------------------------------------------------
//  CLASS : gsys_in<T>
//
//  The gsys_signal<T> input port class.
// ----------------------------------------------------------------------------

template <class T>
class gsys_in   : public sc_in<T>
//: public sc_port<sc_signal_in_if<T>,1>
{
public:

    // typedefs

    typedef T                             data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef gsys_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

private:
    gsysPort *gPort;
    
public:

    // constructors

    gsys_in()
	: /*base_type()*/ sc_in<T>(), m_traces( 0 )
	{
           gPort = new gsysPort(0);
           gPort->setName("unnamedPort");
           (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_in( const char* name_ )
	: /*base_type( name_ )*/ sc_in<T>(name_), m_traces( 0 )
	{
          gPort = new gsysPort(0);
          gPort->setName((char*) name_);
          (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_in( const in_if_type& interface_ )
        : /*base_type( const_cast<in_if_type&>( interface_ ) )*/ sc_in<T>(interface_), m_traces( 0 )
        {}

    gsys_in( const char* name_, const in_if_type& interface_ )
	: /*base_type( name_, const_cast<in_if_type&>( interface_ ) )*/ sc_in<T>(name_,interface_), m_traces( 0 )
	{}

    explicit gsys_in( in_port_type& parent_ )
	: /*base_type( parent_ )*/ sc_in<T>(parent_), m_traces( 0 )
	{}

    gsys_in( const char* name_, in_port_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_in<T>(name_,parent_), m_traces( 0 )
	{}

    explicit gsys_in( inout_port_type& parent_ )
	: /*base_type()*/ sc_in<T>(parent_), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    gsys_in( const char* name_, inout_port_type& parent_ )
	: /*base_type( name_ )*/ sc_in<T>(name_,parent_), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    gsys_in( this_type& parent_ )
	: /*base_type( parent_ )*/ sc_in<T>(parent_), m_traces( 0 )
	{}

    gsys_in( const char* name_, this_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_in<T>(name_,parent_), m_traces( 0 )
	{}


    // destructor

    virtual ~gsys_in()
	{ remove_traces(); }


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( const_cast<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( const_cast<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }


    // called by sc_trace
    void add_trace( sc_trace_file*, const sc_string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
    gsys_in( const this_type& );
    this_type& operator = ( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T>
const char* const gsys_in<T>::kind_string = "gsys_in";


// called when elaboration is done

template <class T>
inline
void
gsys_in<T>::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = dynamic_cast<in_if_type*>( this->get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <class T>
inline
void
gsys_in<T>::add_trace( sc_trace_file* tf_, const sc_string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	static std::string tmp_str(name_);
	m_traces->push_back( new sc_trace_params( tf_, tmp_str ) );
    }
}

template <class T>
inline
void
gsys_in<T>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// called by pbind (for internal use only)

template <class T>
inline
int
gsys_in<T>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

template <class T>
inline
int
gsys_in<T>::vbind( sc_port_base& parent_ )
{
    in_port_type* in_parent = dynamic_cast<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = dynamic_cast<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


// ----------------------------------------------------------------------------
//  CLASS : gsys_in<bool>
//
//  Specialization of gsys_in<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class gsys_in<bool> : public sc_in<bool>
//: public sc_port<sc_signal_in_if<bool>,1>
{
public:

    // typedefs

    typedef bool                          data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef gsys_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

private:
    gsysPort *gPort;
    
public:

    // constructors

    gsys_in()
	: /*base_type()*/ sc_in<bool>(), m_traces( 0 )
	{
            gPort = new gsysPort(0);
            gPort->setName("unnamedPort");
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_in( const char* name_ )
	: /*base_type( name_ )*/ sc_in<bool>(name_), m_traces( 0 )
	{
            gPort = new gsysPort(0);
            gPort->setName((char*) name_);
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_in( const in_if_type& interface_ )
	: /*base_type( const_cast<in_if_type&>( interface_ ) )*/ sc_in<bool>(interface_), m_traces( 0 )
	{}

    gsys_in( const char* name_, const in_if_type& interface_ )
	: /*base_type( name_, const_cast<in_if_type&>( interface_ ) )*/ sc_in<bool>(name_,interface_), m_traces( 0 )
	{}

    explicit gsys_in( in_port_type& parent_ )
	: /*base_type( parent_ )*/ sc_in<bool>(parent_), m_traces( 0 )
	{}

    gsys_in( const char* name_, in_port_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_in<bool>(name_,parent_), m_traces( 0 )
	{}

    explicit gsys_in( inout_port_type& parent_ )
	: /*base_type()*/ sc_in<bool>(parent_), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    gsys_in( const char* name_, inout_port_type& parent_ )
	: /*base_type( name_ )*/ sc_in<bool>(name_,parent_), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    gsys_in( this_type& parent_ )
	: /*base_type( parent_ )*/ sc_in<bool>(parent_), m_traces( 0 )
	{}

    gsys_in( const char* name_, this_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_in<bool>(name_,parent_), m_traces( 0 )
	{}


    // destructor

    virtual ~gsys_in()
	{ remove_traces(); }


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( const_cast<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( const_cast<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }

    // get the positive edge event

    const sc_event& posedge_event() const
	{ return (*this)->posedge_event(); }

    // get the negative edge event

    const sc_event& negedge_event() const
	{ return (*this)->negedge_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_bool_deval& delayed() const;


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }


    // called by sc_trace
    void add_trace( sc_trace_file*, const sc_string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
    gsys_in( const this_type& );
    this_type& operator = ( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_bool_deval&
gsys_in<bool>::delayed() const
{
    const in_if_type* iface = dynamic_cast<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return reinterpret_cast<const sc_signal_bool_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return reinterpret_cast<const sc_signal_bool_deval&>( *pb );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : gsys_in<sc_logic>
//
//  Specialization of gsys_in<T> for type sc_logic.
// ----------------------------------------------------------------------------

template <>
class gsys_in<sc_logic> : public sc_in<sc_logic>
//: public sc_port<sc_signal_in_if<sc_logic>,1>
{
public:

    // typedefs

    typedef sc_logic                      data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef gsys_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

private:
    gsysPort *gPort;
    
public:

    // constructors

    gsys_in()
	: /*base_type()*/ sc_in<sc_logic>(), m_traces( 0 )
	{
            gPort = new gsysPort(0);
            gPort->setName("unnamedPort");
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_in( const char* name_ )
	: /*base_type( name_ )*/ sc_in<sc_logic>(name_), m_traces( 0 )
	{
            gPort = new gsysPort(0);
            gPort->setName((char*) name_);
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_in( const in_if_type& interface_ )
	: /*base_type( const_cast<in_if_type&>( interface_ ) )*/ sc_in<sc_logic>(interface_), m_traces( 0 )
	{}

    gsys_in( const char* name_, const in_if_type& interface_ )
	: /*base_type( name_, const_cast<in_if_type&>( interface_ ) )*/ sc_in<sc_logic>(name_,interface_), m_traces( 0 )
	{}

    explicit gsys_in( in_port_type& parent_ )
	: /*base_type( parent_ )*/ sc_in<sc_logic>(parent_), m_traces( 0 )
	{}

    gsys_in( const char* name_, in_port_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_in<sc_logic>(name_,parent_), m_traces( 0 )
	{}

    explicit gsys_in( inout_port_type& parent_ )
	: /*base_type()*/ sc_in<sc_logic>(parent_), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    gsys_in( const char* name_, inout_port_type& parent_ )
	: /*base_type( name_ )*/ sc_in<sc_logic>(name_,parent_), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    gsys_in( this_type& parent_ )
	: /*base_type( parent_ )*/ sc_in<sc_logic>(parent_), m_traces( 0 )
	{}

    gsys_in( const char* name_, this_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_in<sc_logic>(name_,parent_), m_traces( 0 )
	{}


    // destructor

    virtual ~gsys_in()
	{ remove_traces(); }


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( const_cast<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( const_cast<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }

    // get the positive edge event

    const sc_event& posedge_event() const
	{ return (*this)->posedge_event(); }

    // get the negative edge event

    const sc_event& negedge_event() const
	{ return (*this)->negedge_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_logic_deval& delayed() const;


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }


    // called by sc_trace
    void add_trace( sc_trace_file*, const sc_string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
    gsys_in( const this_type& );
    this_type& operator = ( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_logic_deval&
gsys_in<sc_logic>::delayed() const
{
    const in_if_type* iface = dynamic_cast<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return reinterpret_cast<const sc_signal_logic_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return reinterpret_cast<const sc_signal_logic_deval&>( *pb );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : gsys_inout<T>
//
//  The gsys_signal<T> input/output port class.
// ----------------------------------------------------------------------------

template <class T>
class gsys_inout : public sc_inout<T>
//: public sc_port<sc_signal_inout_if<T>,1>
{
public:

    // typedefs

    typedef T                             data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef gsys_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

private:
    gsysPort *gPort;
    
public:

    // constructors

    gsys_inout()
	: /*base_type()*/ sc_inout<T>(), m_init_val( 0 ), m_traces( 0 )
	{
            gPort = new gsysPort(2);
            gPort->setName("unnamedPort");
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_inout( const char* name_ )
	: /*base_type( name_ )*/ sc_inout<T>(name_), m_init_val( 0 ), m_traces( 0 )
	{
            gPort = new gsysPort(2);
            gPort->setName((char*) name_);
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_inout( inout_if_type& interface_ )
	: /*base_type( interface_ )*/ sc_inout<T>(interface_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, inout_if_type& interface_ )
	: /*base_type( name_, interface_ )*/ sc_inout<T>(name_,interface_), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit gsys_inout( inout_port_type& parent_ )
	: /*base_type( parent_ )*/ sc_inout<T>(parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, inout_port_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_inout<T>(name_,parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( this_type& parent_ )
	: /*base_type( parent_ )*/ sc_inout<T>(parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, this_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_inout<T>(name_,parent_), m_init_val( 0 ), m_traces( 0 )
	{}


    // destructor

    virtual ~gsys_inout();


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // write the new value

    this_type& write( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    data_type* m_init_val;

public:

    // called by sc_trace
    void add_trace( sc_trace_file*, const sc_string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

private:

    // disabled
    gsys_inout( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T>
const char* const gsys_inout<T>::kind_string = "gsys_inout";


// destructor

template <class T>
inline
gsys_inout<T>::~gsys_inout()
{
    if( m_init_val != 0 ) {
	delete m_init_val;
    }
    remove_traces();
}


// set initial value (can also be called when port is not bound yet)

template <class T>
inline
void
gsys_inout<T>::initialize( const data_type& value_ )
{
    inout_if_type* iface = dynamic_cast<inout_if_type*>( this->get_interface() );
    if( iface != 0 ) {
	iface->write( value_ );
    } else {
	if( m_init_val == 0 ) {
	    m_init_val = new data_type;
	}
	*m_init_val = value_;
    }
}


// called when elaboration is done

template <class T>
inline
void
gsys_inout<T>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = dynamic_cast<in_if_type*>( this->get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <class T>
inline
void
gsys_inout<T>::add_trace( sc_trace_file* tf_, const sc_string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	static std::string tmp_str(name_);
	m_traces->push_back( new sc_trace_params( tf_, tmp_str ) );
    }
}

template <class T>
inline
void
gsys_inout<T>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : gsys_inout<bool>
//
//  Specialization of gsys_inout<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class gsys_inout<bool> : public sc_inout<bool>
//: public sc_port<sc_signal_inout_if<bool>,1>
{
public:

    // typedefs

    typedef bool                          data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef gsys_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

private:
    gsysPort *gPort;
    
public:

    // constructors

    gsys_inout()
	: /*base_type()*/ sc_inout<bool>(), m_init_val( 0 ), m_traces( 0 )
	{
            gPort = new gsysPort(2);
            gPort->setName("unnamedPort");
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_inout( const char* name_ )
	: /*base_type( name_ )*/ sc_inout<bool>(name_), m_init_val( 0 ), m_traces( 0 )
	{
            gPort = new gsysPort(2);
            gPort->setName((char*) name_);
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
	}

    explicit gsys_inout( inout_if_type& interface_ )
	: /*base_type( interface_ )*/ sc_inout<bool>(interface_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, inout_if_type& interface_ )
	: /*base_type( name_, interface_ )*/ sc_inout<bool>(name_,interface_), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit gsys_inout( inout_port_type& parent_ )
	: /*base_type( parent_ )*/ sc_inout<bool>(parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, inout_port_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_inout<bool>(name_,parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( this_type& parent_ )
	: /*base_type( parent_ )*/ sc_inout<bool>(parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, this_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_inout<bool>(name_,parent_), m_init_val( 0 ), m_traces( 0 )
	{}


    // destructor

    virtual ~gsys_inout();


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }

    // get the positive edge event

    const sc_event& posedge_event() const
	{ return (*this)->posedge_event(); }

    // get the negative edge event

    const sc_event& negedge_event() const
	{ return (*this)->negedge_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_bool_deval& delayed() const;


    // write the new value

    this_type& write( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    data_type* m_init_val;

public:

    // called by sc_trace
    void add_trace( sc_trace_file*, const sc_string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

private:

    // disabled
    gsys_inout( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_bool_deval&
gsys_inout<bool>::delayed() const
{
    const in_if_type* iface = dynamic_cast<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return reinterpret_cast<const sc_signal_bool_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return reinterpret_cast<const sc_signal_bool_deval&>( *pb );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : gsys_inout<sc_logic>
//
//  Specialization of gsys_inout<T> for type sc_logic.
// ----------------------------------------------------------------------------

template <>
class gsys_inout<sc_logic> : public sc_inout<sc_logic>
//: public sc_port<sc_signal_inout_if<sc_logic>,1>
{
public:

    // typedefs

    typedef sc_logic                      data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef gsys_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

private:
    gsysPort *gPort;
    
public:

    // constructors

    gsys_inout()
	: /*base_type()*/ sc_inout<sc_logic>(), m_init_val( 0 ), m_traces( 0 )
	{
            gPort = new gsysPort(2);
            gPort->setName("unnamedPort");
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
        }

    explicit gsys_inout( const char* name_ )
	: /*base_type( name_ )*/ sc_inout<sc_logic>(name_), m_init_val( 0 ), m_traces( 0 )
	{
            gPort = new gsysPort(2);
            gPort->setName((char*) name_);
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
        }

    explicit gsys_inout( inout_if_type& interface_ )
	: /*base_type( interface_ )*/ sc_inout<sc_logic>(interface_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, inout_if_type& interface_ )
	: /*base_type( name_, interface_ )*/ sc_inout<sc_logic>(name_,interface_), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit gsys_inout( inout_port_type& parent_ )
	: /*base_type( parent_ )*/ sc_inout<sc_logic>(parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, inout_port_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_inout<sc_logic>(name_,parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( this_type& parent_ )
	: /*base_type( parent_ )*/ sc_inout<sc_logic>(parent_), m_init_val( 0 ), m_traces( 0 )
	{}

    gsys_inout( const char* name_, this_type& parent_ )
	: /*base_type( name_, parent_ )*/ sc_inout<sc_logic>(name_,parent_), m_init_val( 0 ), m_traces( 0 )
	{}


    // destructor

    virtual ~gsys_inout();


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }

    // get the positive edge event

    const sc_event& posedge_event() const
	{ return (*this)->posedge_event(); }

    // get the negative edge event

    const sc_event& negedge_event() const
	{ return (*this)->negedge_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_logic_deval& delayed() const;


    // write the new value

    this_type& write( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    data_type* m_init_val;

public:

    // called by sc_trace
    void add_trace( sc_trace_file*, const sc_string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

private:

    // disabled
    gsys_inout( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_logic_deval&
gsys_inout<sc_logic>::delayed() const
{
    const in_if_type* iface = dynamic_cast<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return reinterpret_cast<const sc_signal_logic_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return reinterpret_cast<const sc_signal_logic_deval&>( *pb );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : gsys_out<T>
//
//  The gsys_signal<T> output port class.
// ----------------------------------------------------------------------------

// gsys_out can also read from its port, hence no difference with gsys_inout.
// For debugging reasons, a class is provided instead of a define.

template <class T>
class gsys_out
: public gsys_inout<T>
{
public:

    // typedefs

    typedef T                                   data_type;

    typedef gsys_out<data_type>                   this_type;
    typedef gsys_inout<data_type>                 base_type;

    typedef typename base_type::in_if_type      in_if_type;
    typedef typename base_type::in_port_type    in_port_type;
    typedef typename base_type::inout_if_type   inout_if_type;
    typedef typename base_type::inout_port_type inout_port_type;

private:
    gsysPort *gPort;
    
public:

    // constructors

    gsys_out()
	: base_type()
	{
           gPort = new gsysPort(1);
           gPort->setName("unnamedPort");
           (new gsysMain())->getRegModule()->preRegPort(gPort,this);
        }

    explicit gsys_out( const char* name_ )
	: base_type( name_ )
	{
            gPort = new gsysPort(1);
            gPort->setName((char*) name_);
            (new gsysMain())->getRegModule()->preRegPort(gPort,this);
        }

    explicit gsys_out( inout_if_type& interface_ )
	: base_type( interface_ )
	{}

    gsys_out( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ )
	{}

    explicit gsys_out( inout_port_type& parent_ )
	: base_type( parent_ )
	{}

    gsys_out( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ )
	{}

    gsys_out( this_type& parent_ )
	: base_type( parent_ )
	{}

    gsys_out( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ )
	{}


    // destructor (does nothing)

    virtual ~gsys_out()
	{}


    // write the new value

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

private:

    // disabled
    gsys_out( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T>
const char* const gsys_out<T>::kind_string = "gsys_out";


// ----------------------------------------------------------------------------
//  FUNCTION : sc_trace
// ----------------------------------------------------------------------------

template <class T>
inline
void
sc_trace( sc_trace_file* tf, const gsys_in<T>& port, const sc_string& name )
{
    port.add_trace( tf, name );
}

template <>
inline
void
sc_trace<bool>( sc_trace_file* tf,
                const gsys_in<bool>& port,
                const sc_string& name )
{
    port.add_trace( tf, name );
}

template <>
inline
void
sc_trace<sc_logic>( sc_trace_file* tf,
                    const gsys_in<sc_logic>& port,
                    const sc_string& name )
{
    port.add_trace( tf, name );
}

template <class T>
inline
void
sc_trace( sc_trace_file* tf, const gsys_inout<T>& port, const sc_string& name )
{
    port.add_trace( tf, name );
}

template <>
inline
void
sc_trace<bool>( sc_trace_file* tf,
                const gsys_inout<bool>& port,
                const sc_string& name )
{
    port.add_trace( tf, name );
}

template <>
inline
void
sc_trace<sc_logic>( sc_trace_file* tf,
                    const gsys_inout<sc_logic>& port,
                    const sc_string& name )
{
    port.add_trace( tf, name );
}


#endif

// Taf!
