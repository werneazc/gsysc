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

/******************************************************************************
  This file is nearly completely identical to a merge of the SystemC-files
  sc_signal_ports.cc and sc_signal.cc
  Because of this the authors are the same as mentioned in these files.
  For gSysC it was necessary to adjust some things in this files, so 
  this file is used instead.
  Comments are not changed. 
 ******************************************************************************/

#include <sysc/communication/sc_communication_ids.h>
#include "gsysReplaced.h"


// to avoid code bloat in gsys_signal<T>

/*void
sc_signal_invalid_writer( const char* name,
			  const char* kind,
			  const char* first_writer,
			  const char* second_writer )
{
    char msg[BUFSIZ];
    sprintf( msg, "\n signal `%s' (%s)"
	     "\n first driver `%s'"
	     "\n second driver `%s'",
	     name, kind, first_writer, second_writer );
    SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_, msg );
}
*/

// ----------------------------------------------------------------------------
//  CLASS : gsys_signal<bool>
//
//  Specialization of gsys_signal<T> for type bool.
// ----------------------------------------------------------------------------

const char* const gsys_signal<bool>::kind_string = "gsys_signal";


// ----------------------------------------------------------------------------
//  CLASS : gsys_signal<sc_logic>
//
//  Specialization of gsys_signal<T> for type sc_logic.
// ----------------------------------------------------------------------------

const char* const gsys_signal<sc_logic>::kind_string = "gsys_signal";




// ----------------------------------------------------------------------------
//  CLASS : gsys_in<bool>
//
//  Specialization of gsys_in<T> for type bool.
// ----------------------------------------------------------------------------

const char* const gsys_in<bool>::kind_string = "gsys_in";


// called when elaboration is done

void
gsys_in<bool>::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = dynamic_cast<in_if_type*>( get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

void
gsys_in<bool>::add_trace( sc_trace_file* tf_, const sc_string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	static std::string tmp_str(name_);
	m_traces->push_back( new sc_trace_params( tf_, tmp_str ) );
    }
}

void
gsys_in<bool>::remove_traces() const
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

int
gsys_in<bool>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

int
gsys_in<bool>::vbind( sc_port_base& parent_ )
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
//  CLASS : gsys_in<sc_logic>
//
//  Specialization of gsys_in<T> for type sc_logic.
// ----------------------------------------------------------------------------

const char* const gsys_in<sc_logic>::kind_string = "gsys_in";


// called when elaboration is done

void
gsys_in<sc_logic>::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = dynamic_cast<in_if_type*>( get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

void
gsys_in<sc_logic>::add_trace( sc_trace_file* tf_, const sc_string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	static std::string tmp_str(name_);
	m_traces->push_back( new sc_trace_params( tf_, tmp_str ) );
    }
}

void
gsys_in<sc_logic>::remove_traces() const
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

int
gsys_in<sc_logic>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

int
gsys_in<sc_logic>::vbind( sc_port_base& parent_ )
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
//  CLASS : gsys_inout<bool>
//
//  Specialization of gsys_inout<T> for type bool.
// ----------------------------------------------------------------------------

const char* const gsys_inout<bool>::kind_string = "gsys_inout";


// destructor

gsys_inout<bool>::~gsys_inout()
{
    if( m_init_val != 0 ) {
	delete m_init_val;
    }
    remove_traces();
}


// set initial value (can also be called when port is not bound yet)

void
gsys_inout<bool>::initialize( const data_type& value_ )
{
    inout_if_type* iface = dynamic_cast<inout_if_type*>( get_interface() );
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

void
gsys_inout<bool>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = dynamic_cast<in_if_type*>( get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

void
gsys_inout<bool>::add_trace( sc_trace_file* tf_, const sc_string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	static std::string tmp_str(name_);
	m_traces->push_back( new sc_trace_params( tf_, tmp_str ) );
    }
}

void
gsys_inout<bool>::remove_traces() const
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
//  CLASS : gsys_inout<sc_logic>
//
//  Specialization of gsys_inout<T> for type sc_logic.
// ----------------------------------------------------------------------------

const char* const gsys_inout<sc_logic>::kind_string = "gsys_inout";


// destructor

gsys_inout<sc_logic>::~gsys_inout()
{
    if( m_init_val != 0 ) {
	delete m_init_val;
    }
    remove_traces();
}


// set initial value (can also be called when port is not bound yet)

void
gsys_inout<sc_logic>::initialize( const data_type& value_ )
{
    inout_if_type* iface = dynamic_cast<inout_if_type*>( get_interface() );
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

void
gsys_inout<sc_logic>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = dynamic_cast<in_if_type*>( get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

void
gsys_inout<sc_logic>::add_trace( sc_trace_file* tf_,
			       const sc_string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	static std::string tmp_str(name_);
	m_traces->push_back( new sc_trace_params( tf_, tmp_str ) );
    }
}

void
gsys_inout<sc_logic>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// Taf!
